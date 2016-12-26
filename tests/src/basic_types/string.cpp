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

//#include "tests.h"
//#include "asn1/io/buffered.h"
//#include "asn1/encoders/encoder.h"
//#include "asn1/decoders/string_decoder.h"
//#include "asn1/basic_types/utf8_string.h"
//#include "asn1/basic_types/ia5_string.h"
//#include "asn1/basic_types/bmp_string.h"
//#include "asn1/basic_types/printable_string.h"
//#include "asn1/basic_types/numeric_string.h"
//
//using namespace asn1;
//
//void EncodeStringTest(const std::vector<uint8_t>& expected, const IString& string)
//{
//	io::BufferedWriter writer;
//	string.encode(BerEncoder(), writer);
//	auto actual = std::vector<uint8_t>(writer.data(), writer.data() + writer.size());
//	CompareVectors(expected, actual);
//}
//
//TEST(utf8_string_case, create_test)
//{
//	auto str = L"Тест для UTF8 строки";
//	auto utf_string = UTF8String::create(str);
//	EXPECT_EQ(str, utf_string.to_string());
//}
//
//TEST(printable_string_case, create_test)
//{
//	auto str = "Test for printable '()+,-./:=?";
//	auto printable_string = PrintableString::create(str);
//	EXPECT_EQ(str, printable_string.to_string());
//	EXPECT_THROW(PrintableString::create("bad@test"), Asn1Exception);
//}
//
//TEST(numeric_string_case, create_test)
//{
//	auto str = "1234567890";
//	auto numeric_string = NumericString::create(str);
//	EXPECT_EQ(str, numeric_string.to_string());
//	EXPECT_THROW(NumericString::create("12345 67890"), Asn1Exception);
//}
//
//TEST(bmp_string_case, create_test)
//{
//	auto str = L"Тест для BMP строки";
//	auto bmp_string = BMPString::create(str);
//	EXPECT_EQ(str, bmp_string.to_string());
//}
//
//TEST(ia5_string_case, create_test)
//{
//	auto str = "Test for ia5 string";
//	auto ia5_string = IA5String::create(str);
//	EXPECT_EQ(str, ia5_string.to_string());
//	EXPECT_THROW(IA5String::create("Плохой тест"), Asn1Exception);
//}
//
//TEST(string_encode_case, utf8_string_encode_test)
//{
//	std::vector<uint8_t> expected = { 0x0C, 0x23, 0xD0, 0xA2, 0xD0, 0xB5, 0xD1, 0x81, 0xD1, 0x82, 0xD0, 0xB8, 0xD1, 0x80, 0xD0, 0xBE, 0xD0, 0xB2, 0xD0, 0xB0, 0xD0, 0xBD, 0xD0, 0xB8, 0xD0, 0xB5, 0x20, 0x55, 0x54, 0x46, 0x38, 0x53, 0x74, 0x72, 0x69, 0x6E, 0x67 };
//	auto utf8_string = UTF8String::create(L"Тестирование UTF8String");
//	EncodeStringTest(expected, utf8_string);
//}
//
//TEST(string_encode_case, printable_string_encode_test)
//{
//	std::vector<uint8_t> expected = { 0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29 };
//	auto printable_string = PrintableString::create("UC SKB Kontur (Test)");
//	EncodeStringTest(expected, printable_string);
//}
//
//TEST(string_encode_case, numeric_string_encode_test)
//{
//	std::vector<uint8_t> expected = { 0x12, 0x0A, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
//	auto numeric_string = NumericString::create("0123456789");
//	EncodeStringTest(expected, numeric_string);
//}
//
//TEST(string_encode_case, bmp_string_encode_test)
//{
//	std::vector<uint8_t> expected = { 0x1E, 0x1C, 0x04, 0x22, 0x04, 0x35, 0x04, 0x41, 0x04, 0x42, 0x00, 0x20, 0x00, 0x42, 0x00, 0x4D, 0x00, 0x50, 0x00, 0x53, 0x00, 0x74, 0x00, 0x72, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x67 };
//	auto bmp_string = BMPString::create(L"Тест BMPString");
//	EncodeStringTest(expected, bmp_string);
//}
//
//TEST(string_encode_case, ia5_string_encode_test)
//{
//	std::vector<uint8_t> expected = { 0x16, 0x0f, 0x54, 0x65, 0x73, 0x74, 0x40, 0x20, 0x49, 0x41, 0x35, 0x53, 0x74, 0x72, 0x69, 0x6E, 0x67 };
//	auto ia5_string = IA5String::create("Test@ IA5String");
//	EncodeStringTest(expected, ia5_string);
//}
//
//TEST(string_decoder_case, utf8_string_decode_test)
//{
//	std::vector<uint8_t> data = { 0x0C, 0x23, 0xD0, 0xA2, 0xD0, 0xB5, 0xD1, 0x81, 0xD1, 0x82, 0xD0, 0xB8, 0xD1, 0x80, 0xD0, 0xBE, 0xD0, 0xB2, 0xD0, 0xB0, 0xD0, 0xBD, 0xD0, 0xB8, 0xD0, 0xB5, 0x20, 0x55, 0x54, 0x46, 0x38, 0x53, 0x74, 0x72, 0x69, 0x6E, 0x67 };
//	bool fire{ false };
//
//	UTF8StringDecoder decoder([&fire](std::unique_ptr<IAsn1Value> val)
//	{
//		fire = true;
//		auto utf8_string = static_cast<UTF8String*>(val.get());
//		EXPECT_EQ(L"Тестирование UTF8String", utf8_string->to_string());
//	});
//	TestDecoder(&decoder, data);
//	EXPECT_TRUE(fire);
//}
//
//TEST(string_decoder_case, printable_string_decode_test)
//{
//	std::vector<uint8_t> data = { 0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29 };
//	bool fire{ false };
//
//	PrintableStringDecoder decoder([&fire](std::unique_ptr<IAsn1Value> val)
//	{
//		fire = true;
//		auto printable_string = static_cast<PrintableString*>(val.get());
//		EXPECT_EQ("UC SKB Kontur (Test)", printable_string->to_string());
//	});
//	TestDecoder(&decoder, data);
//	EXPECT_TRUE(fire);
//}
//
//TEST(string_decoder_case, numeric_string_decode_test)
//{
//	std::vector<uint8_t> data = { 0x12, 0x0A, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
//	bool fire{ false };
//
//	NumericStringDecoder decoder([&fire](std::unique_ptr<IAsn1Value> val)
//	{
//		fire = true;
//		auto numeric_string = static_cast<NumericString*>(val.get());
//		EXPECT_EQ("0123456789", numeric_string->to_string());
//	});
//	TestDecoder(&decoder, data);
//	EXPECT_TRUE(fire);
//}
//
//TEST(string_decoder_case, bmp_string_decode_test)
//{
//	std::vector<uint8_t> data = { 0x1E, 0x1C, 0x04, 0x22, 0x04, 0x35, 0x04, 0x41, 0x04, 0x42, 0x00, 0x20, 0x00, 0x42, 0x00, 0x4D, 0x00, 0x50, 0x00, 0x53, 0x00, 0x74, 0x00, 0x72, 0x00, 0x69, 0x00, 0x6E, 0x00, 0x67 };
//	bool fire{ false };
//
//	BMPStringDecoder decoder([&fire](std::unique_ptr<IAsn1Value> val)
//	{
//		fire = true;
//		auto bmp_string = static_cast<BMPString*>(val.get());
//		EXPECT_EQ(L"Тест BMPString", bmp_string->to_string());
//	});
//	TestDecoder(&decoder, data);
//	EXPECT_TRUE(fire);
//}
//
//TEST(string_decoder_case, ia5_string_decode_test)
//{
//	std::vector<uint8_t> data = { 0x16, 0x0f, 0x54, 0x65, 0x73, 0x74, 0x40, 0x20, 0x49, 0x41, 0x35, 0x53, 0x74, 0x72, 0x69, 0x6E, 0x67 };
//	bool fire{ false };
//
//	IA5StringDecoder decoder([&fire](std::unique_ptr<IAsn1Value> val)
//	{
//		fire = true;
//		auto ia5_string = static_cast<IA5String*>(val.get());
//		EXPECT_EQ("Test@ IA5String", ia5_string->to_string());
//	});
//	TestDecoder(&decoder, data);
//	EXPECT_TRUE(fire);
//}