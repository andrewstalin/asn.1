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

#ifndef _ASN1_GOST3410_KEY_TRANSPORT_H_
#define _ASN1_GOST3410_KEY_TRANSPORT_H_

#include "asn1/gost/gost3410_transport_parameters.h"
#include "asn1/gost/gost28147_89_encrypted_key.h"

namespace asn1
{
	namespace gost
	{
		/*
			SEQUENCE {
				sessionEncryptedKey   Gost28147-89-EncryptedKey,
				transportParameters [0] IMPLICIT GostR3410-TransportParameters OPTIONAL
			}
		*/
		struct Gost3410KeyTransport : Asn1Value
		{
			Gost28147_89EncryptedKey session_encrypted_key;
			optional::OptionalValue<Gost3410TransportParameters> transport_parameters;

			Gost3410KeyTransport()
				: Gost3410KeyTransport(SEQUENCE_TAG)
			{}

			Gost3410KeyTransport(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		class Gost3410KeyTransportDecoder : public details::SequenceBasedTypeDecoder<Gost3410KeyTransport>
		{
		private:
			Gost28147_89EncryptedKeyDecoder session_encrypted_key_decoder_;
			Gost3410TransportParametersDecoder transport_parameters_decoder_;
	
		public:
			explicit Gost3410KeyTransportDecoder(IValueEventHandler* const event_handler)
				: Gost3410KeyTransportDecoder(SEQUENCE_TAG, event_handler)
			{}

			Gost3410KeyTransportDecoder(const Tag& tag, IValueEventHandler* const event_handler);

		protected:
			void on_decode_element(Asn1Value&& val) override;
			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};
	}
}

#endif