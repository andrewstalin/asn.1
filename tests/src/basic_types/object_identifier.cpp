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
#include "asn1/encoders/encoder.h"
#include "asn1/basic_types/object_identifier.h"
#include "asn1/error.h"
#include "gtest/gtest.h"

using namespace asn1;

void ObjectIdentifierEncodeTest(const std::vector<uint8_t>& expected,  const std::string& val)
{
	test::BasicEncoderTest encoder_test;
	encoder_test.execute(ObjectIdentifier::parse(val), expected);
}

void ObjectIdentifierDecodeTest(const std::vector<byte_type>& data, const std::string& expected)
{
	test::BasicDecoderTestEventHandler event_handler([&](Asn1Value&& val)
	{
		auto oid = static_cast<ObjectIdentifier&>(val);
		EXPECT_EQ(expected, oid.to_string());
	});
	ObjectIdentifierDecoder decoder(&event_handler);

	test::BasicDecoderTest decoder_test(&decoder);
	decoder_test.execute(data);

	EXPECT_TRUE(event_handler.fire);
}

TEST(object_identifier_case, parse_test)
{
	EXPECT_EQ("1.23.456.78", ObjectIdentifier::parse("1.23.456.78").to_string());
	EXPECT_EQ("2.789.34.0", ObjectIdentifier::parse("2.789.34.0").to_string());
}

TEST(object_identifier_case, parse_exception_test)
{
	EXPECT_THROW(ObjectIdentifier::parse("1"), Asn1Exception);
	EXPECT_THROW(ObjectIdentifier::parse("3.2"), Asn1Exception);
	EXPECT_THROW(ObjectIdentifier::parse("1.40"), Asn1Exception);
	EXPECT_THROW(ObjectIdentifier::parse("0.40"), Asn1Exception);
	EXPECT_THROW(ObjectIdentifier::parse("2.-1"), Asn1Exception);
	EXPECT_THROW(ObjectIdentifier::parse("1.2..7"), Asn1Exception);
	EXPECT_THROW(ObjectIdentifier::parse("1.2.89f.7"), Asn1Exception);
}

TEST(object_identifier_case, to_string_test)
{
	ObjectIdentifier oid;
	oid.numbers = { 1, 11, 234, 67 };
	EXPECT_EQ("1.11.234.67", oid.to_string());
}

TEST(object_identifier_case, encode_long_test)
{
	ObjectIdentifierEncodeTest({ 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x09, 0x01 }, "1.2.840.113549.1.9.1");
}

TEST(object_identifier_case, encode_short_test)
{
	ObjectIdentifierEncodeTest({ 0x06, 0x01, 0x2A }, "1.2");
}

TEST(object_identifier_decoder_case, decode_long_test)
{
	ObjectIdentifierDecodeTest({ 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x09, 0x01 }, "1.2.840.113549.1.9.1");
}

TEST(object_identifier_decoder_case, decode_short_test)
{
	ObjectIdentifierDecodeTest({ 0x06, 0x01, 0x2A }, "1.2");
}