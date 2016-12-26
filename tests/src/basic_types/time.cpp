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
//#include "asn1/basic_types/time.h"
//#include "asn1/decoders/time_decoder.h"
//#include "asn1/io/buffered.h"
//
//using namespace asn1;
//
//void CheckTime(const Time& expected, const Time& actual)
//{
//	EXPECT_EQ(expected.year, actual.year);
//	EXPECT_EQ(expected.month, actual.month);
//	EXPECT_EQ(expected.day, actual.day);
//	EXPECT_EQ(expected.hour, actual.hour);
//	EXPECT_EQ(expected.minute, actual.minute);
//	EXPECT_EQ(expected.second, actual.second);
//	EXPECT_EQ(expected.offset, actual.offset);
//}
//
//void CheckUtcTime(const UtcTime& expected, const UtcTime& actual)
//{
//	CheckTime(expected, actual);
//}
//
//void CheckGeneralizedTime(const GeneralizedTime& expected, const GeneralizedTime& actual)
//{
//	CheckTime(expected, actual);
//	EXPECT_EQ(expected.fraction, actual.fraction);
//	EXPECT_EQ(expected.utc, actual.utc);
//}
//
//void TimeEncodeTest(const IEncoder& encoder, const Time& time, const std::vector<uint8_t>& expected)
//{
//	io::BufferedWriter writer;
//	time.encode(encoder, writer);
//	auto x = ToHextStr(std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//	CompareVectors(expected, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//}
//
//TEST(utctime_case, parse_test)
//{
//	CheckUtcTime(UtcTime(76, 10, 21, 7, 56, 0), UtcTime::parse("7610210756Z"));
//	CheckUtcTime(UtcTime(76, 10, 21, 7, 56, 43), UtcTime::parse("761021075643Z"));
//
//	CheckUtcTime(UtcTime(76, 10, 21, 7, 56, 43, 20), UtcTime::parse("761021075643+0020"));
//	CheckUtcTime(UtcTime(76, 10, 21, 7, 56, 43, -80), UtcTime::parse("761021075643-0120"));
//
//	CheckUtcTime(UtcTime(76, 10, 21, 7, 56, 0, 20), UtcTime::parse("7610210756+0020"));
//	CheckUtcTime(UtcTime(76, 10, 21, 7, 56, 0, -80), UtcTime::parse("7610210756-0120"));
//}
//
//TEST(utctime_case, error_parse_test)
//{
//	EXPECT_THROW(UtcTime::parse("761021Z"), Asn1Exception);
//	EXPECT_THROW(UtcTime::parse("76102107564"), Asn1Exception);
//	EXPECT_THROW(UtcTime::parse("76102107564F"), Asn1Exception);
//	EXPECT_THROW(UtcTime::parse("761021075643"), Asn1Exception);
//	EXPECT_THROW(UtcTime::parse("76102107564+04F7"), Asn1Exception);
//}
//
//TEST(generalizedtime_case, parse_test)
//{
//	auto create_time = [](GeneralizedTime time, uint64_t fraction, bool utc)
//	{
//		time.fraction = fraction;
//		time.utc = utc;
//		return time;
//	};
//
//	CheckGeneralizedTime(GeneralizedTime(1976, 10, 21, 10, 0, 0, 0), GeneralizedTime::parse("1976102110"));
//	CheckGeneralizedTime(GeneralizedTime(1976, 10, 21, 10, 23, 0, 0), GeneralizedTime::parse("197610211023"));
//	CheckGeneralizedTime(GeneralizedTime(1976, 10, 21, 10, 23, 46, 0), GeneralizedTime::parse("19761021102346"));
//	CheckGeneralizedTime(create_time(GeneralizedTime(1976, 10, 21, 10, 23, 46, 0), 567, false), GeneralizedTime::parse("19761021102346.567"));
//
//	CheckGeneralizedTime(create_time(GeneralizedTime(1976, 10, 21, 10, 0, 0, 0), 0, true), GeneralizedTime::parse("1976102110Z"));
//	CheckGeneralizedTime(create_time(GeneralizedTime(1976, 10, 21, 10, 23, 0, 0), 0, true), GeneralizedTime::parse("197610211023Z"));
//	CheckGeneralizedTime(create_time(GeneralizedTime(1976, 10, 21, 10, 23, 46, 0), 0, true), GeneralizedTime::parse("19761021102346Z"));
//	CheckGeneralizedTime(create_time(GeneralizedTime(1976, 10, 21, 10, 23, 46, 0), 567, true), GeneralizedTime::parse("19761021102346.567Z"));
//
//	CheckGeneralizedTime(GeneralizedTime(1976, 10, 21, 10, 0, 0, 90), GeneralizedTime::parse("1976102110+0130"));
//	CheckGeneralizedTime(GeneralizedTime(1976, 10, 21, 10, 23, 0, -145), GeneralizedTime::parse("197610211023-0225"));
//	CheckGeneralizedTime(GeneralizedTime(1976, 10, 21, 10, 23, 46, 20), GeneralizedTime::parse("19761021102346+0020"));
//	CheckGeneralizedTime(create_time(GeneralizedTime(1976, 10, 21, 10, 23, 46, -40), 567, false), GeneralizedTime::parse("19761021102346.567-0040"));
//}
//
//TEST(utctime_case, ber_encode_YYMMDDhhmmZ_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x0B, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x5A };
//	auto time = UtcTime(14, 10, 23, 7, 23, 0);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(utctime_case, ber_encode_YYMMDDhhmm_minus_hhmm_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x0F, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x30, 0x37, 0x2D, 0x30, 0x31, 0x30, 0x37 };
//	auto time = UtcTime(14, 10, 23, 7, 7, 0, -67);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(utctime_case, ber_encode_YYMMDDhhmm_plus_hhmm_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x0F, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x30, 0x37, 0x2B, 0x30, 0x30, 0x32, 0x33 };
//	auto time = UtcTime(14, 10, 23, 7, 7, 0, 23);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(utctime_case, ber_encode_YYMMDDhhmmssZ_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x0D, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x5A };
//	auto time = UtcTime(14, 10, 23, 7, 23, 34);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(utctime_case, ber_encode_YYMMDDhhmmss_minus_hhmm_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x11, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x2D, 0x30, 0x30, 0x33, 0x30 };
//	auto time = UtcTime(14, 10, 23, 7, 23, 34, -30);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(utctime_case, ber_encode_YYMMDDhhmmss_plus_hhmm_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x11, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x2B, 0x30, 0x30, 0x32, 0x37 };
//	auto time = UtcTime(14, 10, 23, 7, 23, 34, 27);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(utctime_case, der_encode_YYMMDDhhmmZ_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x0D, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x30, 0x30, 0x5A };
//	auto time = UtcTime(14, 10, 23, 7, 23, 0);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(utctime_case, der_encode_YYMMDDhhmm_minus_hhmm_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x0D, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x36, 0x30, 0x30, 0x30, 0x30, 0x5A };
//	auto time = UtcTime(14, 10, 23, 7, 7, 0, -67);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(utctime_case, der_encode_YYMMDDhhmm_plus_hhmm_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x0D, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x33, 0x30, 0x30, 0x30, 0x5A };
//	auto time = UtcTime(14, 10, 23, 7, 7, 0, 23);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(utctime_case, der_encode_YYMMDDhhmmssZ_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x0D, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x5A };
//	auto time = UtcTime(14, 10, 23, 7, 23, 34);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(utctime_case, der_encode_YYMMDDhhmmss_minus_hhmm_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x0D, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x36, 0x35, 0x33, 0x33, 0x34, 0x5A };
//	auto time = UtcTime(14, 10, 23, 7, 23, 34, -30);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(utctime_case, der_encode_YYMMDDhhmmss_plus_hhmm_test)
//{
//	std::vector<uint8_t> expected = { 0x17, 0x0D, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x35, 0x30, 0x33, 0x34, 0x5A };
//	auto time = UtcTime(14, 10, 23, 7, 23, 34, 27);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHH_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0A, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37 };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 0, 0, 0);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHHMM_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0C, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33 };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 0, 0);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHHMMSS_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0E, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34 };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, 0);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHHMMSSfff_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x12, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x2E, 0x33, 0x34, 0x35 };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, 0);
//	time.fraction = 345;
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHHZ_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0B, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 0, 0, 0);
//	time.utc = true;
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHHMMZ_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0D, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 0, 0);
//	time.utc = true;
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHHMMSSZ_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0F, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, 0);
//	time.utc = true;
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHHMMSSfffZ_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x13, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x2E, 0x33, 0x34, 0x35, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, 0);
//	time.fraction = 345;
//	time.utc = true;
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHHZ_HHMM_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0F, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x2B, 0x30, 0x30, 0x32, 0x30 };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 0, 0, 20);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHHMM_HHMM_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x11, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x2D, 0x30, 0x30, 0x34, 0x35 };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 0, -45);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHHMMSS_HHMM_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x13, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x2D, 0x30, 0x30, 0x31, 0x30 };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, -10);
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, ber_encode_YYYYMMDDHHMMSSfff_HHMM_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x17, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x2E, 0x33, 0x34, 0x35, 0x2B, 0x30, 0x32, 0x30, 0x33 };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, 123);
//	time.fraction = 345;
//	TimeEncodeTest(BerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHH_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0F, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x30, 0x30, 0x30, 0x30, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 0, 0, 0);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHHMM_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0F, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x30, 0x30, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 0, 0);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHHMMSS_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0F, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, 0);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHHMMSSfff_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x13, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x2E, 0x33, 0x34, 0x35, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, 0);
//	time.fraction = 345;
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHHZ_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0F, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x30, 0x30, 0x30, 0x30, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 0, 0, 0);
//	time.utc = true;
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHHMMZ_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0F, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x30, 0x30, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 0, 0);
//	time.utc = true;
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHHMMSSZ_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0F, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, 0);
//	time.utc = true;
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHHMMSSfffZ_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x13, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x33, 0x33, 0x34, 0x2E, 0x33, 0x34, 0x35, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, 0);
//	time.fraction = 345;
//	time.utc = true;
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHHZ_HHMM_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0F, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x32, 0x30, 0x30, 0x30, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 0, 0, 20);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHHMM_HHMM_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0F, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x36, 0x33, 0x38, 0x30, 0x30, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 0, -45);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHHMMSS_HHMM_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x0F, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x37, 0x31, 0x33, 0x33, 0x34, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, -10);
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(generalizedtime_case, der_encode_YYYYMMDDHHMMSSfff_HHMM_test)
//{
//	std::vector<uint8_t> expected = { 0x18, 0x13, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x30, 0x39, 0x32, 0x36, 0x33, 0x34, 0x2E, 0x33, 0x34, 0x35, 0x5A };
//	auto time = GeneralizedTime(2014, 10, 23, 7, 23, 34, 123);
//	time.fraction = 345;
//	TimeEncodeTest(DerEncoder(), time, expected);
//}
//
//TEST(utctime_decoder_case, decode_test)
//{
//	std::vector<uint8_t> data = { 0x17, 0x11, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x32, 0x30, 0x33, 0x30, 0x34, 0x37, 0x2D, 0x30, 0x30, 0x33, 0x30 };
//	bool fire{ false };
//
//	UtcTimeDecoder decoder([&fire](std::unique_ptr<IAsn1Value> val)
//	{
//		fire = true;
//		auto expected = UtcTime(2014, 10, 23, 20, 30, 47, -30);
//		auto time = static_cast<UtcTime*>(val.get());
//		CheckUtcTime(expected, *time);
//	});
//
//	TestDecoder(&decoder, data);
//	EXPECT_TRUE(fire);
//}
//
//TEST(generalizedtime_decoder_case, decode_test)
//{
//	std::vector<uint8_t> data = { 0x18, 0x17, 0x32, 0x30, 0x31, 0x34, 0x31, 0x30, 0x32, 0x33, 0x32, 0x30, 0x33, 0x30, 0x34, 0x37, 0x2E, 0x34, 0x35, 0x36, 0x2B, 0x30, 0x30, 0x34, 0x30 };
//	bool fire{ false };
//
//	GeneralizedTimeDecoder decoder([&fire](std::unique_ptr<IAsn1Value> val)
//	{
//		fire = true;
//		auto expected = GeneralizedTime(2014, 10, 23, 20, 30, 47, 40);
//		expected.fraction = 456;
//		auto time = static_cast<GeneralizedTime*>(val.get());
//		CheckGeneralizedTime(expected, *time);
//	});
//
//	TestDecoder(&decoder, data);
//	EXPECT_TRUE(fire);
//}