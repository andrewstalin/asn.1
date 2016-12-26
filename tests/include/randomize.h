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

#ifndef _ASN1_TESTS_RANDOMIZE_H_
#define _ASN1_TESTS_RANDOMIZE_H_

#include "asn1/type_traits.h"
#include <vector>

namespace asn1
{
	namespace test
	{
		struct StepDecoding
		{
			uint8_t unused_bytes{ 0 };
			bool completed{ false };
			std::vector<asn1::byte_type> chunk;
		};

		uint8_t generate_random_number(uint8_t min_val, uint8_t max_val);
		std::vector<std::vector<asn1::byte_type>> split_data(const std::vector<asn1::byte_type>& data);
		std::vector<asn1::byte_type> generate_random_sequence();
		std::vector<StepDecoding> generate_test_data(const std::vector<asn1::byte_type>& data);
	}
}

#endif