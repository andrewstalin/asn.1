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

#include "asn1/header.h"
#include "convert.h"
#include "decoding.h"
#include <climits>

using namespace asn1;

DecodingResult HeaderDecoder::decode(const byte_type* data, size_type size)
{
	CHECK_PRECONDITIONS(data, size, finished());
	DecodingResult decoding_result = { 0, DecodingCode::UNDEFINED };

	byte_type byte{ 0 };

	while (state_ != State::COMPLETED)
	{
		switch (state_)
		{
		case State::INITIAL:
			{
				READ_BYTE(decoding_result, data, byte);

				tag_.tag_class = static_cast<TagClass>(byte >> 6);
				tag_.encoding_form = (byte & 0x20) == 0x20 ? EncodingForm::CONSTRUCTED : EncodingForm::PRIMITIVE;

				if ((byte & 0x1f) == 0x1f)
				{
					tag_.identifier = 0;
					state_ = State::IDENTIFIER_DEFINITION;
				}
				else
				{
					tag_.identifier = byte & 0x1f;
					state_ = tag_.identifier == 0 && tag_.tag_class == TagClass::UNIVERSAL ? State::END_CONTENT_DEFINITION : State::LENGTH_MODE_DEFINITION;
				}

				buffer_.push_back(byte);
			}
			break;
		case State::END_CONTENT_DEFINITION:
			{
				READ_BYTE(decoding_result, data, byte);
				THROW_IF(byte != 0, Asn1Exception(errors::WRONG_STRUCTURE));

				state_ = State::COMPLETED;
				decoding_result.code = DecodingCode::END_CONTENT;
				return decoding_result;
			}
			break;
		case State::IDENTIFIER_DEFINITION:
			{
				READ_BYTE(decoding_result, data, byte);
				uint8_t bitsCount{ CHAR_BIT - 1 };

				while ((byte & 0x80) == 0x80)
				{
					tag_.identifier |= (byte & 0x7f);
					tag_.identifier = tag_.identifier << bitsCount;
					READ_BYTE(decoding_result, data, byte);
				}

				tag_.identifier |= byte;
				state_ = State::LENGTH_MODE_DEFINITION;
				buffer_.push_back(byte);
			}
			break;
		case State::LENGTH_MODE_DEFINITION:
			{
				READ_BYTE(decoding_result, data, byte);

				if ((byte & 0x80) == 0)
				{
					content_length_ = byte;
					state_ = State::COMPLETED;
					decoding_result.code = DecodingCode::OK;
				}
				else
				{
					if ((byte & 0x7f) == 0)
					{
						content_length_ = INDEFINITE_CONTENT_LENGTH;
						state_ = State::COMPLETED;
						decoding_result.code = DecodingCode::OK;
					}
					else
					{
						length_bytes_ = (byte & 0x7f) - 1;
						content_length_ = 0;
						state_ = State::LENGTH_DEFINITION;
					}
				}
				buffer_.push_back(byte);
			}
			break;
		case State::LENGTH_DEFINITION:
			{
				while (length_bytes_ > 0)
				{
					READ_BYTE(decoding_result, data, byte);
					content_length_ |= byte;
					content_length_ = content_length_ << CHAR_BIT;
					--length_bytes_;
					buffer_.push_back(byte);
				}

				READ_BYTE(decoding_result, data, byte);
				content_length_ |= byte;
				state_ = State::COMPLETED;
				decoding_result.code = DecodingCode::OK;
				buffer_.push_back(byte);
			}
			break;
		default:
			throw Asn1Exception(errors::WRONG_DECODER_STATE);
		}
	}

	return decoding_result;
}
