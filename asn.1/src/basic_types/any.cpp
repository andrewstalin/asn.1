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

#include "asn1/basic_types/any.h"
#include "decoding.h"
#include <cassert>

using namespace asn1;

DecodingResult AnyDecoder::decode(const byte_type* data, size_type size)
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
				write(data, decoding_result.read_bytes);
				CHECK_RESULT(decoding_result);

				content_length_ = header_decoder_.content_length();

				// set tag to ANY value (useful in the future)
				value_.tag = header_decoder_.tag();

				if (content_length_ == INDEFINITE_CONTENT_LENGTH)
				{
					constructed_type_decoder_ = std::make_unique<AnyDecoder>(&constructed_type_event_handler_);
				}
				else if (data_event_handler_ == nullptr)
				{
					//todo: throw if greater
					value_.bytes.reserve(static_cast<size_t>(content_length_));
				}

				state_ = State::BYTES_DEFINITION;
			}
			break;
		case State::BYTES_DEFINITION:
			{
				if (content_length_ == INDEFINITE_CONTENT_LENGTH)
				{
					assert(constructed_type_decoder_ != nullptr);

					while (decoding_result.code != DecodingCode::END_CONTENT)
					{
						decoding_result << constructed_type_decoder_->decode(data + decoding_result.read_bytes, size - decoding_result.read_bytes);

						if (decoding_result.code == DecodingCode::OK)
						{
							constructed_type_decoder_->reset_state();
						}
						else if (decoding_result.code != DecodingCode::END_CONTENT)
						{
							return decoding_result;
						}
					}

					state_ = State::COMPLETED;
					decoding_result.code = DecodingCode::OK;
					on_decoding_completion(value_event_handler_, std::move(value_));
				}
				else
				{
					size_type remaining_size{ size - decoding_result.read_bytes };

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
						write(data + decoding_result.read_bytes, remaining_size);
						decoding_result.read_bytes += remaining_size;
						decoding_result.code = DecodingCode::MORE_DATA;
						content_length_ -= remaining_size;
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

void AnyDecoder::write(const byte_type* data, size_type size)
{
	if (data_event_handler_ != nullptr)
	{
		assert(value_event_handler_ == nullptr);
		data_event_handler_->execute(data, size);
	}
	else
	{
		assert(value_event_handler_ != nullptr);
		value_.bytes.insert(value_.bytes.end(), data, data + size);
	}
}