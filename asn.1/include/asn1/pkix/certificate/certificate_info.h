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

#ifndef _ASN1_CERTIFICATE_INFO_H_
#define _ASN1_CERTIFICATE_INFO_H_

#include "asn1/basic_types/explicit.h"
#include "asn1/basic_types/integer.h"
#include "asn1/basic_types/bit_string.h"
#include "asn1/basic_types/sequence.h"
#include "asn1/pkix/primitive/algorithm_identifier.h"
#include "asn1/pkix/certificate/extension.h"
#include "asn1/pkix/primitive/name.h"
#include "asn1/pkix/primitive/subject_public_key_info.h"
#include "asn1/pkix/certificate/validity.h"
#include "optional/optional.h"

namespace asn1
{
	namespace pkix
	{
		namespace cert
		{
			/*
				SEQUENCE {
					version                  [0] EXPLICIT INTEGER DEFAULT v1,
					serialNumber             INTEGER, 
					signature                AlgorithmIdentifier,
					issuer                   Name,
					validity                 Validity,
					subject                  Name,
					subjectPublicKeyInfo     SubjectPublicKeyInfo,
					issuerUniqueIdentifier   [1] IMPLICIT BIT STRING OPTIONAL,

					--  if present, version must be v2 or v3
					subjectUniqueIdentifier  [2] IMPLICIT BIT STRING OPTIONAL,

					--  if present, version must be v2 or v3
					extensions               [3] EXPLICIT  Extensions OPTIONAL
				}

				Version ::= INTEGER {v1(0), v2(1), v3(2)}
			*/

			struct CertificateInfo : Asn1Value
			{
				Integer version;
				Integer serial_number;
				AlgorithmIdentifier signature;
				Name issuer;
				Validity validity;
				Name subject;
				SubjectPublicKeyInfo subject_public_key_info;
				optional::OptionalValue<BitString> issuer_unique_identifier;
				optional::OptionalValue<BitString> subject_unique_identifier;
				optional::OptionalValue<ExtensionCollection> extensions;

				CertificateInfo()
					: CertificateInfo(SEQUENCE_TAG)
				{}

				CertificateInfo(const Tag& tag)
					: Asn1Value(tag), version(0)
				{}

				length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
			};

			class CertificateInfoDecoder : public details::SequenceBasedTypeDecoder<CertificateInfo>
			{
			private:
				enum class State : uint8_t
				{
					VERSION_DECODING,
					SERIAL_NUMBER_DECODING,
					SIGNATURE_DECODING,
					ISSUER_DECODING,
					VALIDITY_DECODING,
					SUBJECT_DECODING,
					SUBJECT_PUBLIC_KEY_INFO_DECODING,
					OPTIONAL_VALUES_DECODING,
				};

				ExplicitDecoder<IntegerDecoder> version_decoder_;
				IntegerDecoder serial_number_decoder_;
				AlgorithmIdentifierDecoder signature_decoder_;
				NameDecoder issuer_decoder_;
				ValidityDecoder validity_decoder_;
				NameDecoder subject_decoder_;
				SubjectPublicKeyInfoDecoder subject_public_key_info_decoder_;
				BitStringDecoder issuer_unique_identifier_decoder_;
				BitStringDecoder subject_unique_identifier_decoder_;
				ExplicitDecoder<ExtensionCollectionDecoder> extensions_decoder_;
				State state_{ State::VERSION_DECODING };

			public:
				explicit CertificateInfoDecoder(IValueEventHandler* const event_handler)
					: CertificateInfoDecoder(SEQUENCE_TAG, event_handler)
				{}

				CertificateInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler);

				void reset_state() override
				{
					details::SequenceBasedTypeDecoder<CertificateInfo>::reset_state();
					state_ = State::VERSION_DECODING;
				}

			protected:
				void on_decode_element(Asn1Value&& val) override;
				void on_decode_element(const Tag& explicit_tag, Asn1Value&& val) override;
			};
		}
	}
}

#endif