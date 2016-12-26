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

#ifndef _ASN1_SIGNER_INFO_H_
#define _ASN1_SIGNER_INFO_H_

#include "asn1/basic_types/integer.h"
#include "asn1/basic_types/octet_string.h"
#include "asn1/pkcs7/attribute.h"
#include "asn1/pkcs7/issuer_and_serial_number.h"
#include "asn1/pkix/primitive/algorithm_identifier.h"
#include "optional/optional.h"

namespace asn1
{
	namespace pkcs7
	{
		/*
			SignerInfo ::= SEQUENCE 
			{
				version                    INTEGER,
				issuerAndSerialNumber	   IssuerAndSerialNumber,
				digestAlgorithm            AlgorithmIdentifier,
				authenticatedAttributes    [0] IMPLICIT SET OF Attribute OPTIONAL,
				digestEncryptionAlgorithm  AlgorithmIdentifier,
				encryptedDigest            OCTET STRING,
				unauthenticatedAttributes  [1] IMPLICIT  SET OF Attribute OPTIONAL
			}
		*/
		struct SignerInfo : Asn1Value
		{
			Integer version;
			IssuerAndSerialNumber issuer_and_serial_number;
			pkix::AlgorithmIdentifier digest_algorithm;
			optional::OptionalValue<AttributeCollection> authenticated_attributes;
			pkix::AlgorithmIdentifier digest_encryption_algorithm;
			OctetString encrypted_digest;
			optional::OptionalValue<AttributeCollection> unauthenticated_attributes;

			SignerInfo()
				: SignerInfo(SEQUENCE_TAG)
			{}

			SignerInfo(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		class SignerInfoDecoder : public details::SequenceBasedTypeDecoder<SignerInfo>
		{
		private:
			enum class State
			{
				VERSION_DECODING,
				ISSUER_AND_SERIAL_NUMBER_DECODING,
				DIGEST_ALGORITHM_DECODING,
				AUTHENTICATED_ATTRIBUTES_DECODING,
				DIGEST_ENCRYPTION_ALGORITHM_DECODING,
				ENCRYPTED_DIGEST_DECODING,
				UNAUTHENTICATED_ATTRIBUTES_DECODING
			};

			IntegerDecoder version_decoder_;
			IssuerAndSerialNumberDecoder issuer_and_serial_number_decoder_;
			pkix::AlgorithmIdentifierDecoder digest_algorithm_decoder_;
			AttributeCollectionDecoder authenticated_attributes_decoder_;
			pkix::AlgorithmIdentifierDecoder digest_encryption_algorithm_decoder_;
			OctetStringDecoder encrypted_digest_decoder_;
			AttributeCollectionDecoder unauthenticated_attributes_decoder_;
			State state_{ State::VERSION_DECODING };

		public:
			explicit SignerInfoDecoder(IValueEventHandler* const event_handler)
				: SignerInfoDecoder(SEQUENCE_TAG, event_handler)
			{}

			SignerInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler);

			void reset_state() override
			{
				details::SequenceBasedTypeDecoder<SignerInfo>::reset_state();
				state_ = State::VERSION_DECODING;
			}

		protected:
			void on_decode_element(Asn1Value&& val) override;
			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};

		using SignerInfoCollection = SetOf<SignerInfo>;
	}
}

#endif