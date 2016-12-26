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

#ifndef _ASN1_PKCS7_SIGNED_DATA_H_
#define _ASN1_PKCS7_SIGNED_DATA_H_

#include "asn1/basic_types/integer.h"
#include "asn1/pkcs7/encapsulated_content_info.h"
#include "asn1/pkcs7/signer_info.h"
#include "asn1/pkix/primitive/algorithm_identifier.h"
#include "asn1/pkix/certificate/certificate_revocation_list.h"
#include "asn1/pkix/certificate/certificate.h"
#include "optional/optional.h"

namespace asn1
{
	namespace pkcs7
	{
		/*
			SignedData ::= SEQUENCE 
			{
				version           Version,
				digestAlgorithms  SET OF AlgorithmIdentifier,
				contentInfo       ContentInfo,
				certificates      [0] IMPLICIT SET OF CERTIFICATE OPTIONAL,
				crls              [1] IMPLICIT SET OF CertificateRevocationList OPTIONAL,
				signerInfos       SET OF SignerInfo
			}
		*/
		struct SignedData : Asn1Value
		{
			Integer version;
			SetOf<pkix::AlgorithmIdentifier> digest_algorithms;
			EncapsulatedContentInfo content_info;
			optional::OptionalValue<SetOf<pkix::cert::Certificate>> certificates;
			optional::OptionalValue<SetOf<pkix::cert::CertificateRevocationList>> crls;
			SetOf<SignerInfo> signer_infos;

			SignedData()
				: SignedData(SEQUENCE_TAG)
			{}

			SignedData(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		struct StreamingSignedData : public StreamingAsn1Value
		{
			Integer version;
			SetOf<pkix::AlgorithmIdentifier> digest_algorithms;
			StreamingEncapsulatedContentInfo content_info;
			optional::OptionalValue<SetOf<pkix::cert::Certificate>> certificates;
			optional::OptionalValue<SetOf<pkix::cert::CertificateRevocationList>> crls;
			SetOf<SignerInfo> signer_infos;

			StreamingSignedData()
				: StreamingSignedData(SEQUENCE_TAG)
			{}

			StreamingSignedData(const Tag& tag)
				: StreamingAsn1Value(tag)
			{}

			length_type evaluate_content_size(const IEncoder& encoder) const override;

			void reset_state() override
			{
				StreamingAsn1Value::reset_state();
				content_info.reset_state();
			}
			
		protected:
			length_type encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer) override;
			length_type encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer) override;
			length_type encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer) override;
		};

		class SignedDataDecoder : public details::SequenceBasedTypeDecoder<SignedData>
		{
		private:
			enum class State
			{
				VERSION_DECODING,
				DIGEST_ALGORITHMS_DECODING,
				CONTENT_INFO_DECODING,
				CERTIFICATES_DECODING,
				CRLS_DECODING,
				SIGNER_INFOS_DECODING,
			};

			IntegerDecoder version_decoder_;
			SetOfDecoder<pkix::AlgorithmIdentifierDecoder, pkix::AlgorithmIdentifier> digest_algorithms_decoder_;
			EncapsulatedContentInfoDecoder content_info_decoder_;
			SetOfDecoder<pkix::cert::CertificateDecoder, pkix::cert::Certificate> certificates_decoder_;
			SetOfDecoder<pkix::cert::CertificateRevocationListDecoder, pkix::cert::CertificateRevocationList> crls_decoder_;
			SetOfDecoder<SignerInfoDecoder, SignerInfo> signer_infos_decoder_;
			State state_{ State::VERSION_DECODING };

		public:
			explicit SignedDataDecoder(IValueEventHandler* const event_handler)
				: SignedDataDecoder(SEQUENCE_TAG, event_handler)
			{}

			explicit SignedDataDecoder(IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
				: SignedDataDecoder(SEQUENCE_TAG, event_handler, data_event_handler)
			{}

			SignedDataDecoder(const Tag& tag, IValueEventHandler* const event_handler);
			SignedDataDecoder(const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler);

			void reset_state() override
			{
				details::SequenceBasedTypeDecoder<SignedData>::reset_state();
				state_ = State::VERSION_DECODING;
			}

		protected:
			void on_decode_element(Asn1Value&& val) override;
			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};
	}
}

#endif