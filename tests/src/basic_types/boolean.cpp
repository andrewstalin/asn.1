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

#include "gtest/gtest.h"
#include "asn1/basic_types/boolean.h"
#include "basic_decoder_test.h"
#include "basic_encoder_test.h"

using namespace asn1;

void BooleanEncodeTest(bool val, const std::vector<byte_type>& expected)
{
	test::BasicEncoderTest encoder_test;
	encoder_test.execute(Boolean(val), expected);
}

void BooleanDecodeTest(const std::vector<byte_type>& bytes, bool expected)
{
	test::BasicDecoderTestEventHandler event_handler([&](Asn1Value&& val)
	{
		auto boolean = static_cast<Boolean&>(val);
		EXPECT_EQ(expected, boolean.value);
	});
	BooleanDecoder decoder(&event_handler);

	test::BasicDecoderTest decoder_test(&decoder);
	decoder_test.execute(bytes);

	EXPECT_TRUE(event_handler.fire);
}

TEST(boolean_case, encode_true_test)
{
	BooleanEncodeTest(true, { 0x01, 0x01, 0xFF });
}

TEST(boolean_case, encode_false_test)
{
	BooleanEncodeTest(false, { 0x01, 0x01, 0x00 });
}

TEST(boolean_decoder_case, decode_true_test)
{
	BooleanDecodeTest({ 0x01, 0x01, 0x01 }, true);
	BooleanDecodeTest({ 0x01, 0x01, 0xFF }, true);
}

TEST(boolean_decoder_case, decode_false_test)
{
	BooleanDecodeTest({ 0x01, 0x01, 0x00 }, false);
}
