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

#include "asn1/basic_types/octet_string.h"
#include "decoding.h"
#include <cassert>

using namespace asn1;

DecodingResult OctetStringDecoder::decode(const byte_type* data, size_type size)
{
	CHECK_PRECONDITIONS(data, size, finished());
	DecodingResult decoding_result = { 0, DecodingCode::UNDEFINED };

	while (state_ != State::COMPLETED)
	{
		switch (state_)
		{
		case State::INITIAL:
			{
				decoding_result << header_decoder_.decode(data, size);
				CHECK_RESULT(decoding_result);
	
				//check without encoding form (for constructed form)
				if (header_decoder_.tag().tag_class != tag().tag_class || header_decoder_.tag().identifier != tag().identifier)
				{
					throw Asn1Exception(errors::WRONG_TAG);
				}

				content_length_ = header_decoder_.content_length();
				value_.tag = header_decoder_.tag();

				if (content_length_ == INDEFINITE_CONTENT_LENGTH)
				{
					constructed_type_decoder_ = std::make_unique<OctetStringDecoder>(subcollection_tag_, &constructed_type_event_handler_);
				}
				else if (!data_event_handler_)
				{
					value_.bytes.reserve(static_cast<size_t>(content_length_));
				}

				state_ = State::BYTES_DEFINITION;
			}
			break;
		case State::BYTES_DEFINITION:
			{
				if (header_decoder_.tag().encoding_form == EncodingForm::CONSTRUCTED)
				{
					assert(constructed_type_decoder_ != nullptr);

					while (true)
					{
						auto dr = constructed_type_decoder_->decode(data + decoding_result.read_bytes, size - decoding_result.read_bytes);
						decoding_result << dr;

						if (content_length_ != INDEFINITE_CONTENT_LENGTH)
						{
							content_length_ -= dr.read_bytes;
						}

						if (decoding_result.code != DecodingCode::OK && decoding_result.code != DecodingCode::END_CONTENT)
						{
							return decoding_result;
						}

						if (content_length_ == INDEFINITE_CONTENT_LENGTH)
						{
							if (decoding_result.code == DecodingCode::END_CONTENT)
							{
								state_ = State::COMPLETED;
								decoding_result.code = DecodingCode::OK;
								on_decoding_completion(value_event_handler_, std::move(value_));

								return decoding_result;
							}
						}
						else
						{
							// Unexpected termination of the data
							THROW_IF(decoding_result.code == DecodingCode::END_CONTENT, Asn1Exception(errors::WRONG_STRUCTURE));

							if (content_length_ == 0)
							{
								state_ = State::COMPLETED;
								decoding_result.code = DecodingCode::OK;
								on_decoding_completion(value_event_handler_, std::move(value_));

								return decoding_result;
							}
						}

						constructed_type_decoder_->reset_state();
					}
				}
				else
				{
					auto remaining_size = size - decoding_result.read_bytes;

					if (content_length_ <= remaining_size)
					{
						write(data + decoding_result.read_bytes, static_cast<size_type>(content_length_));
						decoding_result.read_bytes += static_cast<size_type>(content_length_);
						content_length_ = 0;
						decoding_result.code = DecodingCode::OK;
						state_ = State::COMPLETED;
						on_decoding_completion(value_event_handler_, std::move(value_));
					}
					else
					{
						if (remaining_size > 0)
						{
							write(data + decoding_result.read_bytes, remaining_size);
							decoding_result.read_bytes += remaining_size;
							content_length_ -= remaining_size;
						}

						decoding_result.code = DecodingCode::MORE_DATA;
					}

					return decoding_result;
				}
			}
			break;
		default:
			throw Asn1Exception(errors::WRONG_DECODER_STATE);
		}
	}

	return decoding_result;
}
