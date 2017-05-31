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

#ifndef _ASN1_PKCS7_CONTENT_INFO_H_
#define _ASN1_PKCS7_CONTENT_INFO_H_

#include "asn1/basic_types/object_identifier.h"
#include "asn1/basic_types/explicit.h"
#include "asn1/basic_types/sequence.h"
#include "optional/optional.h"

namespace asn1
{
	namespace pkcs7
	{
		enum class PKCS7ContentType : uint8_t
		{ 
			DATA, 
			SIGNED_DATA, 
			ENVELOPED_DATA,
			SIGNED_AND_ENVELOPED_DATA,
			DIGESTED_DATA,
			ENCRYPTED_DATA,
			AUTHENTICATED_DATA,
			UNKNOWN
		};

		/*
			ContentInfo ::= SEQUENCE 
			{
				content-type OBJECT IDENTIFIER
				pkcs7-content  [0] EXPLICIT ANY OPTIONAL
			}
		*/
		template<typename content_type>
		struct ContentInfo : Asn1Value
		{
			ObjectIdentifier content_type_oid;
			optional::OptionalValue<content_type> pkcs7_content;

			ContentInfo()
				: ContentInfo(SEQUENCE_TAG)
			{}

			ContentInfo(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		template<typename content_type>
		struct StreamingContentInfo : public StreamingAsn1Value
		{
		protected:
			length_type content_length_{ INDEFINITE_CONTENT_LENGTH };

		public:
			ObjectIdentifier content_type_oid;
			optional::OptionalValue<content_type> pkcs7_content;

			StreamingContentInfo()
				: StreamingContentInfo(SEQUENCE_TAG)
			{}

			StreamingContentInfo(const Tag& tag)
				: StreamingAsn1Value(tag)
			{}

			length_type evaluate_content_size(const IEncoder& encoder) const override;

			void reset_state() override
			{
				StreamingAsn1Value::reset_state();

				if (pkcs7_content)
				{
					pkcs7_content.value().reset_state();
				}
			}

		protected:
			length_type encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer) override;
			length_type encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer) override;
			length_type encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer) override;
		};

		template<typename content_type, typename decoder_type>
		class ContentInfoDecoder : public details::SequenceBasedTypeDecoder<ContentInfo<content_type>>
		{
		private:
			ObjectIdentifierDecoder content_type_oid_decoder_;
			ExplicitDecoder<decoder_type> pkcs7_content_decoder_;

		public:
			explicit ContentInfoDecoder(IValueEventHandler* const event_handler)
				: ContentInfoDecoder(SEQUENCE_TAG, event_handler)
			{}

			ContentInfoDecoder(IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
				: ContentInfoDecoder(SEQUENCE_TAG, event_handler, data_event_handler)
			{}

			ContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler);
			ContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler);

			const ExplicitDecoder<decoder_type>& pkcs7_content_decoder() const { return pkcs7_content_decoder_; }

		protected:
			void on_decode_element(Asn1Value&& val) override
			{
				this->decoded_value_.content_type_oid = static_cast<ObjectIdentifier&&>(val);
			}
	
			void on_decode_element(const Tag& tag, Asn1Value&& val) override
			{
				this->decoded_value_.pkcs7_content = static_cast<content_type&&>(val);
			}
		};
	}
}

#endif