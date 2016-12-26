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

#include "asn1/pkix/certificate/extension.h"
#include <cassert>

using namespace asn1;

length_type pkix::cert::Extension::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;

	if (critical.value)
	{
		encoded_values.reserve(3);
		encoded_values.emplace_back(&id);
		encoded_values.emplace_back(&critical);
		encoded_values.emplace_back(&value);
	}
	else
	{
		encoded_values.reserve(2);
		encoded_values.emplace_back(&id);
		encoded_values.emplace_back(&value);
	}

	return encoder.encode_collection(writer, tag, encoded_values);
}

pkix::cert::ExtensionDecoder::ExtensionDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<Extension>(tag, event_handler)
	, extnsion_id_decoder_(&decoded_element_event_handler_)
	, critical_decoder_(&decoded_element_event_handler_)
	, extnsion_value_decoder_(&decoded_element_event_handler_)
{
	reserve(3);
	add(&extnsion_id_decoder_);
	add_optional(&critical_decoder_);
	add(&extnsion_value_decoder_);
}

void pkix::cert::ExtensionDecoder::on_decode_element(Asn1Value&& val)
{
	if (val.tag == OBJECT_IDENTIFIER_TAG)
	{
		decoded_value_.id = static_cast<ObjectIdentifier&&>(val);
	}
	else if (val.tag == BOOLEAN_TAG)
	{
		decoded_value_.critical = static_cast<Boolean&&>(val);
	}
	else if (val.tag == OCTET_STRING_TAG)
	{
		decoded_value_.value = static_cast<OctetString&&>(val);
	}
	else
	{
		assert(false && "unexpected extension property");
	}
}
