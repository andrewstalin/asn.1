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

#include "asn1/encoders/encoder.h"
#include "asn1/value.h"
#include "asn1/basic_types/octet_string.h"
#include "asn1/basic_types/bit_string.h"
#include "asn1/error.h"
#include <cassert>

using namespace asn1;

length_type IEncoder::encode_tag(io::IAsn1Writer& writer, const Tag& tag) const
{
	char first_byte{ 0 };
	first_byte |= static_cast<int>(tag.tag_class) << 6;

	if (tag.encoding_form == EncodingForm::CONSTRUCTED)
	{
		first_byte |= 0x20;
	}

	auto identifier = tag.identifier;
	uint8_t bytes_count{ 0 };

	if (identifier <= 30)
	{
		// short form
		first_byte |= identifier & 0x1f;
		writer.put(first_byte);
		++bytes_count;
	}
	else
	{
		// long form
		first_byte |= 0x1f;
		writer.put(first_byte);
		auto id = identifier;
		++bytes_count;

		while (id)
		{
			id = id >> 7;
			++bytes_count;
		}

		for (auto i = bytes_count - 2; i > 0; --i)
		{
			writer.put(((identifier >> (7 * i)) & 0x7f) | 0x80);
		}

		writer.put(identifier & 0x7f);
	}

	return bytes_count;
}

length_type IEncoder::encode_boolean(io::IAsn1Writer& writer, const Tag& tag, bool value) const
{
	auto bytes_count = encode_header(writer, tag, 1);
	writer.put(value ? 0xFF : 0x00);
	return ++bytes_count;
}

length_type IEncoder::encode_null(io::IAsn1Writer& writer, const Tag& tag) const
{
	return encode_header(writer, tag, 0);
}

length_type IEncoder::encode_integer(io::IAsn1Writer& writer, const Tag& tag, const std::vector<byte_type>& bytes) const
{
	assert(!bytes.empty());
	auto bytes_count = encode_header(writer, tag, bytes.size());
	writer.write(bytes.data(), bytes.size());
	//auto index = bytes.size() - 1;

	//while (index > 0)
	//{
	//	writer.put(bytes[index--]);
	//}

	//writer.put(bytes[0]);
	return bytes_count + bytes.size();
}

length_type IEncoder::encode_object_identifier(io::IAsn1Writer& writer, const Tag& tag, const std::vector<uint64_t>& numbers) const
{
	auto size = numbers.size();
	THROW_IF(size < 2, Asn1Exception(errors::INVALID_OBJECT_IDENTIFIER_VALUE));

	auto sub_id1 = numbers[0];
	auto sub_id2 = numbers[1];

	THROW_IF(sub_id1 < 0 || sub_id1 > 2, Asn1Exception(errors::INVALID_OBJECT_IDENTIFIER_VALUE));
	THROW_IF(sub_id1 < 2 && sub_id2 > 39, Asn1Exception(errors::INVALID_OBJECT_IDENTIFIER_VALUE));

	size_t length{ 1 };

	for (size_t i = 2; i < size; ++i)
	{
		uint64_t number{ numbers[i] };
		++length;

		while (number >>= 7)
		{
			++length;
		}
	}

	std::vector<byte_type> bytes;
	bytes.reserve(length);

	// processing first byte
	bytes.push_back(static_cast<byte_type>(sub_id1 * 40 + sub_id2));

	for (size_t i = 2; i < size; ++i)
	{
		// output bytes (7 bits) of the number, in reverse order
		std::vector<byte_type> buffer;
		buffer.reserve(10); // 70 bits for long long value
		uint64_t number{ numbers[i] };

		do
		{
			buffer.push_back(number & 127);
			number = number >> 7;
		} while (number > 0);

		for (auto j = buffer.size() - 1; j > 0; --j)
		{
			bytes.push_back((buffer[j] | 0x80));
		}

		// last byte has sign but
		bytes.push_back(buffer[0]);
	}

	auto bytes_count = encode_header(writer, tag, length);
	writer.write(bytes.data(), length);
	return bytes_count + length;
}

length_type IEncoder::encode_octet_string(io::IAsn1Writer& writer, const Tag& tag, const byte_type* data, size_type size) const
{
	StreamingOctetString octet_string(tag, size);
	auto count = octet_string.encode(*this, data, size, writer);
	count += octet_string.encode_final(*this, writer);
	return count;
}

length_type IEncoder::encode_bit_string(io::IAsn1Writer& writer, const Tag& tag, uint8_t unused_bits, const byte_type* data, size_type size) const
{
	StreamingBitString bit_string(tag, size);
	bit_string.set_unused_bits(unused_bits);

	auto count = bit_string.encode(*this, data, size, writer);
	count += bit_string.encode_final(*this, writer);
	return count;
}

length_type IEncoder::encode_collection(io::IAsn1Writer& writer, const Tag& tag, const std::vector<EncodedValue>& elements) const
{
	io::NullWriter null_writer;
	length_type content_length{ 0 };

	for (const auto& element : elements)
	{
		length_type value_size = 0;

		if (element.value != nullptr)
		{
			value_size = element.value->encode(*this, null_writer);
		}
		else if (element.default_value != nullptr)
		{
			value_size = element.default_value->encode(*this, null_writer);
		}
		else
		{
			throw std::invalid_argument("Element for encode is absent");
		}

		if (element.explicit_tag != UNDEFINED_TAG)
		{
			content_length += encode_header(null_writer, element.explicit_tag, value_size);
		}

		content_length += value_size;
	}

	auto bytes_count = encode_header(writer, tag, content_length);
	bytes_count += content_length;

	for (const auto& element : elements)
	{
		if (element.explicit_tag != UNDEFINED_TAG)
		{
			auto count = element.value->encode(*this, null_writer);
			encode_header(writer, element.explicit_tag, count);
		}

		if (element.value)
		{
			element.value->encode(*this, writer);
		}
		else if (element.default_value)
		{
			element.default_value->encode(*this, writer);
		}
		else
		{
			throw std::invalid_argument("Element for encode is absent");
		}		
	}

	return bytes_count;
}
