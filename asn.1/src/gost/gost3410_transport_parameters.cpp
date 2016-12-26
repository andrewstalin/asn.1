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

#include "asn1/gost/gost3410_transport_parameters.h"

using namespace asn1;

static constexpr Tag EPHEMERAL_PUBLIC_KEY_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 0 };

length_type gost::Gost3410TransportParameters::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;
	encoded_values.reserve(3);

	encoded_values.push_back(EncodedValue(&encryption_param_set));

	if (ephemeral_public_key)
	{
		auto val = const_cast<pkix::SubjectPublicKeyInfo*>(ephemeral_public_key.ptr());
		val->tag = EPHEMERAL_PUBLIC_KEY_TAG;
		encoded_values.emplace_back(val);
	}

	encoded_values.push_back(EncodedValue(&ukm));
	return encoder.encode_collection(writer, tag, encoded_values);
}

gost::Gost3410TransportParametersDecoder::Gost3410TransportParametersDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<Gost3410TransportParameters>(tag, event_handler)
	, encryption_param_set_decoder_(&decoded_element_event_handler_)
	, ephemeral_public_key_decoder_(EPHEMERAL_PUBLIC_KEY_TAG, &decoded_element_event_handler_)
	, ukm_decoder_(&decoded_element_event_handler_)
{
	reserve(3);
	add(&encryption_param_set_decoder_);
	add_optional(&ephemeral_public_key_decoder_);
	add(&ukm_decoder_);
}

void gost::Gost3410TransportParametersDecoder::on_decode_element(Asn1Value&& val)
{
	if (val.tag == OBJECT_IDENTIFIER_TAG)
	{
		decoded_value_.encryption_param_set = static_cast<ObjectIdentifier&&>(val);
	}
	else if (val.tag == EPHEMERAL_PUBLIC_KEY_TAG)
	{
		decoded_value_.ephemeral_public_key = static_cast<pkix::SubjectPublicKeyInfo&&>(val);
	}
	else if (val.tag == OCTET_STRING_TAG)
	{
		decoded_value_.ukm = static_cast<OctetString&&>(val);
	}
}