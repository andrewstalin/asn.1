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
#include "asn1/basic_types/integer.h"
#include "gtest/gtest.h"

using namespace asn1;

void IntegerDecodeTest(const std::vector<byte_type>& bytes, int64_t expected)
{
	test::BasicDecoderTestEventHandler event_handler([&](Asn1Value&& val)
	{
		auto integer = static_cast<Integer&>(val);
		EXPECT_EQ(expected, integer.to_int64());
	});
	IntegerDecoder decoder(&event_handler);

	test::BasicDecoderTest decoder_test(&decoder);
	decoder_test.execute(bytes);

	EXPECT_TRUE(event_handler.fire);
}

void IntegerEncodeTest(int64_t val, const std::vector<uint8_t>& expected)
{
	test::BasicEncoderTest encoder_test;
	encoder_test.execute(Integer(val), expected);
}

TEST(integer_case, positive_integer_encode_test)
{
	IntegerEncodeTest(65345634324, { 0x02, 0x05, 0x0F, 0x36, 0xE7, 0x40, 0x14 });
}

TEST(integer_case, negative_integer_encode_test)
{
	IntegerEncodeTest(-45675367564634, { 0x02, 0x06, 0xD6, 0x75, 0x5F, 0xF0, 0xFA, 0xA6 });
}

TEST(integer_decoder_case, positive_integer_decode_test)
{
	std::vector<uint8_t> data = { 0x02, 0x05, 0x0F, 0x36, 0xE7, 0x00, 0x14 };
	int64_t expected{ 65345617940 };
	IntegerDecodeTest(data, expected);
}

TEST(integer_decoder_case, negative_integer_decode_test)
{
	std::vector<uint8_t> data = { 0x02, 0x06, 0xD6, 0x75, 0xFF, 0x00, 0xFA, 0xA6 };
	int64_t expected{ -45672698938714 };
	IntegerDecodeTest(data, expected);
}