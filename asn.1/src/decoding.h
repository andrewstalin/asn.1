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

#ifndef _ASN1_DECODING_H_
#define _ASN1_DECODING_H_

#include "asn1/error.h"

namespace asn1
{
	#define CHECK_RESULT(result) if (result.code != DecodingCode::OK) return result

	#define CHECK_TAG(tag1, tag2) THROW_IF(tag1 != tag2, Asn1Exception(errors::WRONG_TAG))
	
	#define READ_BYTE(result, data, byte) \
		if (result.read_bytes >= size) return DecodingResult(result.read_bytes, DecodingCode::MORE_DATA); \
		else byte = data[result.read_bytes++]

	#define CHECK_PRECONDITIONS(data, size, finished) \
		if (finished) return{ 0, DecodingCode::OK }; \
		else if (data == nullptr || size == 0) return{ 0, DecodingCode::MORE_DATA }	
}

#endif