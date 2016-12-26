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

#include "convert.h"
#include <sstream>
#include <iomanip>
#include <climits>

std::vector<asn1::byte_type> asn1::int_to_bytes(int64_t val)
{
	if (val == 0)
	{
		return{ 0 };
	}

	std::vector<asn1::byte_type> bytes;
	bytes.reserve(sizeof(val));

	uint8_t mask{ 0XFF };
	uint8_t shift{ CHAR_BIT };

	if (val > 0)
	{
		char byte = val & mask;

		do
		{
			bytes.push_back(byte);
			val = val >> shift;
			byte = val & mask;
		} while (val > 0);
	}
	else
	{
		val = -val;
		asn1::byte_type byte{ static_cast<asn1::byte_type>(val & mask) };
		bool bit{ true };

		do
		{
			byte = ~byte;

			if (bit)
			{
				if (byte == 255)
				{
					byte = 0;
				}
				else
				{
					++byte;
					bit = false;
				}
			}

			bytes.push_back(byte);
			val = val >> shift;
			byte = val & mask;
		} while (val > 0);

		if (bytes.back() < 128)
		{
			bytes.push_back(0xFF);
		}
	}

	return bytes;
}

int64_t asn1::bytes_to_int(const std::vector<asn1::byte_type>& bytes)
{
	int64_t val{ 0 };

	if (bytes.size() > 0)
	{
		uint8_t shift{ 0 };

		if (bytes.back() & (1 << (CHAR_BIT - 1)))
		{
			bool bit{ true };

			for (auto byte : bytes)
			{
				if (bit)
				{
					if (byte == 0)
					{
						byte = 0XFF;
					}
					else
					{
						--byte;
						bit = false;
					}
				}

				byte = ~byte;
				val += static_cast<uint64_t>(byte) << shift;
				shift += CHAR_BIT;
			}

			val = -val;
		}
		else
		{
			for (const auto& byte : bytes)
			{
				val |= static_cast<uint64_t>(byte) << shift;
				shift += CHAR_BIT;
			}
		}
	}

	return val;
}

std::string asn1::to_hext_string(const std::vector<asn1::byte_type>& bytes)
{
	std::stringstream ss;

	for (const auto& byte : bytes)
	{
		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(byte);
	}

	return ss.str();
}
