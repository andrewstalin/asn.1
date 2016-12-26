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

#ifndef _ASN1_TBS_CERT_REVOCATION_LIST_H_
#define _ASN1_TBS_CERT_REVOCATION_LIST_H_

#include "asn1/basic_types/explicit.h"
#include "asn1/basic_types/integer.h"
#include "asn1/basic_types/time.h"
#include "asn1/basic_types/sequence.h"
#include "asn1/pkix/primitive/algorithm_identifier.h"
#include "asn1/pkix/certificate/extension.h"
#include "asn1/pkix/primitive/name.h"
#include "asn1/pkix/certificate/revoked_certificate.h"
#include "optional/optional.h"

namespace asn1
{
	namespace pkix
	{
		namespace cert
		{
			/*
				CertificateList ::= SEQUENCE 
				{
					version              Version OPTIONAL, --  if present, version must be v2
					signature            AlgorithmIdentifier,
					issuer               Name,
					thisUpdate           Time,
					nextUpdate           Time OPTIONAL,
					revokedCertificates SEQUENCE OF revokedCertificate OPTIONAL,
					crlExtensions        [0] EXPLICIT Extensions OPTIONAL
				}
			*/
			struct TBSCertificateRevocationList : Asn1Value
			{
				optional::OptionalValue<Integer> version;
				AlgorithmIdentifier signature;
				Name issuer;
				Time this_update;
				optional::OptionalValue<Time> next_update;
				optional::OptionalValue<RevokedCertificateCollection> revoked_certificates;
				optional::OptionalValue<ExtensionCollection> crl_extensions;

				TBSCertificateRevocationList()
					: TBSCertificateRevocationList(SEQUENCE_TAG)
				{}

				TBSCertificateRevocationList(const Tag& tag)
					: Asn1Value(tag)
				{}

				length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
			};

			class TBSCertificateRevocationListDecoder : public details::SequenceBasedTypeDecoder<TBSCertificateRevocationList>
			{
			private:
				enum class State
				{
					SIGNATURE_DECODING,
					ISSUER_DECODING,
					THIS_UPDATE_DECODING,
					NEXT_UPDATE_DECODING,
					OPTIONAL_VALUES_DECODING
				};

				IntegerDecoder version_decoder_;
				AlgorithmIdentifierDecoder signature_decoder_;
				NameDecoder issuer_decoder_;
				TimePairDecoder this_update_decoder_;
				TimePairDecoder next_update_decoder_;
				RevokedCertificateCollectionDecoder revoked_certificates_decoder_;
				ExplicitDecoder<ExtensionCollectionDecoder> crl_extensions_decoder_;
				State state_{ State::SIGNATURE_DECODING };

			public:
				explicit TBSCertificateRevocationListDecoder(IValueEventHandler* const event_handler)
					: TBSCertificateRevocationListDecoder(SEQUENCE_TAG, event_handler)
				{}

				TBSCertificateRevocationListDecoder(const Tag& tag, IValueEventHandler* const event_handler);

				void reset_state() override
				{
					details::SequenceBasedTypeDecoder<TBSCertificateRevocationList>::reset_state();
					state_ = State::SIGNATURE_DECODING;
				}

			protected:
				void on_decode_element(Asn1Value&& val) override;
				void on_decode_element(const Tag& tag, Asn1Value&& val) override;
			};
		}
	}
}

#endif