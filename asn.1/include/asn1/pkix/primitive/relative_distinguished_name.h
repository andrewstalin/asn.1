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

#ifndef _ASN1_RELATIVE_DISTINGUISHED_NAME_H_
#define _ASN1_RELATIVE_DISTINGUISHED_NAME_H_

#include "asn1/basic_types/collection.h"
#include "asn1/basic_types/set.h"
#include "asn1/pkix/primitive/attribute_type_and_value.h"

namespace asn1
{
	namespace pkix
	{
		// RDN ::= SET OF AttributeTypeAndValue

		struct RDNItem
		{
			std::string key;
			std::string value;

			RDNItem(const std::string& key, const std::string& value)
				: key(key), value(value)
			{}
		};

		struct RelativeDistinguishedName : SetOf<AttributeTypeAndValue>
		{
			std::string to_string() const;
			std::vector<RDNItem> parts() const;
		};

		using RelativeDistinguishedNameDecoder = SetOfDecoder<AttributeTypeAndValueDecoder, AttributeTypeAndValue>;
	}
}

#endif