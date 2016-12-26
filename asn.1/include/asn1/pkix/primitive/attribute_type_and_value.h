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

#ifndef _ASN1_ATTRIBUTE_TYPE_AND_VALUE_H_
#define _ASN1_ATTRIBUTE_TYPE_AND_VALUE_H_

#include "asn1/basic_types/object_identifier.h"
#include "asn1/basic_types/any.h"
#include "asn1/basic_types/sequence.h"

namespace asn1
{
	namespace pkix
	{
		/*
			AttributeTypeAndValue :: = SEQUENCE 
			{
				type     OBJECT IDENTIFIER,
				value    ANY DEFINED BY AttributeType 
			}
		*/

		struct AttributeTypeAndValue : Asn1Value
		{
			ObjectIdentifier type;
			Any value;

			AttributeTypeAndValue()
				: Asn1Value(SEQUENCE_TAG)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		class AttributeTypeAndValueDecoder : public details::SequenceBasedTypeDecoder<AttributeTypeAndValue>
		{
		private:
			ObjectIdentifierDecoder type_decoder;
			AnyDecoder value_decoder_;

		public:
			explicit AttributeTypeAndValueDecoder(IValueEventHandler* const event_handler)
				: AttributeTypeAndValueDecoder(SEQUENCE_TAG, event_handler)
			{}
			
			AttributeTypeAndValueDecoder(const Tag& tag, IValueEventHandler* const event_handle);
				
		protected:
			void on_decode_element(Asn1Value&& val) override;
			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};
	}
}

#endif