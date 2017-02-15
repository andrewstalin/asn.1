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

#ifndef _ASN1_ESS_CERT_ID_V2_H_
#define _ASN1_ESS_CERT_ID_V2_H_

#include "asn1/pkix/primitive/algorithm_identifier.h"
#include "asn1/basic_types/octet_string.h"
#include "asn1/pkcs7/issuer_and_serial_number.h"
#include "asn1/basic_types/sequence.h"

namespace asn1
{
	namespace pkix
	{
		namespace cert
		{
			/*
				ESSCertIDv2 ::=  SEQUENCE {
					hashAlgorithm           AlgorithmIdentifier DEFAULT {algorithm id-sha256},
					certHash                 Hash,
					issuerSerial             IssuerSerial OPTIONAL
				}
			*/

			struct ESSCertIDV2GeneralNames : Asn1Value
			{
				asn1::pkix::Name issuer;

				ESSCertIDV2GeneralNames()
					: ESSCertIDV2GeneralNames(SEQUENCE_TAG)
				{}

				ESSCertIDV2GeneralNames(const Tag& tag)
					: Asn1Value(tag)
				{}

				length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
			};

			struct ESSCertIDV2IssuerSerial : Asn1Value
			{
				ESSCertIDV2GeneralNames issuer;
				asn1::Integer serial_number;

				ESSCertIDV2IssuerSerial()
					: ESSCertIDV2IssuerSerial(SEQUENCE_TAG)
				{}

				ESSCertIDV2IssuerSerial(const Tag& tag)
					: Asn1Value(tag)
				{}

				length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
			};

			struct ESSCertIDV2 : Asn1Value
			{
				pkix::AlgorithmIdentifier hash_algorithm;
				OctetString cert_hash;
				optional::OptionalValue<ESSCertIDV2IssuerSerial> issuer_serial;

				ESSCertIDV2()
					: ESSCertIDV2(SEQUENCE_TAG)
				{}

				ESSCertIDV2(const Tag& tag)
					: Asn1Value(tag)
				{}

				length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
			};
		}
	}
}

#endif
