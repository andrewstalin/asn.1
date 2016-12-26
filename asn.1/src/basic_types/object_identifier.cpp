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

#include "asn1//basic_types/object_identifier.h"
#include "decoding.h"
#include "asn1/error.h"
#include <sstream>
#include <iterator>

using namespace asn1;

bool ObjectIdentifier::validate_oid_numbers(const std::vector<uint64_t>& numbers)
{
	// object identifier must contain at least two bytes
	if (numbers.size() > 1)
	{
		auto sub_id1 = numbers[0];
		auto sub_id2 = numbers[1];

		// the first sub-identifier must equal 0,1,2
		if (sub_id1 <= 2)
		{
			if (sub_id1 == 2 || sub_id2 <= 39)
			{
				return true;
			}
		}
	}

	return false;
}

ObjectIdentifier ObjectIdentifier::parse(const std::string& oid)
{
	ObjectIdentifier object_identifier;

	if (!oid.empty())
	{
		size_t numbers_count{ 1 };

		for (char ch : oid)
		{
			if (ch == '.')
			{
				++numbers_count;
			}
			else if (ch < '0' || ch > '9')
			{
				throw Asn1Exception(errors::INVALID_OBJECT_IDENTIFIER_VALUE);
			}
		}

		object_identifier.numbers.reserve(numbers_count);
		auto position = std::string::npos;

		do
		{
			auto sub_id = oid.substr(position + 1, oid.find('.', position + 1) - position - 1);

			if (sub_id.empty())
			{
				throw Asn1Exception(errors::INVALID_OBJECT_IDENTIFIER_VALUE);
			}

			object_identifier.numbers.push_back(std::stoull(sub_id));
			position = oid.find('.', position + 1);
		} while (position != std::string::npos);

		if (!ObjectIdentifier::validate_oid_numbers(object_identifier.numbers))
		{
			throw Asn1Exception(errors::INVALID_OBJECT_IDENTIFIER_VALUE);
		}
	}

	return object_identifier;
}

std::string ObjectIdentifier::to_string() const
{
	if (!numbers.empty())
	{
		std::ostringstream out;
		std::copy(numbers.begin(), numbers.end() - 1, std::ostream_iterator<uint64_t>(out, "."));
		out << numbers.back();
		return out.str();
	}

	return std::string();
}

uint64_t ObjectIdentifier::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	if (!ObjectIdentifier::validate_oid_numbers(numbers))
	{
		throw Asn1Exception(errors::INVALID_OBJECT_IDENTIFIER_VALUE);
	}

	return encoder.encode_object_identifier(writer, tag, numbers);
}

DecodingResult ObjectIdentifierDecoder::decode(const byte_type* data, size_type size)
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
			value_.numbers.reserve(8);
			state_ = State::NUMBERS_DEFINITION;
		}
		break;
		case State::NUMBERS_DEFINITION:
		{
			if (content_length_ == 0)
			{
				THROW_IF(!ObjectIdentifier::validate_oid_numbers(value_.numbers), Asn1Exception(errors::INVALID_OBJECT_IDENTIFIER_VALUE));

				state_ = State::COMPLETED;
				decoding_result.code = DecodingCode::OK;
				on_decoding_completion(value_event_handler_, std::move(value_));
			}
			else
			{
				char byte{ 0 };

				if (value_.numbers.empty())
				{
					READ_BYTE(decoding_result, data, byte);
					--content_length_;

					auto sid1 = byte / 40;
					value_.numbers.push_back(sid1 > 2 ? 2 : sid1);
					value_.numbers.push_back(byte % 40 + (sid1 > 2 ? 40 : 0));
				}

				while (content_length_ > 0)
				{
					do
					{
						READ_BYTE(decoding_result, data, byte);
						--content_length_;

						number_ <<= 7;
						number_ |= byte & 0x7f;

					} while (byte & 0x80);

					value_.numbers.push_back(number_);
					number_ = 0;
				}
			}
		}
		break;
		default:
			throw Asn1Exception(errors::WRONG_DECODER_STATE);
		}
	}

	return decoding_result;
}