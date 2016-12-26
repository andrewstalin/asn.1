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

#ifndef _ASN1_BIT_STRING_H_
#define _ASN1_BIT_STRING_H_

#include "asn1/header.h"
#include "asn1/encoders/stream_encoder.h"
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <climits>

namespace asn1
{
	struct BitString : Asn1Value
	{
		std::vector<byte_type> bytes;
		uint8_t unused_bits{ 0 };

		BitString()
			: Asn1Value(BIT_STRING_TAG)
		{}

		template<typename integral_type>
		explicit BitString(integral_type val)
			: Asn1Value(BIT_STRING_TAG)
		{
			static_assert(std::is_integral<integral_type>::value, "integral type required");

			if (val > 0)
			{
				bytes.reserve(sizeof(integral_type));

				while (val > 0)
				{
					bytes.push_back(static_cast<byte_type>(val & 0xFF));
					val = val >> CHAR_BIT;
				}

				std::reverse(bytes.begin(), bytes.end());
			}
			else
			{
				bytes = { 0x00 };
			}
		}

		BitString(const Tag& tag)
			: Asn1Value(tag)
		{}

		static BitString parse(const std::string& str);

		std::string to_string() const;

		template<typename integral_type>
		integral_type to_integral_type() const
		{
			static_assert(std::is_integral<integral_type>::value, "integral type required");

			auto bytes_count = bytes.size();
			THROW_IF(sizeof(integral_type) < bytes_count, Asn1Exception(errors::DATA_TYPE_TOO_SMALL));
			integral_type result{ 0 };
			uint8_t i{ static_cast<uint8_t>(bytes_count) };

			for (auto b : bytes)
			{
				if (--i == 0 && unused_bits != 0)
				{
					b = b >> unused_bits;
				}

				result = result | (b << (CHAR_BIT * i));
			}

			return result;
		}

		length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
		{
			return encoder.encode_bit_string(writer, tag, unused_bits, bytes.data(), bytes.size());
		}
	};

	class StreamingBitString : public details::StreamEncoder
	{
	public:
		StreamingBitString()
			: details::StreamEncoder(BIT_STRING_TAG)
		{}

		StreamingBitString(const Tag& tag)
			: details::StreamEncoder(tag)
		{}

		//todo: content_length must not equal indefinite length
		StreamingBitString(const Tag& tag, length_type content_length)
			: details::StreamEncoder(tag, content_length)
		{}

		uint8_t unused_bits() const { return unused_bits_; }
		void set_unused_bits(uint8_t val) { unused_bits_ = val; }

		void set_block_size(size_t size)
		{
			stream_block_ = StreamBlock(BIT_STRING_TAG, size);
		}
	};

	class BitStringDecoder : public BerDecoder
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

		struct BitStringDecoderEventHandler : IValueEventHandler
		{
			BitStringDecoder* bit_string_decoder;

			BitStringDecoderEventHandler(BitStringDecoder* bit_string_decoder)
				: bit_string_decoder(bit_string_decoder)
			{}

			void execute(Asn1Value&& value) override
			{
				bit_string_decoder->consume(std::move(value));
			}

			void execute(const Tag&, Asn1Value&&) override {}
		};

		State state_{ State::INITIAL };
		HeaderDecoder header_decoder_;
		length_type content_length_{ 0 };

		IValueEventHandler* value_event_handler_{ nullptr };

		BitStringDecoderEventHandler constructed_type_event_handler_;
		std::unique_ptr<BitStringDecoder> constructed_type_decoder_;

		BitString value_;

	public:
		BitStringDecoder(IValueEventHandler* const event_handler)
			: BitStringDecoder(BIT_STRING_TAG, event_handler)
		{}

		BitStringDecoder(const Tag& tag, IValueEventHandler* const event_handler)
			: BerDecoder(tag)
			, value_event_handler_(event_handler)
			, constructed_type_event_handler_(this)
		{}

		bool finished() const override { return state_ == State::COMPLETED; }

		void reset_state() override
		{
			state_ = State::INITIAL;
			header_decoder_.reset_state();
			content_length_ = 0;
			constructed_type_decoder_.reset();
			value_ = BitString();
		}

		DecodingResult decode(const byte_type* data, size_type size) override;

	private:
		void consume(Asn1Value&& val);
	};
}

#endif
