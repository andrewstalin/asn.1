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

#ifndef _ASN1_PKCS7_ENCAPSULATED_CONTENT_INFO_H_
#define _ASN1_PKCS7_ENCAPSULATED_CONTENT_INFO_H_

#include "asn1/basic_types/object_identifier.h"
#include "asn1/basic_types/octet_string.h"
#include "asn1/basic_types/explicit.h"
#include "asn1/basic_types/sequence.h"
#include "asn1/encoders/stream_encoder.h"
#include "optional/optional.h"

namespace asn1
{
	namespace pkcs7
	{
		/*
			ContentInfo ::= SEQUENCE
			{
				content-type OBJECT IDENTIFIER
				content  [0] EXPLICIT OCTET STRING OPTIONAL
			}
		*/
		struct EncapsulatedContentInfo : Asn1Value
		{
			ObjectIdentifier content_type_oid;
			optional::OptionalValue<OctetString> content;

			EncapsulatedContentInfo()
				: EncapsulatedContentInfo(SEQUENCE_TAG)
			{}

			EncapsulatedContentInfo(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		struct StreamingEncapsulatedContentInfo : public StreamingAsn1Value
		{
		protected:
			length_type content_length_{ INDEFINITE_CONTENT_LENGTH };

		public:
			optional::OptionalValue<StreamingOctetString> content;
			ObjectIdentifier content_type_oid;

			StreamingEncapsulatedContentInfo()
				: StreamingEncapsulatedContentInfo(SEQUENCE_TAG)
			{}

			StreamingEncapsulatedContentInfo(const Tag& tag)
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

		class EncapsulatedContentInfoDecoder : public details::SequenceBasedTypeDecoder<EncapsulatedContentInfo>
		{
		private:
			enum class State
			{
				CONTENT_TYPE_OID_DECODING,
				CONTENT_DECODING
			};

			ObjectIdentifierDecoder content_type_oid_decoder_;
			ExplicitDecoder<OctetStringDecoder> content_decoder_;
			State state_{ State::CONTENT_TYPE_OID_DECODING };

		public:
			explicit EncapsulatedContentInfoDecoder(IValueEventHandler* const event_handler)
				: EncapsulatedContentInfoDecoder(SEQUENCE_TAG, event_handler)
			{}

			EncapsulatedContentInfoDecoder(IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
				: EncapsulatedContentInfoDecoder(SEQUENCE_TAG, event_handler, data_event_handler)
			{}

			EncapsulatedContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler);
			EncapsulatedContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler);

			void reset_state() override
			{
				details::SequenceBasedTypeDecoder<EncapsulatedContentInfo>::reset_state();
				state_ = State::CONTENT_TYPE_OID_DECODING;
			}

		protected:
			void on_decode_element(Asn1Value&& val) override
			{
				decoded_value_.content_type_oid = static_cast<ObjectIdentifier&&>(val);
			}

			void on_decode_element(const Tag& tag, Asn1Value&& val) override
			{
				decoded_value_.content = static_cast<OctetString&&>(val);
			}
		};
	}
}

#endif