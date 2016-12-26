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

#ifndef _ASN1_PKCS7_RECIPIENT_INFO_H_
#define _ASN1_PKCS7_RECIPIENT_INFO_H_

#include "asn1/basic_types/integer.h"
#include "asn1/basic_types/octet_string.h"
#include "asn1/pkix/primitive/algorithm_identifier.h"
#include "asn1/pkcs7/issuer_and_serial_number.h"

namespace asn1
{
	namespace pkcs7
	{
		/*
			SEQUENCE 
			{
				version INTEGER,
				issuerAndSerialNumber IssuerAndSerialNumber,
				keyEncryptionAlgorithm AlgorithmIdentifier,
				encryptedKey OCTET STRING
			}
		*/
		struct RecipientInfo : Asn1Value
		{
		public:
			Integer version;
			IssuerAndSerialNumber issuer_and_serial_number;
			pkix::AlgorithmIdentifier key_encryption_algorithm;
			OctetString encrypted_key;

			RecipientInfo()
				: RecipientInfo(SEQUENCE_TAG)
			{}

			RecipientInfo(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
			{
				return encoder.encode_collection(writer, tag,
				{
					EncodedValue(&version),
					EncodedValue(&issuer_and_serial_number),
					EncodedValue(&key_encryption_algorithm),
					EncodedValue(&encrypted_key)
				});
			}
		};

		class RecipientInfoDecoder : public details::SequenceBasedTypeDecoder<RecipientInfo>
		{
		private:
			enum class State : uint8_t
			{
				VERSION_DECODING,
				ISSUER_AND_SERIAL_NUMBER_DECODING,
				KEY_ENCRYPTION_ALGORITHM_DECODING,
				ENCRYPTED_KEY_DECODING
			};

			IntegerDecoder version_decoder_;
			IssuerAndSerialNumberDecoder issuer_and_serial_number_decoder_;
			pkix::AlgorithmIdentifierDecoder key_encryption_algorithm_decoder_;
			OctetStringDecoder encrypted_key_decoder_;
			State state_{ State::VERSION_DECODING };

		public:
			explicit RecipientInfoDecoder(IValueEventHandler* const event_handler)
				: RecipientInfoDecoder(SEQUENCE_TAG, event_handler)
			{}

			RecipientInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler)
				: details::SequenceBasedTypeDecoder<RecipientInfo>(tag, event_handler)
				, version_decoder_(&decoded_element_event_handler_)
				, issuer_and_serial_number_decoder_(&decoded_element_event_handler_)
				, key_encryption_algorithm_decoder_(&decoded_element_event_handler_)
				, encrypted_key_decoder_(&decoded_element_event_handler_)
			{
				add({
					&version_decoder_,
					&issuer_and_serial_number_decoder_,
					&key_encryption_algorithm_decoder_,
					&encrypted_key_decoder_
				});
			}

			void reset_state() override
			{
				details::SequenceBasedTypeDecoder<RecipientInfo>::reset_state();
				state_ = State::VERSION_DECODING;
			}

		protected:
			void on_decode_element(Asn1Value&& val) override
			{
				switch (state_)
				{
				case State::VERSION_DECODING:
					decoded_value_.version = static_cast<Integer&&>(val);
					state_ = State::ISSUER_AND_SERIAL_NUMBER_DECODING;
					break;
				case State::ISSUER_AND_SERIAL_NUMBER_DECODING:
					decoded_value_.issuer_and_serial_number = static_cast<IssuerAndSerialNumber&&>(val);
					state_ = State::KEY_ENCRYPTION_ALGORITHM_DECODING;
					break;
				case State::KEY_ENCRYPTION_ALGORITHM_DECODING:
					decoded_value_.key_encryption_algorithm = static_cast<pkix::AlgorithmIdentifier&&>(val);
					state_ = State::ENCRYPTED_KEY_DECODING;
					break;
				case State::ENCRYPTED_KEY_DECODING:
					decoded_value_.encrypted_key = static_cast<OctetString&&>(val);
					break;
				}
			}

			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};

		using RecipientInfoCollection = SetOf<RecipientInfo>;
		using RecipientInfoCollectionDecoder = SetOfDecoder<RecipientInfoDecoder, RecipientInfo>;
	}
}

#endif