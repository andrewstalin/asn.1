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
#include "asn1/basic_types/any.h"
#include "gtest/gtest.h"

using namespace asn1;

void DecodeAnyTest(const std::vector<byte_type>& bytes)
{
	test::BasicDecoderTestEventHandler event_handler([&](Asn1Value&& val)
	{
		auto any = static_cast<Any&>(val);
		EXPECT_EQ(bytes, any.bytes);
	});
	AnyDecoder decoder(&event_handler);

	test::BasicDecoderTest decoder_test(&decoder);
	decoder_test.execute(bytes);

	EXPECT_TRUE(event_handler.fire);
}

void StreamDecodeAnyTest(const std::vector<byte_type>& bytes)
{
	test::StreamDecoderTestDataHandler event_handler;
	AnyDecoder decoder(&event_handler);

	test::BasicDecoderTest decoder_test(&decoder);
	decoder_test.execute(bytes);

	EXPECT_EQ(bytes, event_handler.result);
}

TEST(any_decoder_case, decode_indefinite_length_test)
{
	DecodeAnyTest(std::vector<byte_type>({ 0x30, 0x80, 0x24, 0x80, 0x04, 0x02, 0x04, 0x0F, 0x00, 0x00, 0x00, 0x00 }));
}

TEST(any_decoder_case, decode_definite_length_test)
{
	DecodeAnyTest(std::vector<byte_type>({ 0x02, 0x06, 0xD6, 0x75, 0x5F, 0xF0, 0xFA, 0xA6 }));
}

TEST(any_decoder_case, stream_decode_indefinite_length_test)
{
	StreamDecodeAnyTest(std::vector<byte_type>({ 0x30, 0x80, 0x24, 0x80, 0x04, 0x02, 0x04, 0x0F, 0x00, 0x00, 0x00, 0x00 }));
}

TEST(any_decoder_case, stream_decode_definite_length_test)
{
	StreamDecodeAnyTest(std::vector<uint8_t>({ 0x02, 0x06, 0xD6, 0x75, 0x5F, 0xF0, 0xFA, 0xA6 }));
}
