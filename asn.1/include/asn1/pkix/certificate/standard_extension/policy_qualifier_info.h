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

#ifndef _ASN1_POLICY_QUALIFIER_INFO_H_
#define _ASN1_POLICY_QUALIFIER_INFO_H_

#include "asn1/basic_types/object_identifier.h"
#include "asn1/basic_types/any.h"
#include "asn1/basic_types/sequence.h"

namespace asn1
{
	namespace pkix
	{
		namespace cert
		{
			/*
				SEQUENCE   
				{
					policyQualifierId	OBJECT IDENTIFIER,
					qualifier	ANY DEFINED BY   policyQualifierId
				}
			*/
			struct PolicyQualifierInfo : Asn1Value
			{
				ObjectIdentifier policy_qualifier_id;
				Any qualifier;

				PolicyQualifierInfo()
					: PolicyQualifierInfo(SEQUENCE_TAG)
				{}

				PolicyQualifierInfo(const Tag& tag)
					: Asn1Value(tag)
				{}

				length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
				{
					return encoder.encode_collection(writer, tag,
					{
						EncodedValue(&policy_qualifier_id),
						EncodedValue(&qualifier)
					});
				}
			};

			class PolicyQualifierInfoDecoder : public details::SequenceBasedTypeDecoder<PolicyQualifierInfo>
			{
			private:
				enum class State : uint8_t
				{
					POLICY_QUALIFIER_ID_DECODING,
					QUALIFIER_DECODING
				};

				ObjectIdentifierDecoder policy_qualifier_id_decoder_;
				AnyDecoder qualifier_decoder_;
				State state_{ State::POLICY_QUALIFIER_ID_DECODING };

			public:
				explicit PolicyQualifierInfoDecoder(IValueEventHandler* const event_handler)
					: PolicyQualifierInfoDecoder(SEQUENCE_TAG, event_handler)
				{}

				PolicyQualifierInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler);

				void reset_state() override
				{
					details::SequenceBasedTypeDecoder<PolicyQualifierInfo>::reset_state();
					state_ = State::POLICY_QUALIFIER_ID_DECODING;
				}

			protected:
				void on_decode_element(Asn1Value&& val) override;
				void on_decode_element(const Tag&, Asn1Value&&) override {}
			};
		}
	}
}

#endif