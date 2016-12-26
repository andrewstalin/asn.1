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

#include "asn1/gost/gost28147_89_encrypted_key.h"

using namespace asn1;

static constexpr Tag MASK_KEY_TAG = { EncodingForm::PRIMITIVE, TagClass::CONTEXT_SPECIFIC, 0 };

length_type gost::Gost28147_89EncryptedKey::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;
	encoded_values.reserve(3);

	encoded_values.push_back(EncodedValue(&encrypted_key));

	if (mask_key)
	{
		auto val = const_cast<OctetString*>(mask_key.ptr());
		val->tag = MASK_KEY_TAG;
		encoded_values.emplace_back(val);
	}

	encoded_values.push_back(EncodedValue(&mac_key));
	return encoder.encode_collection(writer, tag, encoded_values);
}

gost::Gost28147_89EncryptedKeyDecoder::Gost28147_89EncryptedKeyDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<Gost28147_89EncryptedKey>(tag, event_handler)
	, encrypted_key_decoder_(&decoded_element_event_handler_)
	, mask_key_decoder_(MASK_KEY_TAG, &decoded_element_event_handler_)
	, mac_key_decoder_(&decoded_element_event_handler_)
{
	reserve(3);
	add(&encrypted_key_decoder_);
	add_optional(&mask_key_decoder_);
	add(&mac_key_decoder_);
}

void gost::Gost28147_89EncryptedKeyDecoder::on_decode_element(Asn1Value&& val)
{
	switch (state_)
	{
	case State::ENCRYPTED_KEY_DECODING:
		decoded_value_.encrypted_key = static_cast<OctetString&&>(val);
		state_ = State::MASK_KEY_DECODING;
		break;
	case State::MASK_KEY_DECODING:
		if (val.tag == MASK_KEY_TAG)
		{
			decoded_value_.mask_key = static_cast<OctetString&&>(val);
			state_ = State::MAC_KEY_DECODING;
			break;
		}
	case State::MAC_KEY_DECODING:
		decoded_value_.mac_key = static_cast<OctetString&&>(val);
		state_ = State::MAC_KEY_DECODING;
	}
}