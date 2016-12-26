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

#ifndef _ASN1_KEY_USAGE_H_
#define _ASN1_KEY_USAGE_H_

#include "asn1/basic_types/bit_string.h"

namespace asn1
{
	namespace pkix
	{
		namespace cert
		{
			enum class KeyUsage : uint16_t
			{
				UNDEFINED = 0,
				DIGITAL_SIGNATURE = 1,
				CONTENT_COMMITMENT = 2,
				KEY_ENCIPHERMENT = 4,
				DATA_ENCIPHERMENT = 8,
				KEY_AGREEMENT = 16,
				KEY_CERT_SIGN = 32,
				CRL_SIGN = 64,
				ENCIPHER_ONLY = 128,
				DECIPHER_ONLY = 256
			};

			inline KeyUsage operator|(KeyUsage x, KeyUsage y)
			{
				return static_cast<KeyUsage>(static_cast<uint16_t>(x) | static_cast<uint16_t>(y));
			}

			inline bool operator&(KeyUsage x, KeyUsage y)
			{
				return (static_cast<uint16_t>(x) & static_cast<uint16_t>(y)) > 0;
			}

			KeyUsage decode_key_usage(const byte_type* data, size_type size);

			//struct KeyUsage : Asn1Value
			//{
			//	KeyUsageValue value;

			//	KeyUsage(KeyUsageValue value)
			//		: Asn1Value(BIT_STRING_TAG), value(value)
			//	{}



			//	length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
			//	{
			//		BitString bit_string;
			//		auto val = static_cast<uint16_t>(value);

			//		if (value & KeyUsageValue::DECIPHER_ONLY)
			//		{
			//			bit_string.bytes.assign({
			//				static_cast<byte_type>(val >> 8),
			//				static_cast<byte_type>(val),
			//			});
			//		}
			//		else
			//		{
			//			bit_string.bytes.push_back(static_cast<byte_type>(val));
			//		}

			//		return bit_string.encode(encoder, writer);
			//	}
			//};

			//class KeyUsageDecoder
			//{
			//public:
			//	KeyUsage decode(const byte_type* data, size_type size)
			//	{
			//		auto bit_string = asn1::decode<BitString, BitStringDecoder>(data, size);
			//		THROW_IF(bit_string.bytes.empty(), Asn1Exception(errors::WRONG_STRUCTURE));
			//		return KeyUsage(static_cast<KeyUsageValue>(bit_string.to_integral_type<uint16_t>()));
			//	}
			//};
		}
	}
}

#endif