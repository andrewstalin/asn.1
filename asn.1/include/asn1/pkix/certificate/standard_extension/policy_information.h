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

#ifndef _ASN1_POLICY_INFORMATION_H_
#define _ASN1_POLICY_INFORMATION_H_

#include "asn1/basic_types/object_identifier.h"
#include "asn1/pkix/certificate/standard_extension/policy_qualifier_info.h"
#include "asn1/basic_types/sequence.h"
#include "optional/optional.h"

namespace asn1
{
	namespace pkix
	{
		namespace cert
		{
			/*
				SEQUENCE {
					policyIdentifier	CertPolicyId,
					policyQualifiers	SEQUENCE SIZE (1..MAX) OF   PolicyQualifierInfo   OPTIONAL
				}
			*/
			struct PolicyInformation : Asn1Value
			{
				ObjectIdentifier policy_id;
				optional::OptionalValue<SequenceOf<PolicyQualifierInfo>> policy_qualifiers;

				PolicyInformation()
					: PolicyInformation(SEQUENCE_TAG)
				{}

				PolicyInformation(const Tag& tag)
					: Asn1Value(tag)
				{}

				length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
			};

			class PolicyInformationDecoder : public details::SequenceBasedTypeDecoder<PolicyInformation>
			{
			private:
				ObjectIdentifierDecoder policy_id_decoder_;
				SequenceOfDecoder<PolicyQualifierInfoDecoder, PolicyQualifierInfo> qpolicy_qualifiers_decoder_;

			public:
				explicit PolicyInformationDecoder(IValueEventHandler* const event_handler)
					: PolicyInformationDecoder(SEQUENCE_TAG, event_handler)
				{}

				PolicyInformationDecoder(const Tag& tag, IValueEventHandler* const event_handler);

			protected:
				void on_decode_element(Asn1Value&& val) override;
				void on_decode_element(const Tag&, Asn1Value&&) override {}
			};

			using PolicyInformationCollection = SequenceOf<PolicyInformation>;
			using PolicyInformationCollectionDecoder = SequenceOfDecoder<PolicyInformationDecoder, PolicyInformation>;
		}
	}
}

#endif