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

#include "asn1/basic_types/collection.h"
#include "decoding.h"
#include <cassert>

using namespace asn1;

void details::CollectionDecoder::reset_state()
{
	header_decoder_.reset_state();
	state_ = State::INITIAL;
	content_length_ = 0;
	element_decoder_ = nullptr;

	for (auto& decoder : decoders_)
	{
		decoder->reset_state();
	}
}

DecodingResult details::CollectionDecoder::decode(const byte_type* data, size_type size)
{
	CHECK_PRECONDITIONS(data, size, finished());
	DecodingResult decoding_result = { 0, DecodingCode::UNDEFINED };

	while (state_ != State::COMPLETED)
	{
		switch (state_)
		{
		case State::INITIAL:
		{
			decoding_result = header_decoder_.decode(data, size);
			CHECK_RESULT(decoding_result);
			CHECK_TAG(header_decoder_.tag(), tag());

			content_length_ = header_decoder_.content_length();

			if (content_length_ == 0)
			{
				notify(tag());
				state_ = State::COMPLETED;
			}
			else
			{
				header_decoder_.reset_state(); // reset header decoder state for decode sequence elements headers
				state_ = State::ELEMENT_HEADER_DEFINITION;
			}
		}
		break;
		case State::ELEMENT_HEADER_DEFINITION:
		{
			decoding_result << header_decoder_.decode(data + decoding_result.read_bytes, size - decoding_result.read_bytes);

			if (decoding_result.code == DecodingCode::OK)
			{
				state_ = State::DECODER_DEFINITION;
			}
			else if (decoding_result.code == DecodingCode::END_CONTENT)
			{
				decoding_result.code = DecodingCode::OK;
				state_ = State::COMPLETED;
				notify(tag());
			}
			else
			{
				return decoding_result;
			}
		}
		break;
		case State::DECODER_DEFINITION:
		{
			if (!is_all_proccessed())
			{
				const auto& header_tag = header_decoder_.tag();
				element_decoder_ = get_next_decoder(header_tag);
				assert(element_decoder_ != nullptr);
				const auto& decoder_tag = element_decoder_->tag();

				if (content_length_ == 0 
					|| (decoder_tag != UNDEFINED_TAG 
						&& (header_tag.identifier != decoder_tag.identifier || header_tag.tag_class != decoder_tag.tag_class)))
				{
					if (element_decoder_->repeatable())
					{
						// çàêîí÷èëàñü ïîñëåäîâàòåëüíîñòü ïîâòîðÿåìîãî ýëåìåíòà
					}
					else if (element_decoder_->optional())
					{
						//else if (decoder->has_default_value())
						//{
						//	value_->add(decoder->tag(), decoder->default_value());
						//}
					}
					else
					{
						throw Asn1Exception(errors::MISSING_COLLECTION_ELEMENT);
					}

					mark_as_processed(element_decoder_);
				}
				else
				{
					state_ = State::DECODING_ELEMENT;
					element_decoder_->decode(header_decoder_.header_data(), header_decoder_.header_size());
				}
			}
			else
			{
				// read end content token
				if (content_length_ == INDEFINITE_CONTENT_LENGTH)
				{
					decoding_result << header_decoder_.decode(data + decoding_result.read_bytes, size - decoding_result.read_bytes);

					if (decoding_result.code == DecodingCode::MORE_DATA)
					{
						return decoding_result;
					}
					else if (decoding_result.code != DecodingCode::END_CONTENT)
					{
						throw Asn1Exception(errors::WRONG_STRUCTURE);
					}

					decoding_result.code = DecodingCode::OK;
				}
				else if (content_length_ != 0)
				{
					throw Asn1Exception(errors::WRONG_ELEMENT_LENGTH);
				}

				state_ = State::COMPLETED;
				notify(tag());
			}
		}
		break;
		case State::DECODING_ELEMENT:
		{
			assert(element_decoder_ != nullptr);

			decoding_result << element_decoder_->decode(data + decoding_result.read_bytes, size - decoding_result.read_bytes);
			assert(decoding_result.code == DecodingCode::MORE_DATA || decoding_result.code == DecodingCode::OK);

			if (decoding_result.code == DecodingCode::MORE_DATA)
			{
				return decoding_result;
			}

			if (content_length_ != INDEFINITE_CONTENT_LENGTH)
			{
				auto element_length = header_decoder_.content_length();
				THROW_IF(content_length_ < element_length, Asn1Exception(errors::WRONG_ELEMENT_LENGTH));
				content_length_ -= element_length + header_decoder_.header_size();
			}

			// reset decoder
			if (element_decoder_->repeatable())
			{
				element_decoder_->reset_state();
			}
			else
			{
				mark_as_processed(element_decoder_);
			}

			element_decoder_ = nullptr;
			header_decoder_.reset_state();
			state_ = content_length_ == 0 ? State::DECODER_DEFINITION : State::ELEMENT_HEADER_DEFINITION;
		}
		break;
		default:
			throw Asn1Exception(errors::WRONG_DECODER_STATE);
		}
	}

	return decoding_result;
}
