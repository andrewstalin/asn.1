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

#include "asn1/pkix/certificate/standard_extension/authority_key_identifier.h"

using namespace asn1;

static constexpr Tag KEY_IDENTIFIER_TAG(EncodingForm::PRIMITIVE, TagClass::CONTEXT_SPECIFIC, 0);
static constexpr Tag AUTHORITY_CERT_ISSUER_TAG(EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 1);
static constexpr Tag AUTHORITY_CERT_SERIAL_NUMBER_TAG(EncodingForm::PRIMITIVE, TagClass::CONTEXT_SPECIFIC, 2);

length_type pkix::cert::AuthorityKeyIdentifier::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;
	encoded_values.reserve(3);

	if (key_identifier)
	{
		auto val = const_cast<OctetString*>(key_identifier.ptr());
		val->tag = KEY_IDENTIFIER_TAG;
		encoded_values.emplace_back(val);
	}

	if (authority_cert_issuer)
	{
		auto val = const_cast<SequenceOf<Any>*>(authority_cert_issuer.ptr());
		val->tag = AUTHORITY_CERT_ISSUER_TAG;
		encoded_values.emplace_back(val);
	}

	if (authority_cert_serial_number)
	{
		auto val = const_cast<Integer*>(authority_cert_serial_number.ptr());
		val->tag = AUTHORITY_CERT_SERIAL_NUMBER_TAG;
		encoded_values.emplace_back(val);
	}

	return encoder.encode_collection(writer, tag, encoded_values);
}

pkix::cert::AuthorityKeyIdentifierDecoder::AuthorityKeyIdentifierDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<AuthorityKeyIdentifier>(tag, event_handler)
	, key_identifier_decoder_(KEY_IDENTIFIER_TAG, &decoded_element_event_handler_)
	, authority_cert_issuer_decoder_(AUTHORITY_CERT_ISSUER_TAG, &decoded_element_event_handler_)
	, authority_cert_serial_number_decoder_(AUTHORITY_CERT_SERIAL_NUMBER_TAG, &decoded_element_event_handler_)
{
	reserve(3);
	add_optional(&key_identifier_decoder_);
	add_optional(&authority_cert_issuer_decoder_);
	add_optional(&authority_cert_serial_number_decoder_);
}

void pkix::cert::AuthorityKeyIdentifierDecoder::on_decode_element(Asn1Value&& val)
{
	if (val.tag == KEY_IDENTIFIER_TAG)
	{
		decoded_value_.key_identifier = static_cast<OctetString&&>(val);
	}
	else if (val.tag == AUTHORITY_CERT_ISSUER_TAG)
	{
		decoded_value_.authority_cert_issuer = static_cast<SequenceOf<Any>&&>(val);
	}
	else if (val.tag == AUTHORITY_CERT_SERIAL_NUMBER_TAG)
	{
		decoded_value_.authority_cert_serial_number = static_cast<Integer&&>(val);
	}
}