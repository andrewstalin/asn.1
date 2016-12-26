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

#ifndef _ASN1_GOST_3410_TRANSPORT_PARAMETERS_H_
#define _ASN1_GOST_3410_TRANSPORT_PARAMETERS_H_

#include "asn1/basic_types/object_identifier.h"
#include "asn1/basic_types/octet_string.h"
#include "asn1/pkix/primitive/subject_public_key_info.h"
#include "optional/optional.h"

namespace asn1
{
	namespace gost
	{
		/*
			SEQUENCE {
				encryptionParamSet   OBJECT IDENTIFIER,
				ephemeralPublicKey   [0] IMPLICIT SubjectPublicKeyInfo OPTIONAL,
				ukm                  OCTET STRING
			}
		*/
		struct Gost3410TransportParameters : Asn1Value
		{
			ObjectIdentifier encryption_param_set;
			optional::OptionalValue<pkix::SubjectPublicKeyInfo> ephemeral_public_key;
			OctetString ukm;

			Gost3410TransportParameters()
				: Gost3410TransportParameters(SEQUENCE_TAG)
			{}

			Gost3410TransportParameters(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		class Gost3410TransportParametersDecoder : public details::SequenceBasedTypeDecoder<Gost3410TransportParameters>
		{
		private:
			ObjectIdentifierDecoder encryption_param_set_decoder_;
			pkix::SubjectPublicKeyInfoDecoder ephemeral_public_key_decoder_;
			OctetStringDecoder ukm_decoder_;

		public:
			explicit Gost3410TransportParametersDecoder(IValueEventHandler* const event_handler)
				: Gost3410TransportParametersDecoder(SEQUENCE_TAG, event_handler)
			{}

			Gost3410TransportParametersDecoder(const Tag& tag, IValueEventHandler* const event_handler);
			
		protected:
			void on_decode_element(Asn1Value&& val) override;
			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};
	}
}

#endif