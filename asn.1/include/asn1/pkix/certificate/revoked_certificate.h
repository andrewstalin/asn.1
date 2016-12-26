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

#ifndef _ASN1_REVOKED_CERTIFICATE_H_
#define _ASN1_REVOKED_CERTIFICATE_H_

#include "asn1/basic_types/integer.h"
#include "asn1/basic_types/time.h"
#include "asn1/basic_types/sequence.h"
#include "asn1/pkix/certificate/extension.h"
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
					userCertificate     CertificateSerialNumber,
					revocationDate      Time,
					crlEntryExtensions  Extensions OPTIONAL
				}
			*/
			struct RevokedCertificate : Asn1Value
			{
				Integer user_certificate;
				Time revocation_date;
				optional::OptionalValue<ExtensionCollection> crl_entry_extensions;

				RevokedCertificate()
					: RevokedCertificate(SEQUENCE_TAG)
				{}

				RevokedCertificate(const Tag& tag)
					: Asn1Value(tag)
				{}

				length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
			};

			class RevokedCertificateDecoder : public details::SequenceBasedTypeDecoder<RevokedCertificate>
			{
			private:
				IntegerDecoder user_certificate_decoder_;
				TimePairDecoder revocation_date_decoder_;
				ExtensionCollectionDecoder crl_entry_extensions_decoder_;

			public:
				explicit RevokedCertificateDecoder(IValueEventHandler* const event_handler)
					: RevokedCertificateDecoder(SEQUENCE_TAG, event_handler)
				{}

				RevokedCertificateDecoder(const Tag& tag, IValueEventHandler* const event_handler);

			protected:
				void on_decode_element(Asn1Value&& val) override;
				void on_decode_element(const Tag&, Asn1Value&&) override {}
			};

			using RevokedCertificateCollection = SequenceOf<RevokedCertificate>;
			using RevokedCertificateCollectionDecoder = SequenceOfDecoder<RevokedCertificateDecoder, RevokedCertificate>;
		}
	}
}

#endif