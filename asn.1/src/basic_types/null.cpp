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

#include "asn1/basic_types/null.h"
#include "decoding.h"

using namespace asn1;

DecodingResult NullDecoder::decode(const byte_type* data, size_type size)
{
	CHECK_PRECONDITIONS(data, size, finished());
	auto decoding_result = header_decoder_.decode(data, size);
	
	CHECK_RESULT(decoding_result);
	CHECK_TAG(header_decoder_.tag(), tag());

	THROW_IF(header_decoder_.content_length() != 0, Asn1Exception(errors::WRONG_STRUCTURE));

	state_ = State::COMPLETED;
	on_decoding_completion(value_event_handler_, Null(header_decoder_.tag()));
	return decoding_result;
}
