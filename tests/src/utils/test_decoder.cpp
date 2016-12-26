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
#include "basic_decoder_test.h"
#include "randomize.h"
#include "hex.h"

using namespace asn1;

void test::BasicDecoderTest::execute(const std::vector<asn1::byte_type>& bytes)
{
	if (print_debug)
	{
		std::cout << "asn.1 data: " << to_hex(bytes) << std::endl;
	}

	auto test_data = test::generate_test_data(bytes);

	if (print_debug)
	{
		for (auto& chunk : test_data)
		{
			std::cout << "unused bytes: " << std::to_string(chunk.unused_bytes) << ", chunk data: " << to_hex(chunk.chunk) << std::endl;
		}
	}

	for (const auto& chunk : test_data)
	{
		auto dr = decoder->decode(chunk.chunk.data(), chunk.chunk.size());
		EXPECT_EQ(chunk.chunk.size() - chunk.unused_bytes, dr.read_bytes);
		EXPECT_EQ(chunk.completed ? asn1::DecodingCode::OK : asn1::DecodingCode::MORE_DATA, dr.code);
	}
}