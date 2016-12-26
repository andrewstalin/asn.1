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

#ifndef _ASN1_TESTS_BASIC_ENCODER_TEST_H_
#define _ASN1_TESTS_BASIC_ENCODER_TEST_H_

#include "gtest/gtest.h"
#include "asn1/value.h"
#include "asn1/type_traits.h"
#include "asn1/encoders/encoder.h"

namespace asn1
{
	namespace test
	{
		struct BasicEncoderTest
		{
			void execute(const Asn1Value& val, const std::vector<byte_type>& expected)
			{
				io::BufferedWriter writer;
				val.encode(DerEncoder(), writer);
				EXPECT_EQ(expected, writer.bytes);
			}
		};
	}
}

#endif