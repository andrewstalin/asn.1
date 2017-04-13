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

#ifndef _ASN1_ANY_H_
#define _ASN1_ANY_H_

#include "asn1/header.h"
#include <memory>

namespace asn1
{
	struct Any : Asn1Value
	{
		std::vector<byte_type> bytes;

		Any()
			: Asn1Value(UNDEFINED_TAG)
		{}

		explicit Any(std::vector<byte_type>&& bytes)
			: Asn1Value(UNDEFINED_TAG), bytes(std::move(bytes))
		{}

		length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
		{
			if (bytes.empty())
			{
				throw;
			}

			writer.write(bytes.data(), bytes.size());
			return bytes.size();
		}
	};

	class AnyDecoder : public BerDecoder
	{
	private:
		enum class State : uint8_t
		{
			INITIAL,
			BYTES_DEFINITION,
			COMPLETED
		};

		struct AnyDecoderEventHandler : IDataEventHandler
		{
			AnyDecoder* any_decoder;

			AnyDecoderEventHandler(AnyDecoder* any_decoder)
				: any_decoder(any_decoder)
			{}

			void execute(const byte_type* data, size_type size) override
			{
				any_decoder->write(data, size);
			}
		};

		State state_{ State::INITIAL };
		length_type content_length_{ 0 };
		HeaderDecoder header_decoder_;

		IValueEventHandler* const value_event_handler_{ nullptr };
		IDataEventHandler* const data_event_handler_{ nullptr };

		AnyDecoderEventHandler constructed_type_event_handler_;
		std::unique_ptr<AnyDecoder> constructed_type_decoder_; // decoder for constructed type with indefinite length

		Any value_;

	public:
		AnyDecoder(IValueEventHandler* const event_handler) noexcept
			: BerDecoder(UNDEFINED_TAG)
			, value_event_handler_(event_handler)
			, constructed_type_event_handler_(this)
		{}

		AnyDecoder(IDataEventHandler* const event_handler) noexcept
			: BerDecoder(UNDEFINED_TAG)
			, data_event_handler_(event_handler)
			, constructed_type_event_handler_(this)
		{}

		AnyDecoder(const Tag& tag, IValueEventHandler* const event_handler) noexcept
			: BerDecoder(tag)
			, value_event_handler_(event_handler)
			, constructed_type_event_handler_(this)
		{}

		AnyDecoder(const Tag& tag, IDataEventHandler* const event_handler) noexcept
			: BerDecoder(tag)
			, data_event_handler_(event_handler)
			, constructed_type_event_handler_(this)
		{}

		bool finished() const override { return state_ == State::COMPLETED; }

		void reset_state() override
		{
			state_ = State::INITIAL;
			header_decoder_.reset_state();
			constructed_type_decoder_.reset();
			content_length_ = 0;
			value_ = Any();
		}

		DecodingResult decode(const byte_type* data, size_type size) override;

	private:
		void write(const byte_type* data, size_type size);
	};
}

#endif