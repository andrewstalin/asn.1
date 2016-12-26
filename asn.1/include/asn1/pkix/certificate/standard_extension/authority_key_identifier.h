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

#ifndef _ASN1_AUTHORITY_KEY_IDENTIFIER_H_
#define _ASN1_AUTHORITY_KEY_IDENTIFIER_H_

#include "asn1/basic_types/octet_string.h"
#include "asn1/basic_types/integer.h"
#include "asn1/basic_types/any.h"
#include "asn1/basic_types/sequence.h"
#include "optional/optional.h"

namespace asn1
{
	namespace pkix
	{
		namespace cert
		{
			/*
				SEQUENCE   
				{
					keyIdentifier	[0]   KeyIdentifier   OPTIONAL,
					authorityCertIssuer	[1]   GeneralNames   OPTIONAL,
					authorityCertSerialNumber	[2]   CertificateSerialNumber   OPTIONAL
				}
			*/
			struct AuthorityKeyIdentifier : Asn1Value
			{
				optional::OptionalValue<OctetString> key_identifier;
				optional::OptionalValue<SequenceOf<Any>> authority_cert_issuer;
				optional::OptionalValue<Integer> authority_cert_serial_number;

				AuthorityKeyIdentifier()
					: AuthorityKeyIdentifier(SEQUENCE_TAG)
				{}

				AuthorityKeyIdentifier(const Tag& tag)
					: Asn1Value(tag)
				{}

				length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
			};

			class AuthorityKeyIdentifierDecoder : public details::SequenceBasedTypeDecoder<AuthorityKeyIdentifier>
			{
			private:
				OctetStringDecoder key_identifier_decoder_;
				SequenceOfDecoder<AnyDecoder, Any> authority_cert_issuer_decoder_;
				IntegerDecoder authority_cert_serial_number_decoder_;

			public:
				explicit AuthorityKeyIdentifierDecoder(IValueEventHandler* const event_handler)
					: AuthorityKeyIdentifierDecoder(SEQUENCE_TAG, event_handler)
				{}

				AuthorityKeyIdentifierDecoder(const Tag& tag, IValueEventHandler* const event_handler);

			protected:
				void on_decode_element(Asn1Value&& val) override;
				void on_decode_element(const Tag&, Asn1Value&&) override {}
			};
		}
	}
}

#endif