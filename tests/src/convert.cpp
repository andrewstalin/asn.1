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

#include "convert.h"
#include "gtest/gtest.h"

using namespace asn1;

TEST(convert_case, intToBytes_positive)
{
	EXPECT_EQ(std::vector<byte_type>({ 11 }), int_to_bytes(11));
	EXPECT_EQ(std::vector<byte_type>({ 0, 1 }), int_to_bytes(256));
	EXPECT_EQ(std::vector<byte_type>({ 78, 70, 5 }), int_to_bytes(345678));
	EXPECT_EQ(std::vector<byte_type>({ 0x4C, 00, 5 }), int_to_bytes(327756));
}

TEST(convert_case, intToBytes_negative)
{
	EXPECT_EQ(std::vector<byte_type>({ 109, 34, 255 }), int_to_bytes(-56723));
	EXPECT_EQ(std::vector<byte_type>({ 0, 255 }), int_to_bytes(-256));
	EXPECT_EQ(std::vector<byte_type>({ 0x6E, 0x00, 0xff }), int_to_bytes(-65426));
	EXPECT_EQ(std::vector<byte_type>({ 0x81, 0x80 }), int_to_bytes(-32639));
	EXPECT_EQ(std::vector<byte_type>({ 0x6D, 0xff, 0xff, 0x80 }), int_to_bytes(-2130706579));
}

TEST(convert_case, bytesToInt_positive)
{
	EXPECT_EQ(345678, bytes_to_int({ 78, 70, 5 }));
	EXPECT_EQ(7, bytes_to_int({ 7 }));
	EXPECT_EQ(327756, bytes_to_int({ 0x4C, 00, 5, 0 }));
	EXPECT_EQ(327756, bytes_to_int({ 0x4C, 00, 5 }));
}

TEST(convert_case, bytesToInt_negative)
{
	EXPECT_EQ(-56723, bytes_to_int({ 109, 34, 255 }));
	EXPECT_EQ(-32639, bytes_to_int({ 129, 128 }));
	EXPECT_EQ(-5, bytes_to_int({ 251, 255 }));
	EXPECT_EQ(-56723, bytes_to_int({ 0x6D, 0x22, 0xff, 0xff }));
	EXPECT_EQ(-2130706579, bytes_to_int({ 0x6D, 0xff, 0xff, 0x80 }));
	EXPECT_EQ(-65426, bytes_to_int({ 0x6E, 0x00, 0xff }));
}
