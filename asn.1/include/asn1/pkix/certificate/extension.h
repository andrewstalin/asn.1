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

#ifndef _ASN1_EXTENSION_H_
#define _ASN1_EXTENSION_H_

#include "asn1/basic_types/boolean.h"
#include "asn1/basic_types/object_identifier.h"
#include "asn1/basic_types/octet_string.h"
#include "asn1/basic_types/sequence.h"

namespace asn1
{
	namespace pkix
	{
		namespace cert
		{
			/*
				Extension ::= SEQUENCE 
				{
					extnId     OBJECT IDENTIFIER,
					critical   BOOLEAN DEFAULT FALSE,
					extnValue  OCTET STRING
				}
			*/
			struct Extension : Asn1Value
			{
				static constexpr const char* const KEY_USAGE_OID{ "2.5.29.15" };
				static constexpr const char* const EXTENDED_KEY_USAGE_OID{ "2.5.29.37" };
				static constexpr const char* const CRL_DISTRIBUTION_POINTS{ "2.5.29.31" };
				static constexpr const char* const POLICY_INFORMATION_OID{ "2.5.29.32" };
				static constexpr const char* const AUTHORITY_KEY_IDENTIFIER{ "2.5.29.35" };

				ObjectIdentifier id;
				OctetString value;
				Boolean critical;

				Extension()
					: Extension(SEQUENCE_TAG)
				{}

				Extension(const Tag& tag)
					: Asn1Value(tag)
				{}

				length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
			};

			class ExtensionDecoder : public details::SequenceBasedTypeDecoder<Extension>
			{
			private:
				ObjectIdentifierDecoder extnsion_id_decoder_;
				BooleanDecoder critical_decoder_;
				OctetStringDecoder extnsion_value_decoder_;

			public:
				explicit ExtensionDecoder(IValueEventHandler* const event_handler)
					: ExtensionDecoder(SEQUENCE_TAG, event_handler)
				{}

				ExtensionDecoder(const Tag& tag, IValueEventHandler* const event_handler);

			protected:
				void on_decode_element(Asn1Value&& val) override;
				void on_decode_element(const Tag&, Asn1Value&&) override {}
			};

			using ExtensionCollection = SequenceOf<Extension>;
			using ExtensionCollectionDecoder = SequenceOfDecoder<ExtensionDecoder, Extension>;
		}
	}
}

#endif
