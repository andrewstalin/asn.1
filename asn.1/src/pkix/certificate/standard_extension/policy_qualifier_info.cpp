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

#include "asn1/pkix/certificate/standard_extension/policy_qualifier_info.h"

using namespace asn1;

pkix::cert::PolicyQualifierInfoDecoder::PolicyQualifierInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<PolicyQualifierInfo>(tag, event_handler)
	, policy_qualifier_id_decoder_(&decoded_element_event_handler_)
	, qualifier_decoder_(&decoded_element_event_handler_)
{
	add({
		&policy_qualifier_id_decoder_,
		&qualifier_decoder_
	});
}

void pkix::cert::PolicyQualifierInfoDecoder::on_decode_element(Asn1Value&& val)
{
	switch (state_)
	{
	case State::POLICY_QUALIFIER_ID_DECODING:
		decoded_value_.policy_qualifier_id = static_cast<ObjectIdentifier&&>(val);
		state_ = State::QUALIFIER_DECODING;
		break;
	case State::QUALIFIER_DECODING:
		decoded_value_.qualifier = static_cast<Any&&>(val);
		break;
	}
}