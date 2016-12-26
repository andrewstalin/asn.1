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

#include "asn1/gost/gost3410_key_transport.h"

using namespace asn1;

static constexpr Tag TRANSPORT_PARAMETERS_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 0 };

length_type gost::Gost3410KeyTransport::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;
	encoded_values.reserve(2);

	encoded_values.push_back(EncodedValue(&session_encrypted_key));

	if (transport_parameters)
	{
		auto val = const_cast<Gost3410TransportParameters*>(transport_parameters.ptr());
		val->tag = TRANSPORT_PARAMETERS_TAG;
		encoded_values.emplace_back(val);
	}

	return encoder.encode_collection(writer, tag, encoded_values);
}

gost::Gost3410KeyTransportDecoder::Gost3410KeyTransportDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<Gost3410KeyTransport>(tag, event_handler)
	, session_encrypted_key_decoder_(&decoded_element_event_handler_)
	, transport_parameters_decoder_(TRANSPORT_PARAMETERS_TAG, &decoded_element_event_handler_)
{
	reserve(2);
	add(&session_encrypted_key_decoder_);
	add_optional(&transport_parameters_decoder_);
}

void gost::Gost3410KeyTransportDecoder::on_decode_element(Asn1Value&& val)
{
	if (val.tag == SEQUENCE_TAG)
	{
		decoded_value_.session_encrypted_key = static_cast<Gost28147_89EncryptedKey&&>(val);
	}
	else if (val.tag == TRANSPORT_PARAMETERS_TAG)
	{
		decoded_value_.transport_parameters = static_cast<Gost3410TransportParameters&&>(val);
	}
}