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

#ifndef _ASN1_GOST_ENCRYPTION_ALGORITHM_PARAMS_H_
#define _ASN1_GOST_ENCRYPTION_ALGORITHM_PARAMS_H_

#include "asn1/basic_types/sequence.h"
#include "asn1/basic_types/octet_string.h"
#include "asn1/basic_types/object_identifier.h"

namespace asn1
{
	namespace gost
	{
		struct EncryptionAlgorithmParams : Asn1Value
		{
			OctetString iv;
			ObjectIdentifier encryption_param_set;

			EncryptionAlgorithmParams()
				: EncryptionAlgorithmParams(SEQUENCE_TAG)
			{}

			EncryptionAlgorithmParams(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
			{
				return encoder.encode_collection(writer, tag,
				{
					EncodedValue(&iv),
					EncodedValue(&encryption_param_set)
				});
			}
		};

		class EncryptionAlgorithmParamsDecoder : public details::SequenceBasedTypeDecoder<EncryptionAlgorithmParams>
		{
		private:
			OctetStringDecoder iv_decoder_;
			ObjectIdentifierDecoder encryption_param_set_decoder_;

		public:
			explicit EncryptionAlgorithmParamsDecoder(IValueEventHandler* const event_handler)
				: EncryptionAlgorithmParamsDecoder(SEQUENCE_TAG, event_handler)
			{}

			EncryptionAlgorithmParamsDecoder(const Tag& tag, IValueEventHandler* const event_handler)
				: details::SequenceBasedTypeDecoder<EncryptionAlgorithmParams>(tag, event_handler)
				, iv_decoder_(&decoded_element_event_handler_)
				, encryption_param_set_decoder_(&decoded_element_event_handler_)
			{
				add({
					&iv_decoder_,
					&encryption_param_set_decoder_
				});
			}

		protected:
			void on_decode_element(Asn1Value&& val) override
			{
				if (val.tag == OCTET_STRING_TAG)
				{
					decoded_value_.iv = static_cast<OctetString&&>(val);
				}
				else if (val.tag == OBJECT_IDENTIFIER_TAG)
				{
					decoded_value_.encryption_param_set = static_cast<ObjectIdentifier&&>(val);
				}
			}

			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};
	}
}

#endif