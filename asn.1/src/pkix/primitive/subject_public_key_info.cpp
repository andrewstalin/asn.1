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

#include "asn1/pkix/primitive/subject_public_key_info.h"
#include <cassert>

using namespace asn1;

pkix::SubjectPublicKeyInfoDecoder::SubjectPublicKeyInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<SubjectPublicKeyInfo>(tag, event_handler)
	, algorithm_decoder_(&decoded_element_event_handler_)
	, subject_public_key_decoder_(&decoded_element_event_handler_)
{
	add({
		&algorithm_decoder_,
		&subject_public_key_decoder_
	});
}

void pkix::SubjectPublicKeyInfoDecoder::on_decode_element(Asn1Value&& val)
{
	if (val.tag == SEQUENCE_TAG)
	{
		decoded_value_.algorithm = static_cast<pkix::AlgorithmIdentifier&&>(val);
	}
	else if (val.tag == BIT_STRING_TAG)
	{
		decoded_value_.subject_public_key = static_cast<BitString&&>(val);
	}
}
