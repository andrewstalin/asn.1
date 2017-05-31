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

#ifndef _ASN1_ATTRIBUTES_H_
#define _ASN1_ATTRIBUTES_H_

#include "asn1/basic_types/object_identifier.h"
#include "asn1/basic_types/any.h"
#include "asn1/basic_types/set.h"
#include "asn1/basic_types/sequence.h"

namespace asn1
{
	namespace pkcs7
	{
		/*
			Attribute ::= SEQUENCE 
			{
				type        OBJECT IDENTIFIER,
				values       SET OF ANY
			}
		*/

		using AttributeValueCollection = SetOf<Any>;
		using AttributeValueCollectionDecoder = SetOfDecoder<AnyDecoder, Any>;
		
		struct Attribute : Asn1Value
		{
			ObjectIdentifier type;
			AttributeValueCollection values;

			Attribute()
				: Attribute(SEQUENCE_TAG)
			{}

			Attribute(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
			{
				return encoder.encode_collection(writer, tag,
				{
					EncodedValue(&type),
					EncodedValue(&values)
				});
			}
		};

		class AttributeDecoder : public details::SequenceBasedTypeDecoder<Attribute>
		{
		private:
			ObjectIdentifierDecoder type_decoder_;
			AttributeValueCollectionDecoder values_decoder_;

		public:
			explicit AttributeDecoder(IValueEventHandler* const event_handler)
				: AttributeDecoder(SEQUENCE_TAG, event_handler)
			{}

			AttributeDecoder(const Tag& tag, IValueEventHandler* const event_handler)
				: details::SequenceBasedTypeDecoder<Attribute>(tag, event_handler)
				, type_decoder_(&decoded_element_event_handler_)
				, values_decoder_(&decoded_element_event_handler_)
			{
				add({ 
					&type_decoder_,
					&values_decoder_
				});
			}

		protected:
			void on_decode_element(Asn1Value&& val) override
			{
				if (val.tag == OBJECT_IDENTIFIER_TAG)
				{
					decoded_value_.type = static_cast<ObjectIdentifier&&>(val);
				}
				else if (val.tag == SET_TAG)
				{
					decoded_value_.values = static_cast<AttributeValueCollection&&>(val);
				}
			}

			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};

		using AttributeCollection = SetOf<Attribute>;
		using AttributeCollectionDecoder = SetOfDecoder<AttributeDecoder, Attribute>;
	}
}

#endif