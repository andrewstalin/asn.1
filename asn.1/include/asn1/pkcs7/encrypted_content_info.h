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

#ifndef _ASN1_PKCS7_ENCRYPTED_CONTENT_INFO_H_
#define _ASN1_PKCS7_ENCRYPTED_CONTENT_INFO_H_

#include "asn1/basic_types/object_identifier.h"
#include "asn1/basic_types/octet_string.h"
#include "asn1/pkix/primitive/algorithm_identifier.h"
#include "optional/optional.h"

namespace asn1
{
	namespace pkcs7
	{
		/*
			SEQUENCE 
			{
				contentType OBJECT IDENTIFIER,
				contentEncryptionAlgorithm AlgorithmIdentifier,
				encryptedContent [0] IMPLICIT OCTET STRING OPTIONAL
			}
		*/
		struct EncryptedContentInfo : Asn1Value
		{
			ObjectIdentifier content_type;
			pkix::AlgorithmIdentifier content_encryption_algorithm;
			optional::OptionalValue<OctetString> encrypted_content;

			EncryptedContentInfo()
				: EncryptedContentInfo(SEQUENCE_TAG)
			{}

			EncryptedContentInfo(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		struct StreamingEncryptedContentInfo : public StreamingAsn1Value
		{
			ObjectIdentifier content_type;
			pkix::AlgorithmIdentifier content_encryption_algorithm;
			optional::OptionalValue<StreamingOctetString> content;

			StreamingEncryptedContentInfo()
				: StreamingEncryptedContentInfo(SEQUENCE_TAG)
			{}

			StreamingEncryptedContentInfo(const Tag& tag)
				: StreamingAsn1Value(tag)
			{}

			length_type evaluate_content_size(const IEncoder& encoder) const override;

			void reset_state() override
			{
				StreamingAsn1Value::reset_state();

				if (content)
				{
					content.value().reset_state();
				}
			}

		protected:
			length_type encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer) override;
			length_type encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer) override;
			length_type encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer) override;
		};

		class EncryptedContentInfoDecoder : public details::SequenceBasedTypeDecoder<EncryptedContentInfo>
		{
		private:
			enum class State : uint8_t
			{
				CONTENT_TYPE_DECODING,
				CONTENT_ENCRYPTION_ALGORITHM_DECODING,
				ENCRYPTED_CONTENT_DECODING,
			};
			State state_{ State::CONTENT_TYPE_DECODING };

		public:
			ObjectIdentifierDecoder content_type_decoder;
			pkix::AlgorithmIdentifierDecoder content_encryption_algorithm_decoder;
			OctetStringDecoder encrypted_content_decoder;

			explicit EncryptedContentInfoDecoder(IValueEventHandler* const event_handler)
				: EncryptedContentInfoDecoder(SEQUENCE_TAG, event_handler)
			{}

			EncryptedContentInfoDecoder(IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
				: EncryptedContentInfoDecoder(SEQUENCE_TAG, event_handler, data_event_handler)
			{}

			EncryptedContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler);
			EncryptedContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler);

			void reset_state() override
			{
				details::SequenceBasedTypeDecoder<EncryptedContentInfo>::reset_state();
				state_ = State::CONTENT_TYPE_DECODING;
			}

		protected:
			void on_decode_element(Asn1Value&& val) override;
			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};
	}
}

#endif