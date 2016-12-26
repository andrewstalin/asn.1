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

#include "asn1/pkix/primitive/relative_distinguished_name.h"
#include "asn1/header.h"
#include "asn1/error.h"
#include "babel/encoding.h"
#include <sstream>
#include <list>
#include <limits> 

using namespace asn1;

struct OIDAlias
{
	const char* oid;
	const char* alias;
};

static constexpr OIDAlias OIDS_ALIAS[] = {
	{ "2.5.4.3", "CN" },
	{ "2.5.4.6", "C" },
	{ "2.5.4.7", "L" },
	{ "2.5.4.11", "OU" },
	{ "2.5.4.10", "O" },
	{ "2.5.4.8", "S" },
	{ "2.5.4.4", "SN" },
	{ "2.5.4.9", "STREET" },
	{ "2.5.4.12", "T" },
	{ "2.5.4.43", "I" },
	{ "2.5.4.42", "G" },
	{ "1.2.840.113549.1.9.1", "E" },
	{ "1.2.643.100.3", u8"СНИЛС" },
	{ "1.2.643.3.131.1.1", u8"ИНН" },
	{ "1.2.643.100.1", u8"ОГРН" }, 
	{ nullptr, nullptr }
};

std::string GetObjectIdentiferKey(const ObjectIdentifier& oid)
{
	auto oid_str = oid.to_string();
	
	for (auto i = 0u; OIDS_ALIAS[i].oid != nullptr; ++i)
	{
#ifdef _WIN32
		if (_stricmp(oid_str.c_str(), OIDS_ALIAS[i].oid) == 0)
#else
		if (strcasecmp(oid_str.c_str(), OIDS_ALIAS[i].oid) == 0)
#endif
		{
			return OIDS_ALIAS[i].alias;
		}
	}

	return oid_str;
}

std::vector<pkix::RDNItem> pkix::RelativeDistinguishedName::parts() const
{
	std::vector<RDNItem> result;
	result.reserve(values.size());
	HeaderDecoder header_decoder;

	for(const auto& val : values)
	{
		if (!val.type.numbers.empty())
		{
			auto oid_key = GetObjectIdentiferKey(val.type);

			if (!val.value.bytes.empty())
			{
				auto decoding_result = header_decoder.decode(val.value.bytes.data(), val.value.bytes.size());
				THROW_IF(decoding_result.code != DecodingCode::OK, Asn1Exception(errors::WRONG_STRUCTURE));

				auto size = val.value.bytes.size() - decoding_result.read_bytes;
				THROW_IF(size > (std::numeric_limits<size_t>::max)(), errors::DATA_SIZE_TOO_BIG);

				if (size > 0)
				{
					auto ptr = reinterpret_cast<const char*>(val.value.bytes.data()) + decoding_result.read_bytes;

					if (header_decoder.tag() == BMP_STRING_TAG)
					{
						auto enc = babel::Encoding::open("UTF-16BE", "UTF-8");

						std::string buffer(static_cast<size_t>(size), 0);
						auto buffer_ptr = const_cast<char*>(buffer.data());
						auto encoding_size = enc.encode(ptr, buffer.size(), buffer_ptr, buffer.size());

						if (size > encoding_size)
						{
							buffer.resize(encoding_size);
						}

						result.emplace_back(oid_key, buffer);
					}
					else
					{
						result.emplace_back(oid_key, std::string(ptr, ptr + size));
					}
				}

				header_decoder.reset_state();
			}
			else
			{
				result.emplace_back(oid_key, std::string());
			}
		}
	}

	return result;
}

std::string pkix::RelativeDistinguishedName::to_string() const
{
	std::stringstream ss;
	auto rdn_parts = parts();
	auto count = rdn_parts.size();

	if (!rdn_parts.empty())
	{
		for (auto i = 0u; i < count - 1; ++i)
		{
			const auto& part = rdn_parts[i];
			ss << part.key.c_str() << "=" << part.value.c_str() << " + ";
		}

		const auto& last_part = rdn_parts[count - 1];
		ss << last_part.key.c_str() << "=" << last_part.value.c_str();
	}

	return ss.str();
}
