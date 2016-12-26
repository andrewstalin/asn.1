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
#include "asn1/basic_types/time.h"
#include "asn1/basic_types/octet_string.h"
#include "asn1/basic_types/bit_string.h"
#include "convert.h"

using namespace asn1;

length_type BerEncoder::encode_header(io::IAsn1Writer& writer, const Tag& tag, length_type content_length) const
{
	auto bytes_count = encode_tag(writer, tag);

	// length octets
	if (content_length == INDEFINITE_CONTENT_LENGTH)
	{
		// indefinite form
		writer.put(0x80);
		++bytes_count;
	}
	else if (content_length <= 127)
	{
		// short form
		writer.put(static_cast<uint8_t>(content_length));
		++bytes_count;
	}
	else
	{
		// long form
		auto bytes = asn1::int_to_bytes(content_length);
		writer.put(static_cast<char>(bytes.size() | 0x80));
		++bytes_count;

		for (auto it = bytes.rbegin(); it != bytes.rend(); ++it)
		{
			writer.put(*it);
			++bytes_count;
		}
	}

	return bytes_count;
}

length_type BerEncoder::encode_utc_time(io::IAsn1Writer& writer, const Tag& tag, const Time& time) const
{
	std::vector<byte_type> bytes;
	bytes.reserve(time.offset != 0 ? 17 : 13);

	if (time.year > 99)
	{
		bytes.push_back(((time.year % 100) / 10) + '0');
		bytes.push_back((time.year % 10) + '0');
	}
	else
	{
		bytes.push_back(time.year / 10 + '0');
		bytes.push_back((time.year % 10) + '0');
	}

	bytes.push_back((time.month / 10) + '0');
	bytes.push_back((time.month % 10) + '0');

	bytes.push_back((time.day / 10) + '0');
	bytes.push_back((time.day % 10) + '0');

	bytes.push_back((time.hour / 10) + '0');
	bytes.push_back((time.hour % 10) + '0');

	bytes.push_back((time.minute / 10) + '0');
	bytes.push_back((time.minute % 10) + '0');

	if (time.second > 0)
	{
		bytes.push_back((time.second / 10) + '0');
		bytes.push_back((time.second % 10) + '0');
	}

	if (time.offset != 0)
	{
		bytes.push_back(time.offset > 0 ? '+' : '-');

		byte_type hour = abs(time.offset / 60);
		byte_type minute = abs(time.offset % 60);

		bytes.push_back((hour / 10) + '0');
		bytes.push_back((hour % 10) + '0');

		bytes.push_back((minute / 10) + '0');
		bytes.push_back((minute % 10) + '0');
	}
	else
	{
		bytes.push_back('Z');
	}

	auto bytes_count = static_cast<length_type>(bytes.size());
	bytes_count += encode_header(writer, tag, bytes_count);
	writer.write(bytes.data(), bytes.size());
	return bytes_count;
}

length_type BerEncoder::encode_generalized_time(io::IAsn1Writer& writer, const Tag& tag, const Time& time) const
{
	std::vector<byte_type> bytes;
	bytes.reserve(30);

	bytes.push_back((time.year / 1000) + '0');
	bytes.push_back(((time.year % 1000) / 100) + '0');
	bytes.push_back(((time.year % 100) / 10) + '0');
	bytes.push_back((time.year % 10) + '0');

	bytes.push_back((time.month / 10) + '0');
	bytes.push_back((time.month % 10) + '0');

	bytes.push_back((time.day / 10) + '0');
	bytes.push_back((time.day % 10) + '0');

	bytes.push_back((time.hour / 10) + '0');
	bytes.push_back((time.hour % 10) + '0');

	if (time.minute > 0 || time.second > 0)
	{
		bytes.push_back((time.minute / 10) + '0');
		bytes.push_back((time.minute % 10) + '0');
	}

	if (time.second > 0)
	{
		bytes.push_back((time.second / 10) + '0');
		bytes.push_back((time.second % 10) + '0');
	}

	if (time.fraction > 0)
	{
		bytes.push_back('.');
		auto str = std::to_string(time.fraction);

		for (auto ch : str)
		{
			bytes.push_back(static_cast<byte_type>(ch));
		}
	}

	if (time.offset != 0)
	{
		bytes.push_back(time.offset > 0 ? '+' : '-');

		byte_type hour = abs(time.offset / 60);
		byte_type minute = abs(time.offset % 60);

		bytes.push_back((hour / 10) + '0');
		bytes.push_back((hour % 10) + '0');

		bytes.push_back((minute / 10) + '0');
		bytes.push_back((minute % 10) + '0');
	}
	else if (time.utc)
	{
		bytes.push_back('Z');
	}

	auto bytes_count = static_cast<length_type>(bytes.size());
	bytes_count += encode_header(writer, tag, bytes_count);
	writer.write(bytes.data(), bytes.size());
	return bytes_count;
}

length_type BerEncoder::encode_octet_string(io::IAsn1Writer& writer, const Tag& tag, size_type block_size, const byte_type* data, size_type size) const
{
	StreamingOctetString octet_string(tag, size);
	octet_string.set_block_size(block_size);

	auto count = octet_string.encode(*this, data, size, writer);
	count += octet_string.encode_final(*this, writer);
	return count;
}

length_type BerEncoder::encode_bit_string(io::IAsn1Writer& writer, const Tag& tag, uint8_t unused_bits, size_type block_size, const byte_type* data, size_type size) const
{
	StreamingBitString bit_string(tag, size);
	bit_string.set_unused_bits(unused_bits);
	bit_string.set_block_size(block_size);

	auto count = bit_string.encode(*this, data, size, writer);
	count += bit_string.encode_final(*this, writer);
	return count;
}