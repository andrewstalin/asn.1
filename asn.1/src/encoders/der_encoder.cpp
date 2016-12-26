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
#include "convert.h"
#include <cassert>

using namespace asn1;

constexpr uint16_t _365[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
constexpr uint16_t _366[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

struct Date
{
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;

	Date(uint16_t year, uint8_t month, uint8_t day)
		: Date(year, month, day, 0, 0)
	{}

	Date(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute)
		: year(year), month(month), day(day), hour(hour), minute(minute)
	{}
};

bool IsLeapYear(uint16_t year)
{
	if ((year % 4) != 0)
	{
		return false;
	}

	if ((year % 100) == 0)
	{
		return ((year % 400) == 0);
	}

	return true;
}

uint64_t DateToDays(const Date& date)
{
	auto days = 365 * date.year + date.year / 4 - date.year / 100 + date.year / 400 + date.day;

	if (date.month > 0)
	{
		days += IsLeapYear(date.month) ? _366[date.month - 1] : _365[date.month - 1];
	}

	return days;
}

uint64_t DateToMinutes(const Date& date)
{
	return DateToDays(date) * 1440 + date.hour * 60 + date.minute;
}

Date MinutesToDate(uint64_t minutes)
{
	uint64_t days = minutes / 1440;
	uint8_t day{ 0 };
	uint8_t month{ 0 };
	auto year = static_cast<uint16_t>(days / 365.2625);
	bool is_leap_year = IsLeapYear(year);

	days -= DateToDays({ year, 0, 0 });
	assert(days < 365);

	for (auto i = 1; i <= 12; ++i)
	{
		uint16_t d = is_leap_year ? _366[i] : _365[i];

		if (d > days)
		{
			month = i;
			--i;
			day = static_cast<uint8_t>(days - (is_leap_year ? _366[i] : _365[i]));
			break;
		}
	}

	assert(year != 0 && month != 0 && day != 0);

	auto hour = static_cast<uint8_t>((minutes % 1440) / 60);
	auto minute = static_cast<uint8_t>((minutes % 1440) % 60);
	return{ year, month, day, hour, minute };
}

//???????
length_type DerEncoder::encode_header(io::IAsn1Writer& writer, const Tag& tag, length_type content_length) const
{
	THROW_IF(content_length == INDEFINITE_CONTENT_LENGTH, Asn1Exception(errors::WRONG_CONTENT_LENGTH_FOR_DER_ENCODER));

	auto bytes_count = encode_tag(writer, tag);

	if (content_length <= 127)
	{
		// short form
		writer.put(static_cast<byte_type>(content_length));
		++bytes_count;
	}
	else
	{
		// long form
		auto bytes = asn1::int_to_bytes(content_length);
		writer.put(static_cast<byte_type>(bytes.size() | 0x80));
		++bytes_count;

		for (auto it = bytes.rbegin(); it != bytes.rend(); ++it)
		{
			writer.put(*it);
			++bytes_count;
		}
	}

	return bytes_count;
}

length_type DerEncoder::encode_utc_time(io::IAsn1Writer& writer, const Tag& tag, const Time& time) const
{
	std::vector<byte_type> bytes;
	bytes.reserve(time.second > 0 ? 15 : 13);
	Date date{ time.year, time.month, time.day, time.hour, time.minute };

	if (time.offset != 0)
	{
		date.year += date.year > 50 ? 1000 : 2000;
		auto total_minute = DateToMinutes(date) + time.offset;
		date = MinutesToDate(total_minute);
		date.year -= date.year > 2000 ? 2000 : 1000;
	}

	bytes.push_back(((date.year % 100) / 10) + '0');
	bytes.push_back((date.year % 10) + '0');

	bytes.push_back((date.month / 10) + '0');
	bytes.push_back((date.month % 10) + '0');

	bytes.push_back((date.day / 10) + '0');
	bytes.push_back((date.day % 10) + '0');

	bytes.push_back((date.hour / 10) + '0');
	bytes.push_back((date.hour % 10) + '0');

	bytes.push_back((date.minute / 10) + '0');
	bytes.push_back((date.minute % 10) + '0');

	bytes.push_back((time.second / 10) + '0');
	bytes.push_back((time.second % 10) + '0');

	bytes.push_back('Z');

	auto bytes_count = static_cast<length_type>(bytes.size());
	bytes_count += encode_header(writer, tag, bytes_count);
	writer.write(bytes.data(), bytes.size());
	return bytes_count;
}

length_type DerEncoder::encode_generalized_time(io::IAsn1Writer& writer, const Tag& tag, const Time& time) const
{
	std::vector<byte_type> bytes;
	bytes.reserve(30);

	Date date{ time.year, time.month, time.day, time.hour, time.minute };

	if (time.offset != 0)
	{
		auto total_minute = DateToMinutes(date) + time.offset;
		date = MinutesToDate(total_minute);
	}

	bytes.push_back((date.year / 1000) + '0');
	bytes.push_back(((date.year % 1000) / 100) + '0');
	bytes.push_back(((date.year % 100) / 10) + '0');
	bytes.push_back((date.year % 10) + '0');

	bytes.push_back((date.month / 10) + '0');
	bytes.push_back((date.month % 10) + '0');

	bytes.push_back((date.day / 10) + '0');
	bytes.push_back((date.day % 10) + '0');

	bytes.push_back((date.hour / 10) + '0');
	bytes.push_back((date.hour % 10) + '0');

	bytes.push_back((date.minute / 10) + '0');
	bytes.push_back((date.minute % 10) + '0');

	bytes.push_back((time.second / 10) + '0');
	bytes.push_back((time.second % 10) + '0');

	if (time.fraction > 0)
	{
		bytes.push_back('.');
		auto str = std::to_string(time.fraction);

		for (auto ch : str)
		{
			bytes.push_back(static_cast<byte_type>(ch));
		}
	}

	bytes.push_back('Z');

	auto bytes_count = static_cast<length_type>(bytes.size());
	bytes_count += encode_header(writer, tag, bytes_count);
	writer.write(bytes.data(), bytes.size());
	return bytes_count;
}