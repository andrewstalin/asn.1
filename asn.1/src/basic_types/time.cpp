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

#include "asn1/basic_types/time.h"
#include "asn1/error.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace asn1;

// UtcTime: YYMMDDhhmm[ss]Z or YYMMDDhhmm[ss](+|-)hhmm
// GeneralizedTime: Local time only YYYYMMDDHH[MM[SS[.fff]]]
// GeneralizedTime: Universal time (UTC time) only YYYYMMDDHH[MM[SS[.fff]]]Z
// GeneralizedTime: Difference between local and UTC times YYYYMMDDHH[MM[SS[.fff]]]+-HHMM.

int16_t ParseOffset(const std::string& str, uint8_t& index)
{
	THROW_IF(index + 4u > str.size(), Asn1Exception(errors::WRONG_STRUCTURE));

	bool negative = str[index++] == '-';
	uint8_t ch1 = str[index++];
	uint8_t ch2 = str[index++];
	uint8_t ch3 = str[index++];
	uint8_t ch4 = str[index++];

	if (!isdigit(ch1) || !isdigit(ch2) || !isdigit(ch3) || !isdigit(ch4))
	{
		throw Asn1Exception(errors::WRONG_STRUCTURE);
	}

	int16_t offset = ((ch1 - '0') * 10 + ch2 - '0') * 60;
	offset += (ch3 - '0') * 10 + ch4 - '0';

	if (negative)
	{
		offset *= -1;
	}

	return offset;
}

uint8_t ParseValue(const std::string& str, uint8_t& index)
{
	if (index + 2u > str.size() || isdigit(str[index + 1]) == 0)
	{
		throw Asn1Exception(errors::WRONG_STRUCTURE);
	}

	return static_cast<uint8_t>((str[index++] - '0') * 10 + str[index++] - '0');
}

// Where YY is greater than 50, the year shall be interpreted as 19YY; and
// Where YY is less than or equal to 50, the year shall be interpreted as 20YY.
uint8_t ParseUtcTime(Time& time, const std::string& str)
{
	time.utc = true;

	auto base_value = std::stoull(str.substr(0, 10));
	time.year = static_cast<uint8_t>(base_value / 100000000);
	time.year += time.year > 50 ? 1000 : 2000;
	base_value %= 100000000;
	time.month = static_cast<uint8_t>(base_value / 1000000);
	base_value %= 1000000;
	time.day = static_cast<uint8_t>(base_value / 10000);
	base_value %= 10000;
	time.hour = static_cast<uint8_t>(base_value / 100);
	base_value %= 100;
	time.minute = static_cast<uint8_t>(base_value);

	uint8_t index{ 10 };

	while (index < str.size())
	{
		auto ch = str[index];

		if (isdigit(ch))
		{
			time.second = ParseValue(str, index);

			if (index == str.size())
			{
				throw Asn1Exception(errors::WRONG_STRUCTURE);
			}
		}
		else if (ch == 'z' || ch == 'Z')
		{
			++index;
			break;
		}
		else if (ch == '+' || ch == '-')
		{
			time.offset = ParseOffset(str, index);
			break;
		}
		else
		{
			throw Asn1Exception(errors::WRONG_STRUCTURE);
		}
	}

	return index;
}

uint8_t ParseGeneralizedTime(Time& time, const std::string& str)
{
	auto base_value = std::stoull(str.substr(0, 10));
	time.year = static_cast<uint16_t>(base_value / 1000000);
	base_value %= 1000000;
	time.month = static_cast<uint8_t>(base_value / 10000);
	base_value %= 10000;
	time.day = static_cast<uint8_t>(base_value / 100);
	base_value %= 100;
	time.hour = static_cast<uint8_t>(base_value);

	uint8_t index{ 10 };
	bool minute{ false };

	while (index < str.size())
	{
		auto ch = str[index];

		if (isdigit(ch))
		{
			uint8_t val = ParseValue(str, index);

			if (!minute)
			{
				time.minute = val;
				minute = true;
			}
			else
			{
				time.second = val;
			}
		}
		else if (ch == '.')
		{
			++index;
			auto start_index = index;

			while (index < str.size() && isdigit(str[index]))
			{
				++index;
			}

			if (start_index != index)
			{
				time.fraction = std::stoull(str.substr(start_index, index - start_index));
			}
		}
		else if (ch == 'z' || ch == 'Z')
		{
			++index;
			time.utc = true;
			break;
		}
		else if (ch == '+' || ch == '-')
		{
			time.offset = ParseOffset(str, index);
			break;
		}
		else
		{
			throw Asn1Exception(errors::WRONG_STRUCTURE);
		}
	}

	return index;
}

Time Time::parse(const Tag& tag, const byte_type* data, length_type size)
{
	THROW_IF(size < 10, Asn1Exception(errors::WRONG_STRUCTURE));
	THROW_IF(size >(std::numeric_limits<size_t>::max)(), Asn1Exception(errors::DATA_SIZE_TOO_BIG));

	auto time_type = Time::TimeType::UTC;

	if (tag == GENERALIZED_TIME_TAG)
	{
		time_type = Time::TimeType::GENERALIZED;
	}
	else if (tag != UTC_TIME_TAG)
	{
		throw Asn1Exception(errors::WRONG_STRUCTURE);
	}

	std::string str(reinterpret_cast<char*>(const_cast<byte_type*>(data)), static_cast<size_t>(size));
	Time time(time_type, tag);
	uint8_t index{ 0 };

	if (time_type == Time::TimeType::UTC)
	{
		index = ParseUtcTime(time, str);
	}
	else
	{
		index = ParseGeneralizedTime(time, str);
	}

	THROW_IF(index != size, Asn1Exception(errors::WRONG_STRUCTURE));
	return time;
}

std::string Time::to_string() const
{
	std::ostringstream ss;
	ss << static_cast<int16_t>(year) << '-' << std::setfill('0');
	ss << std::setw(2) << static_cast<int16_t>(month) << '-';
	ss << std::setw(2) << static_cast<int16_t>(day) << 'T';
	ss << std::setw(2) << static_cast<int16_t>(hour) << ':';
	ss << std::setw(2) << static_cast<int16_t>(minute) << ':';
	ss << std::setw(2) << static_cast<int16_t>(second);

	if (fraction > 0)
	{
		ss << "." << fraction;
	}

	if (utc && offset == 0)
	{
		ss << "Z";
	}
	else if (offset != 0)
	{
		auto offset_hour = static_cast<uint8_t>(offset / 60);
		auto offset_minute = static_cast<uint8_t>(std::abs(offset) - offset_hour * 60);
		ss << (offset > 0 ? "+" : "-") << std::setw(2) << static_cast<int16_t>(offset_hour);
		ss << std::setw(2) << static_cast<int16_t>(offset_minute);
	}

	return ss.str();
}