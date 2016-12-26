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

#ifndef _ASN1_EXPLICIT_H_
#define _ASN1_EXPLICIT_H_

#include "asn1/header.h"
#include "asn1/error.h"

namespace asn1
{
	template<typename decoder_type>
	class ExplicitDecoder : public BerDecoder
	{
	private:
		enum class State : uint8_t
		{
			INITIAL,
			VALUE_DEFINITION,
			END_CONTENT_DEFINITION,
			COMPLETED
		};

		struct ValueEventHandler : IValueEventHandler
		{
			ExplicitDecoder* decoder;

			ValueEventHandler(ExplicitDecoder* decoder)
				: decoder(decoder)
			{}

			void execute(Asn1Value&& val) override
			{
				if (decoder->value_event_handler_ != nullptr)
				{
					decoder->value_event_handler_->execute(decoder->tag(), std::move(val));
				}
			}

			void execute(const Tag&, Asn1Value&&) override {}
		};

		ValueEventHandler internal_event_handler_;
		decoder_type decoder_;
		HeaderDecoder header_decoder_;
		length_type content_length_{ 0 };
		State state_{ State::INITIAL };
		IValueEventHandler* const value_event_handler_;

	public:
		ExplicitDecoder(const Tag& explicit_tag, IValueEventHandler* const event_handler)
			: BerDecoder(explicit_tag)
			, internal_event_handler_(this)
			, decoder_(&internal_event_handler_)
			, value_event_handler_(event_handler)
		{}

		ExplicitDecoder(const Tag& explicit_tag, IDataEventHandler* const data_event_handler)
			: BerDecoder(explicit_tag)
			, internal_event_handler_(this)
			, decoder_(data_event_handler)
			, value_event_handler_(nullptr)
		{}

		ExplicitDecoder(const Tag& explicit_tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
			: BerDecoder(explicit_tag)
			, internal_event_handler_(this)
			, decoder_(&internal_event_handler_, data_event_handler)
			, value_event_handler_(event_handler)
		{}

		ExplicitDecoder(const Tag& explicit_tag, const Tag& tag, IValueEventHandler* const event_handler)
			: BerDecoder(explicit_tag)
			, internal_event_handler_(this)
			, decoder_(tag, &internal_event_handler_)
			, value_event_handler_(event_handler)
		{}

		ExplicitDecoder(const Tag& explicit_tag, const Tag& tag, IDataEventHandler* const data_event_handler)
			: BerDecoder(explicit_tag)
			, internal_event_handler_(this)
			, decoder_(tag, data_event_handler)
			, value_event_handler_(nullptr)
		{}

		ExplicitDecoder(const Tag& explicit_tag, const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
			: BerDecoder(explicit_tag)
			, internal_event_handler_(this)
			, decoder_(tag, &internal_event_handler_, data_event_handler)
			, value_event_handler_(event_handler)
		{}

		const decoder_type& decoder() const { return decoder_; }

		bool finished() const override { return state_ == State::COMPLETED; }

		void reset_state() override
		{
			state_ = State::INITIAL;
			header_decoder_.reset_state();
			content_length_ = 0;
			decoder_.reset_state();
		}

		DecodingResult decode(const byte_type* data, size_type size) override
		{
			if (finished())
			{
				return{ 0, DecodingCode::OK };
			}
			else if (data == nullptr || size == 0)
			{
				return{ 0, DecodingCode::MORE_DATA };
			}

			DecodingResult decoding_result = { 0, DecodingCode::UNDEFINED };

			while (state_ != State::COMPLETED)
			{
				switch (state_)
				{
				case State::INITIAL:
				{
					decoding_result << header_decoder_.decode(data, size);

					if (decoding_result.code != DecodingCode::OK)
					{
						return decoding_result;
					}

					THROW_IF(header_decoder_.tag() != tag(), Asn1Exception(errors::WRONG_TAG));

					content_length_ = header_decoder_.content_length();
					state_ = State::VALUE_DEFINITION;
				}
				break;
				case State::VALUE_DEFINITION:
				{
					auto dr = decoder_.decode(data + decoding_result.read_bytes, size - decoding_result.read_bytes);
					decoding_result << dr;

					if (content_length_ != INDEFINITE_CONTENT_LENGTH)
					{
						content_length_ -= dr.read_bytes;
					}

					if (decoding_result.code != DecodingCode::OK)
					{
						return decoding_result;
					}

					if (content_length_ == INDEFINITE_CONTENT_LENGTH)
					{
						header_decoder_.reset_state();
						state_ = State::END_CONTENT_DEFINITION;
					}
					else
					{
						THROW_IF(content_length_ != 0, Asn1Exception(errors::WRONG_ELEMENT_LENGTH));
						state_ = State::COMPLETED;
					}
				}
				break;
				case State::END_CONTENT_DEFINITION:
				{
					auto dr = header_decoder_.decode(data + decoding_result.read_bytes, size - decoding_result.read_bytes);
					decoding_result.read_bytes += dr.read_bytes;
					decoding_result.code = dr.code;

					if (decoding_result.code != DecodingCode::MORE_DATA && decoding_result.code != DecodingCode::END_CONTENT)
					{
						throw Asn1Exception(errors::WRONG_STRUCTURE);
					}
					else if (decoding_result.code == DecodingCode::MORE_DATA)
					{
						return decoding_result;
					}
					else
					{
						state_ = State::COMPLETED;
						decoding_result.code = DecodingCode::OK;
					}
				}
				break;
				default:
					throw Asn1Exception(errors::WRONG_DECODER_STATE);
				}
			}

			return decoding_result;
		}
	};
}

#endif
