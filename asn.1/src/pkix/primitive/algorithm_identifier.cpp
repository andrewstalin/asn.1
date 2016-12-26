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

#include "asn1/pkix/primitive/algorithm_identifier.h"
#include <cassert>

using namespace asn1;

uint64_t pkix::AlgorithmIdentifier::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	if (parameters)
	{
		return encoder.encode_collection(writer, tag,
		{
			EncodedValue(&algorithm),
			EncodedValue(parameters.ptr())
		});
	}
	else
	{
		return encoder.encode_collection(writer, tag,
		{
			EncodedValue(&algorithm)
		});
	}
}

pkix::AlgorithmIdentifierDecoder::AlgorithmIdentifierDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<AlgorithmIdentifier>(tag, event_handler)
	, algorithm_decoder_(&decoded_element_event_handler_)
	, parameters_decoder_(&decoded_element_event_handler_)
{
	reserve(2);
	add(&algorithm_decoder_);
	add_optional(&parameters_decoder_);
}

void pkix::AlgorithmIdentifierDecoder::on_decode_element(Asn1Value&& val)
{
	if (val.tag == OBJECT_IDENTIFIER_TAG)
	{
		decoded_value_.algorithm = static_cast<ObjectIdentifier&&>(val);
	}
	else
	{
		assert(!decoded_value_.algorithm.numbers.empty());
		decoded_value_.parameters = static_cast<Any&&>(val);
	}
}