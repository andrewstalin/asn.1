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

#include "asn1/pkix/certificate/standard_extension/policy_information.h"

using namespace asn1;

length_type pkix::cert::PolicyInformation::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;

	if (policy_qualifiers)
	{
		encoded_values.reserve(2);
		encoded_values.emplace_back(&policy_id);
		encoded_values.emplace_back(policy_qualifiers.ptr());
	}
	else
	{
		encoded_values.emplace_back(&policy_id);
	}

	return encoder.encode_collection(writer, tag, encoded_values);
}

pkix::cert::PolicyInformationDecoder::PolicyInformationDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<PolicyInformation>(tag, event_handler)
	, policy_id_decoder_(&decoded_element_event_handler_)
	, qpolicy_qualifiers_decoder_(&decoded_element_event_handler_)
{
	reserve(2);
	add(&policy_id_decoder_);
	add_optional(&qpolicy_qualifiers_decoder_);
}

void pkix::cert::PolicyInformationDecoder::on_decode_element(Asn1Value&& val)
{
	if (val.tag == OBJECT_IDENTIFIER_TAG)
	{
		decoded_value_.policy_id = static_cast<ObjectIdentifier&&>(val);
	}
	else if (val.tag == SEQUENCE_TAG)
	{
		decoded_value_.policy_qualifiers = static_cast<SequenceOf<PolicyQualifierInfo>&&>(val);
	}
}
