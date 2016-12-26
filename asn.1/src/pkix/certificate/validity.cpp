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

#include "asn1/pkix/certificate/validity.h"

using namespace asn1;

pkix::cert::ValidityDecoder::ValidityDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<Validity>(tag, event_handler)
	, not_before_decoder_(&decoded_element_event_handler_)
	, not_after_decoder_(&decoded_element_event_handler_)
{
	add({ 
		reinterpret_cast<BerDecoder*>(&not_before_decoder_),
		reinterpret_cast<BerDecoder*>(&not_after_decoder_)
	});
}

void pkix::cert::ValidityDecoder::on_decode_element(Asn1Value&& val)
{
	if (state_ == State::NOT_BEFORE_DECODING)
	{
		decoded_value_.not_before = static_cast<Time&&>(val);
		state_ = State::NOT_AFTER_DECODING;
	}
	else
	{
		decoded_value_.not_after = static_cast<Time&&>(val);
	}
}