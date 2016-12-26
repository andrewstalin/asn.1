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

#ifndef _ASN1_PKCS7_ENVELOPED_DATA_H_
#define _ASN1_PKCS7_ENVELOPED_DATA_H_

#include "asn1/basic_types/integer.h"
#include "asn1/pkcs7/encrypted_content_info.h"
#include "asn1/pkcs7/recipient_info.h"
#include "asn1/basic_types/sequence.h"
#include "asn1/basic_types/set.h"

namespace asn1
{
	namespace pkcs7
	{
		/*
			EnvelopedData ::= SEQUENCE 
			{
			  version                Version,
			  recipientInfos         SET OF RecipientInfo,
			  encryptedContentInfo   EncryptedContentInfo
			}
		*/
		struct EnvelopedData : Asn1Value
		{
		public:
			Integer version;
			RecipientInfoCollection recipient_infos;
			EncryptedContentInfo encrypted_content_info;
			
			EnvelopedData()
				: EnvelopedData(SEQUENCE_TAG)
			{}

			EnvelopedData(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		struct StreamingEnvelopedData : public StreamingAsn1Value
		{
			Integer version;
			RecipientInfoCollection recipient_infos;
			StreamingEncryptedContentInfo encrypted_content_info;

			StreamingEnvelopedData()
				: StreamingEnvelopedData(SEQUENCE_TAG)
			{}

			StreamingEnvelopedData(const Tag& tag)
				: StreamingAsn1Value(tag)
			{}

			length_type evaluate_content_size(const IEncoder& encoder) const override;

			void reset_state() override
			{
				StreamingAsn1Value::reset_state();
				encrypted_content_info.reset_state();
			}

		protected:
			length_type encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer) override;
			length_type encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer) override;
			length_type encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer) override;
		};

		class EnvelopedDataDecoder : public details::SequenceBasedTypeDecoder<EnvelopedData>
		{
		private:
			enum class State
			{
				VERSION_DECODING,
				RECIPIENT_INFOS_DECODING,
				ENCRYPTED_CONTENT_INFO_DECODING
			};
			State state_{ State::VERSION_DECODING };

		public:
			IntegerDecoder version_decoder;
			RecipientInfoCollectionDecoder recipient_infos_decoder;
			EncryptedContentInfoDecoder encrypted_content_info_decoder;

			explicit EnvelopedDataDecoder(IValueEventHandler* const event_handler)
				: EnvelopedDataDecoder(SEQUENCE_TAG, event_handler)
			{}

			explicit EnvelopedDataDecoder(IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
				: EnvelopedDataDecoder(SEQUENCE_TAG, event_handler, data_event_handler)
			{}

			EnvelopedDataDecoder(const Tag& tag, IValueEventHandler* const event_handler);
			EnvelopedDataDecoder(const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler);

			void reset_state() override
			{
				details::SequenceBasedTypeDecoder<EnvelopedData>::reset_state();
				state_ = State::VERSION_DECODING;
			}

		protected:
			void on_decode_element(Asn1Value&& val) override;
			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};
	}
}

#endif