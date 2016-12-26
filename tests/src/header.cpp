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

#include "randomize.h"
#include "gtest/gtest.h"
#include "asn1/header.h"
#include "asn1/header.h"
#include "asn1/encoders/encoder.h"
#include "asn1/io/writer.h"

using namespace asn1;

static void TestHeader(const Header& expected_header, const std::vector<byte_type>& expected_data)
{
	io::BufferedWriter writer;
	BerEncoder encoder;
	encoder.encode_header(writer, expected_header.tag, expected_header.content_length);
	EXPECT_EQ(expected_data, writer.bytes);

	HeaderDecoder decoder;
	auto test_data = test::generate_test_data(expected_data);

	for (auto& chunk : test_data)
	{
		auto dr = decoder.decode(chunk.chunk.data(), chunk.chunk.size());
		EXPECT_EQ(chunk.chunk.size() - chunk.unused_bytes, dr.read_bytes);
		EXPECT_EQ(chunk.completed ? asn1::DecodingCode::OK : asn1::DecodingCode::MORE_DATA, dr.code);
	}

	EXPECT_TRUE(decoder.finished());

	auto actual_header = Header(decoder.tag(), decoder.content_length());
	EXPECT_TRUE(expected_header.tag == actual_header.tag);
	EXPECT_EQ(expected_header.content_length, actual_header.content_length);
}

TEST(header_case, constructed_form_test)
{
	Header header({ EncodingForm::CONSTRUCTED, TagClass::APPLICATION, 27 }, 0);
	std::vector<byte_type> data = { 0x7B, 0x00 };
	TestHeader(header, data);
}

TEST(header_case, long_identifier_test)
{
	Header header({ EncodingForm::PRIMITIVE, TagClass::APPLICATION, 313 }, 0);
	std::vector<byte_type> data = { 0x5F, 0x82, 0x39, 0x00 };
	TestHeader(header, data);
}

TEST(header_case, undefinite_length_test)
{
	Header header({ EncodingForm::CONSTRUCTED, TagClass::APPLICATION, 27 }, INDEFINITE_CONTENT_LENGTH);
	std::vector<byte_type> data = { 0x7B, 0x80 };
	TestHeader(header, data);
}

TEST(header_case, short_length_test)
{
	Header header({ EncodingForm::CONSTRUCTED, TagClass::APPLICATION, 27 }, 90);
	std::vector<byte_type> data = { 0x7B, 0x5A };
	TestHeader(header, data);
}

TEST(header_case, long_length_test)
{
	Header header({ EncodingForm::CONSTRUCTED, TagClass::APPLICATION, 27 }, 302);
	std::vector<byte_type> data = { 0x7B, 0x82, 0x01, 0x2E };
	TestHeader(header, data);
}