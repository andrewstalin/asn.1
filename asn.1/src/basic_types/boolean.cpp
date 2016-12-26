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

#include "asn1/basic_types/boolean.h"
#include "decoding.h"

using namespace asn1;

DecodingResult BooleanDecoder::decode(const byte_type* data, size_type size)
{
	CHECK_PRECONDITIONS(data, size, finished());
	DecodingResult decoding_result = { 0, DecodingCode::UNDEFINED };

	while (state_ != State::COMPLETED)
	{
		switch (state_)
		{
			case State::INITIAL:
			{
				decoding_result << header_decoder_.decode(data, size);
				CHECK_RESULT(decoding_result);
				CHECK_TAG(header_decoder_.tag(), tag());
				state_ = State::VALUE_DEFINITION;
			}
			break;
		case State::VALUE_DEFINITION:
			{
				uint8_t byte{ UINT8_MAX };
				READ_BYTE(decoding_result, data, byte);
				state_ = State::COMPLETED;
				decoding_result.code = DecodingCode::OK;
				on_decoding_completion(value_event_handler_, Boolean(header_decoder_.tag(), byte != 0));
			}
			break;
		default:
			throw Asn1Exception(errors::WRONG_DECODER_STATE);
		}
	}

	return decoding_result;
}


