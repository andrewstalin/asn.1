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

#include "basic_decoder_test.h"
#include "basic_encoder_test.h"
#include "asn1/basic_types/null.h"
#include "gtest/gtest.h"

using namespace asn1;

TEST(null_encoder_case, encode_test)
{
	test::BasicEncoderTest encoder_test;
	encoder_test.execute(Null(), std::vector<byte_type>({ 0x05, 0x00 }));
}

TEST(null_decoder_case, decode_test)
{
	test::BasicDecoderTestEventHandler event_handler;
	NullDecoder decoder(&event_handler);

	test::BasicDecoderTest decoder_test(&decoder);
	decoder_test.execute(std::vector<byte_type>({ 0x05, 0x00 }));

	EXPECT_TRUE(event_handler.fire);
}