// Licensed under the MIT License <http://opensource.org/licenses/MIT>
// Author: Andrew Stalin <andrew.stalin@gmail.com>
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _ASN1_OCTET_STRING_H_
#define _ASN1_OCTET_STRING_H_

#include "asn1/header.h"
#include "asn1/encoders/stream_encoder.h"
#include <vector>
#include <memory>

namespace asn1
{
	struct OctetString : Asn1Value
	{
		std::vector<byte_type> bytes;

		OctetString()
			: Asn1Value(OCTET_STRING_TAG)
		{}

		OctetString(const byte_type* data, size_type size)
			: Asn1Value(OCTET_STRING_TAG), bytes(data, data + size)
		{}

		OctetString(std::vector<byte_type>&& bytes)
			: Asn1Value(OCTET_STRING_TAG), bytes(std::move(bytes))
		{}

		OctetString(const Tag& tag)
			: Asn1Value(tag)
		{}

		OctetString(const Tag& tag, std::vector<byte_type>&& bytes)
			: Asn1Value(tag), bytes(std::move(bytes))
		{}

		length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
		{
			return encoder.encode_octet_string(writer, tag, bytes.data(), bytes.size());
		}
	};

	class StreamingOctetString : public details::StreamEncoder
	{
	public:
		StreamingOctetString()
			: details::StreamEncoder(OCTET_STRING_TAG)
		{}

		StreamingOctetString(const Tag& tag)
			: details::StreamEncoder(tag)
		{}

		//todo: content_length must not equal indefinite length
		StreamingOctetString(const Tag& tag, length_type content_length)
			: details::StreamEncoder(tag, content_length)
		{}

		void set_block_size(size_type size)
		{
			stream_block_ = StreamBlock(OCTET_STRING_TAG, size);
		}
	};

	class OctetStringDecoder : public BerDecoder
	{
	private:
		enum class State : uint8_t
		{
			INITIAL,
			UNUSED_BITS_DEFINITION,
			BYTES_DEFINITION,
			PROCESS_SUBCOLLECTIONS,
			COMPLETED
		};

		struct OctetStringDecoderEventHandler : IDataEventHandler
		{
			OctetStringDecoder* octet_string_decoder;

			OctetStringDecoderEventHandler(OctetStringDecoder* octet_string_decoder)
				: octet_string_decoder(octet_string_decoder)
			{}

			void execute(const byte_type* data, size_type size) override
			{
				octet_string_decoder->write(data, size);
			}
		};

		std::unique_ptr<OctetStringDecoder> constructed_type_decoder_;
		OctetStringDecoderEventHandler constructed_type_event_handler_;

		OctetString value_;
		HeaderDecoder header_decoder_;
		Tag subcollection_tag_{ OCTET_STRING_TAG };
		length_type content_length_{ 0 };
		IValueEventHandler* const value_event_handler_{ nullptr };
		IDataEventHandler* const data_event_handler_{ nullptr };
		State state_{ State::INITIAL };

	public:
		explicit OctetStringDecoder(IValueEventHandler* const event_handler)
			: OctetStringDecoder(OCTET_STRING_TAG, event_handler)
		{}

		explicit OctetStringDecoder(IDataEventHandler* const event_handler)
			: OctetStringDecoder(OCTET_STRING_TAG, event_handler)
		{}

		OctetStringDecoder(const Tag& tag, IValueEventHandler* const event_handler)
			: BerDecoder(tag)
			, constructed_type_event_handler_(this)
			, value_event_handler_(event_handler)
		{}

		OctetStringDecoder(const Tag& tag, IDataEventHandler* const event_handler)
			: BerDecoder(tag)
			, constructed_type_event_handler_(this)
			, data_event_handler_(event_handler)
		{}

		const Tag& subcollection_tag() const { return subcollection_tag_; }
		void set_subcollection_tag(const Tag& val) { subcollection_tag_ = val; }

		bool finished() const override { return state_ == State::COMPLETED; }

		void reset_state() override
		{
			state_ = State::INITIAL;
			header_decoder_.reset_state();
			content_length_ = 0;
			constructed_type_decoder_.reset();
			value_ = OctetString();
		}

		DecodingResult decode(const byte_type* data, size_type size) override;

	private:
		void write(const byte_type* data, size_type size)
		{
			if (data_event_handler_ != nullptr)
			{
				data_event_handler_->execute(data, size);
			}
			else
			{
				value_.bytes.insert(value_.bytes.end(), data, data + size);
			}
		}
	};
}

#endif