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
//#include "asn1/basic_types/ia5_string.h"
//#include "asn1/basic_types/printable_string.h"
//#include "asn1/basic_types/utf8_string.h"
//#include "asn1/basic_types/utf8_string.h"
//#include "asn1/pkix/primitive/subject_public_key_info.h"
//#include "asn1/pkix/certificate/certificate.h"
//#include "asn1/io/buffered.h"
//#include "asn1/encoders/encoder.h"
//
//using namespace asn1;
//using namespace pkix;
//
//template<typename T, typename D>
//void test_decode(const std::vector<uint8_t>& data, const T& expected)
//{
//	int success = 0;
//
//	D decoder([&success, &expected](std::unique_ptr<IAsn1Value> val)
//	{
//		++success;
//		auto actual = static_cast<T*>(val.get());
//		EXPECT_TRUE(expected.equal(*actual));
//	});
//
//	//TestDecoder(&decoder, data);
//	decoder.decode(data.data(), data.size());
//	EXPECT_EQ(1, success);
//}
//
//
//TEST(atribute_type_and_value, decode_test)
//{
//	std::vector<uint8_t> data =
//	{
//		0x30, 0x1C,
//			0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x09, 0x01,
//			0x16, 0x0F, 0x63, 0x61, 0x40, 0x73, 0x6B, 0x62, 0x6B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x2E, 0x72, 0x75
//	};
//
//	AttributeTypeAndValue expected(std::make_unique<ObjectIdentifier>(ObjectIdentifier::parse("1.2.840.113549.1.9.1")), std::make_unique<Any>());
//	expected.value->bytes = { 0x16, 0x0F, 0x63, 0x61, 0x40, 0x73, 0x6B, 0x62, 0x6B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x2E, 0x72, 0x75 };
//
//	test_decode<AttributeTypeAndValue, AttributeTypeAndValueDecoder>(data, expected);
//}
//
//TEST(relative_distinguishe_name, decode_test)
//{
//	std::vector<uint8_t> data =
//	{
//		0x31, 0x3B,
//			0x30, 0x1C,
//				0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x09, 0x01,
//				0x16, 0x0F, 0x63, 0x61, 0x40, 0x73, 0x6B, 0x62, 0x6B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x2E, 0x72, 0x75,
//			0x30, 0x1B,
//				0x06, 0x03, 0x55, 0x04, 0x03,
//				0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29
//	};
//
//	RelativeDistinguishedName expected;
//
//	auto first_attribute_and_value = std::make_unique<AttributeTypeAndValue>(std::make_unique<ObjectIdentifier>(ObjectIdentifier::parse("1.2.840.113549.1.9.1")), std::make_unique<Any>());
//	first_attribute_and_value->value->bytes = { 0x16, 0x0F, 0x63, 0x61, 0x40, 0x73, 0x6B, 0x62, 0x6B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x2E, 0x72, 0x75 };
//
//	auto second_attribute_and_value = std::make_unique<AttributeTypeAndValue>(std::make_unique<ObjectIdentifier>(ObjectIdentifier::parse("2.5.4.3")), std::make_unique<Any>());
//	second_attribute_and_value->value->bytes = { 0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29 };
//
//	expected.elements.push_back(std::move(first_attribute_and_value));
//	expected.elements.push_back(std::move(second_attribute_and_value));
//
//	test_decode<RelativeDistinguishedName, RelativeDistinguishedNameDecoder>(data, expected);
//}
////
////TEST(name_case, decode_test)
////{
////	std::vector<uint8_t> data = {
////		0x30, 0x81, 0x62,
////		0x31, 0x1E,
////			0x30, 0x1C,
////				0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x09, 0x01,
////				0x16, 0x0F, 0x63, 0x61, 0x40, 0x73, 0x6B, 0x62, 0x6B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x2E, 0x72, 0x75,
////		0x31, 0x1D,
////			0x30, 0x1B,
////				0x06,  0x03, 0x55, 0x04, 0x03,
////				0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E,  0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
////		0x31, 0x21,
////			0x30, 0x1F,
////				0x06, 0x03, 0x55, 0x04, 0x07,
////				0x0C, 0x18, 0xD0, 0x95, 0xD0, 0xBA, 0xD0, 0xB0, 0xD1, 0x82, 0xD0, 0xB5, 0xD1, 0x80, 0xD0, 0xB8, 0xD0, 0xBD, 0xD0, 0xB1, 0xD1, 0x83, 0xD1, 0x80, 0xD0, 0xB3
////	};
////
////	Name expected;
////
////	auto first_rdn = std::make_unique<RelativeDistinguishedName>();
////
////	auto first_attribute_and_value = std::make_unique<AttributeTypeAndValue>(std::make_unique<ObjectIdentifier>(ObjectIdentifier::parse("1.2.840.113549.1.9.1")), std::make_unique<Any>());
////	first_attribute_and_value->value->bytes = { 0x16, 0x0F, 0x63, 0x61, 0x40, 0x73, 0x6B, 0x62, 0x6B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x2E, 0x72, 0x75 };
////
////	auto first_rdn = std::make_unique<RelativeDistinguishedName>();
////
////	auto first_attribute_and_value = std::make_unique<AttributeTypeAndValue>(std::make_unique<ObjectIdentifier>(ObjectIdentifier::parse("1.2.840.113549.1.9.1")), std::make_unique<Any>());
////	first_attribute_and_value->value->bytes = { 0x16, 0x0F, 0x63, 0x61, 0x40, 0x73, 0x6B, 0x62, 0x6B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x2E, 0x72, 0x75 };
////
////	first_rdn->elements.push_back(first_attribute_and_value);
////
////	expected.elements.push_back(first_rdn);
////	expected.elements.push_back(first_rdn);
////	expected.elements.push_back(first_rdn);
////	
////
////	test_decode<RelativeDistinguishedName, RelativeDistinguishedNameDecoder>(data, expected);
////
////
////		std::string oid_ia5_expected = "1.2.840.113549.1.9.1";
////		std::string ia5_expected = "ca@skbkontur.ru";
////
////		std::string oid_printable_expected = "2.5.4.3";
////		std::string printable_expected = "UC SKB Kontur (Test)";
////		
////		std::string oid_utf8_expected = "2.5.4.7";
////		std::wstring utf8_expected = L"Екатеринбург";
////		
////}
//
////TEST(subject_public_key_info_case, decode_test)
////{
////	std::vector<uint8_t> data = {
////		0x30, 0x20,
////			0x30, 0x13, 
////				0x06, 0x06, 0x2A, 0x85, 0x03, 0x02,  0x02, 0x13,
////				0x30, 0x09, 
////					0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x24, 0x00,
////			0x03, 0x09, 0x00, 0xD0, 0xB7, 0x2C, 0xB5, 0x25, 0x09, 0x0A, 0xAA
////	};
////
////	int success = 0;
////
////	pkix::SubjectPublicKeyInfoDecoder decoder([&success](std::unique_ptr<IAsn1Value> val)
////	{
////		++success;
////
////		std::string algorithm_oid_expected = "1.2.643.2.2.19";
////		std::vector<uint8_t> algorithm_param_expected = { 0x30, 0x09, 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x24, 0x00 };
////		std::string subject_public_key_expected = "0101010101010000100100001010010010101101001101001110110100001011";
////
////		auto subject_public_key_info = static_cast<pkix::SubjectPublicKeyInfo*>(val.get());
////		EXPECT_EQ(algorithm_oid_expected, subject_public_key_info->algorithm->algorithm->to_string());
////		EXPECT_EQ(algorithm_param_expected, subject_public_key_info->algorithm->parameters->bytes);
////		EXPECT_EQ(subject_public_key_expected, subject_public_key_info->subject_public_key->to_string());
////	});
////
////	TestDecoder(&decoder, data);
////	EXPECT_EQ(success, 1);
////}
////
////
////
////void validate_cert(std::unique_ptr<IAsn1Value> val)
////{
////	using namespace asn1::pkix;
////	auto certificate = static_cast<cert::Certificate*>(val.get());
////	
////	EXPECT_EQ("1011110011000010000000011101010101001111110101011011010001000010", certificate->hash->to_string());
////	EXPECT_EQ("1.2.643.2.2.3", certificate->algorithm_identifier->algorithm->to_string());
////	std::vector<uint8_t> expected_certificate_alg_id_param = { 0x05, 0x00 };
////	EXPECT_EQ(expected_certificate_alg_id_param, certificate->algorithm_identifier->parameters->bytes);
////
////	auto cert_info = certificate->to_be_signed.get();
////	EXPECT_EQ(2, cert_info->version->to_int64());
////	std::vector<uint8_t> expected_serial_number = { 0x02, 0x0A, 0x61, 0x34, 0x2D, 0x5C, 0x00, 0x01, 0x00, 0x00, 0x00, 0x82 };
////	EXPECT_EQ(expected_serial_number, cert_info->serial_number->bytes);
////
/////*
////
////	EXPECT_EQ("1.2.643.2.2.19", subject_public_key_info->algorithm->algorithm->to_string());
////	auto param = static_cast<Any*>(subject_public_key_info->algorithm->parameters.get());
////	std::vector<uint8_t> param_expected = { 0x30, 0x09, 0x06, 0x07, 0x2A, 0x85, 0x03, 0x02, 0x02, 0x24, 0x00 };
////	EXPECT_EQ(param_expected, param->bytes);
////	auto actual_bit_string = subject_public_key_info->subject_public_key->to_string();
////	EXPECT_EQ("0101010101010000100100001010010010101101001101001110110100001011", actual_bit_string);*/
////}
//
//TEST(certificate_case, decode_test)
//{
//	auto data = FromFile(L"data/certificate");
//	int success = 0;
//
//	pkix::cert::CertificateDecoder decoder([&success](std::unique_ptr<IAsn1Value> val)
//	{
//		++success;
//	});
//
//	TestDecoder(&decoder, data);
//	EXPECT_EQ(success, 1);
//}
//
//TEST(certificate_case, decode_encode_test)
//{
//	auto data = FromFile(L"data/certificate");
//	int success = 0;
//
//	io::BufferedWriter writer;
//
//	pkix::cert::CertificateDecoder decoder([&success, &writer](std::unique_ptr<IAsn1Value> val)
//	{
//		++success;
//		val->encode(BerEncoder(), writer);
//	});
//
//	TestDecoder(&decoder, data);
//	EXPECT_EQ(success, 1);
//}
//
////
////TEST(pkcs7_case, signed_data_decode_test)
////{
////	auto data = FromFile(L"data/signature");
////	int success = 0;
////
////	pkix::cert::CertificateDecoder decoder([&success](std::unique_ptr<IAsn1Value> val)
////	{
////		++success;
////	});
////
////	TestDecoder(&decoder, data);
////	EXPECT_EQ(success, 1);
////}