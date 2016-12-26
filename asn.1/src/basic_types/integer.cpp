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

#include "asn1/basic_types/integer.h"
#include "decoding.h"
#include "convert.h"
#include <algorithm>

using namespace asn1;

Integer::Integer(int64_t val, const Tag& tag)
	: Asn1Value(tag), bytes(asn1::int_to_bytes(val))
{
	std::reverse(bytes.begin(), bytes.end());
}

Integer& Integer::operator=(int64_t val)
{
	bytes = asn1::int_to_bytes(val);
	std::reverse(bytes.begin(), bytes.end());
	return *this;
}

int64_t Integer::to_int64() const
{
	return asn1::bytes_to_int(std::vector<asn1::byte_type>(bytes.rbegin(), bytes.rend()));
}

std::string Integer::to_hex_string() const
{
	return asn1::to_hext_string(bytes);
}

DecodingResult IntegerDecoder::decode(const byte_type* data, size_type size)
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
			CHECK_TAG(header_decoder_.tag(), tag());

			content_length_ = header_decoder_.content_length();
			value_.tag = header_decoder_.tag();
			value_.bytes.reserve(static_cast<size_t>(content_length_));
			state_ = State::BYTES_DEFINITION;
		}
		break;
		case State::BYTES_DEFINITION:
		{
			char byte{ INT8_MAX };

			while (content_length_ > 0)
			{
				READ_BYTE(decoding_result, data, byte);
				--content_length_;
				value_.bytes.push_back(byte);
				//value_.bytes.at(static_cast<size_t>(content_length_)) = byte;
			}

			state_ = State::COMPLETED;
			decoding_result.code = DecodingCode::OK;
			on_decoding_completion(value_event_handler_, std::move(value_));
		}
		break;
		default:
			throw Asn1Exception(errors::WRONG_DECODER_STATE);
		}
	}

	return decoding_result;
}

