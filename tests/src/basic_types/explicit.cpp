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
#include "asn1/basic_types/integer.h"
#include "asn1/basic_types/explicit.h"
#include "gtest/gtest.h"

using namespace asn1;

TEST(explicit_decoder_case, decode_test)
{
	Tag explicit_tag(EncodingForm::CONSTRUCTED, TagClass::APPLICATION, 111);
	std::vector<uint8_t> bytes = { 0x7F, 0x6F, 0x06, 0x02, 0x04, 0x49, 0x96, 0x02, 0xD2 };

	test::BasicDecoderTestEventHandler event_handler([&](Asn1Value&& val)
	{
		auto integer = static_cast<Integer&>(val);
		EXPECT_EQ(1234567890, integer.to_int64());
	});
	ExplicitDecoder<IntegerDecoder> decoder(explicit_tag, &event_handler);

	test::BasicDecoderTest decoder_test(&decoder);
	decoder_test.execute(bytes);

	EXPECT_TRUE(event_handler.fire);
	EXPECT_TRUE(event_handler.explicit_tag == explicit_tag);
}


