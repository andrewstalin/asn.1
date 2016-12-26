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

#ifndef _ASN1_GOST28147_89_ENCRYPTED_KEY_H_
#define _ASN1_GOST28147_89_ENCRYPTED_KEY_H_

#include "asn1/basic_types/octet_string.h"
#include "asn1/basic_types/sequence.h"

namespace asn1
{
	namespace gost
	{
		/*
			SEQUENCE {
				encryptedKey         Gost28147-89-Key,
				maskKey              [0] IMPLICIT Gost28147-89-Key OPTIONAL,
				macKey               Gost28147-89-MAC
			}
		*/
		struct Gost28147_89EncryptedKey : Asn1Value
		{
			OctetString encrypted_key;
			optional::OptionalValue<OctetString> mask_key;
			OctetString mac_key;

			Gost28147_89EncryptedKey()
				: Gost28147_89EncryptedKey(SEQUENCE_TAG)
			{}

			Gost28147_89EncryptedKey(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		class Gost28147_89EncryptedKeyDecoder : public details::SequenceBasedTypeDecoder<Gost28147_89EncryptedKey>
		{
		private:
			enum class State : uint8_t
			{
				ENCRYPTED_KEY_DECODING,
				MASK_KEY_DECODING,
				MAC_KEY_DECODING
			};

			OctetStringDecoder encrypted_key_decoder_;
			OctetStringDecoder mask_key_decoder_;
			OctetStringDecoder mac_key_decoder_;
			State state_{ State::ENCRYPTED_KEY_DECODING };

		public:
			explicit Gost28147_89EncryptedKeyDecoder(IValueEventHandler* const event_handler)
				: Gost28147_89EncryptedKeyDecoder(SEQUENCE_TAG, event_handler)
			{}

			Gost28147_89EncryptedKeyDecoder(const Tag& tag, IValueEventHandler* const event_handler);

			void reset_state() override
			{
				details::SequenceBasedTypeDecoder<Gost28147_89EncryptedKey>::reset_state();
				state_ = State::ENCRYPTED_KEY_DECODING;
			}

		protected:
			void on_decode_element(Asn1Value&& val) override;
			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};
	}
}

#endif