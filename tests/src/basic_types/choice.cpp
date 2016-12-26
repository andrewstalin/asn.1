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
#include "asn1/basic_types/choice.h"
#include "asn1/basic_types/integer.h"
#include "asn1/basic_types/boolean.h"
#include "gtest/gtest.h"
#include <functional>

using namespace asn1;

void ChoiceDecodeTest(const std::vector<uint8_t>& bytes, std::function<void(Asn1Value&&)> assertion)
{
	test::BasicDecoderTestEventHandler event_handler([&](Asn1Value&& val)
	{
		assertion(std::move(val));
	});

	BooleanDecoder boolean_decoder(&event_handler);
	IntegerDecoder integer_decoder(&event_handler);
	ChoiceDecoder choice_decoder;
	choice_decoder.add({ &boolean_decoder, &integer_decoder });

	test::BasicDecoderTest decoder_test(&choice_decoder);
	decoder_test.execute(bytes);

	EXPECT_TRUE(event_handler.fire);
};

TEST(choice_decoder_case, decode_test)
{
	std::vector<uint8_t> integer_data = { 0x02, 0x06, 0xD6, 0x75, 0xFF, 0x00, 0xFA, 0xA6 };
	std::vector<uint8_t> boolean_data = { 0x01, 0x01, 0xFF };

	ChoiceDecodeTest(integer_data, [](Asn1Value&& val)
	{
		auto integer = static_cast<Integer&>(val);
		EXPECT_EQ(-45672698938714, integer.to_int64());
	});
	ChoiceDecodeTest(boolean_data, [](Asn1Value&& val)
	{
		auto boolean = static_cast<Boolean&>(val);
		EXPECT_EQ(true, boolean.value);
	});
}

