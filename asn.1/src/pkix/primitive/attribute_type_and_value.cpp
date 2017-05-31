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

#include "asn1/pkix/primitive/attribute_type_and_value.h"
#include <cassert>

using namespace asn1;

uint64_t pkix::AttributeTypeAndValue::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	return encoder.encode_collection(writer, tag,
	{
		EncodedValue(&type),
		EncodedValue(&value)
	});
}

pkix::AttributeTypeAndValueDecoder::AttributeTypeAndValueDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<AttributeTypeAndValue>(tag, event_handler)
	, type_decoder(&decoded_element_event_handler_)
	, value_decoder_(&decoded_element_event_handler_)
{
	add({
		&type_decoder,
		&value_decoder_
	});
}

void pkix::AttributeTypeAndValueDecoder::on_decode_element(Asn1Value&& val)
{
	assert(state_ != State::FINAL);

	switch (state_)
	{
	case State::TYPE_DECODING:
		decoded_value_.type = static_cast<ObjectIdentifier&&>(val);
		state_ = State::VALUE_DECODING;
		break;
	case State::VALUE_DECODING:
		decoded_value_.value = static_cast<Any&&>(val);
		state_ = State::FINAL;
		break;
	}
}
