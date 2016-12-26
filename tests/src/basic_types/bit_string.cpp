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
#include "asn1/basic_types/bit_string.h"
#include "asn1/encoders/encoder.h"
#include "asn1/error.h"
#include "gtest/gtest.h"

using namespace asn1;

void BitStringDecodeTest(const std::vector<byte_type>& bytes, const std::string& expected)
{
	test::BasicDecoderTestEventHandler event_handler([&](Asn1Value&& val)
	{
		auto bit_string = static_cast<BitString&>(val);
		EXPECT_EQ(expected, bit_string.to_string());
	});
	BitStringDecoder decoder(&event_handler);

	test::BasicDecoderTest decoder_test(&decoder);
	decoder_test.execute(bytes);

	EXPECT_TRUE(event_handler.fire);
}

TEST(bit_string_case, to_string_without_unused_bits_test)
{
	BitString bit_string;
	bit_string.bytes = { 0xEA, 0xE4, 0x7D, 0xFB };
	EXPECT_EQ("11101010111001000111110111111011", bit_string.to_string());
}

TEST(bit_string_case, to_string_with_trailing_zeroes_without_unused_bits_test)
{
	BitString bit_string;
	bit_string.bytes = { 0x0D, 0xE4, 0x7D };
	EXPECT_EQ("11011110010001111101", bit_string.to_string());
}

TEST(bit_string_case, to_string_zero_without_unused_bits_test)
{
	BitString bit_string;
	bit_string.bytes = { 0x00, 0x00 };
	EXPECT_EQ("", bit_string.to_string());
}

TEST(bit_string_case, to_string_zero_with_unused_bits_test)
{
	BitString bit_string;
	bit_string.unused_bits = 4;
	bit_string.bytes = { 0x00, 0x00 };
	EXPECT_EQ("", bit_string.to_string());
}

TEST(bit_string_case, to_string_with_unused_bits_test)
{
	BitString bit_string;
	bit_string.unused_bits = 4;
	bit_string.bytes = { 0xEA, 0xE4, 0x7D, 0xFB };
	EXPECT_EQ("1110101011100100011111011111", bit_string.to_string());
}

TEST(bit_string_case, to_string_with_trailing_zeroes_with_unused_bits_test)
{
	BitString bit_string;
	bit_string.unused_bits = 4;
	bit_string.bytes = { 0xF0, 0xE4, 0xFB };
	EXPECT_EQ("11110000111001001111", bit_string.to_string());
}

TEST(bit_string_case, to_uint16_with_unused_bits_test)
{
	BitString bit_string;
	bit_string.unused_bits = 4;
	bit_string.bytes = { 0xF0 };

	EXPECT_EQ(15, bit_string.to_integral_type<uint16_t>());
}

TEST(bit_string_case, to_uint16_without_unused_bits_test)
{
	BitString bit_string;
	bit_string.unused_bits = 0;
	bit_string.bytes = { 0xF0, 0x14 };

	EXPECT_EQ(61460, bit_string.to_integral_type<uint16_t>());
}

TEST(bit_string_case, ctr_from_one_byte_uint16_test)
{
	uint16_t actual{ 15 };
	BitString bit_string(actual);
	EXPECT_EQ(actual, bit_string.to_integral_type<uint16_t>());
}

TEST(bit_string_case, ctr_from_two_byte_uint16_test)
{
	uint16_t actual{ 458 };
	BitString bit_string(actual);
	EXPECT_EQ(actual, bit_string.to_integral_type<uint16_t>());
}

TEST(bit_string_case, parse_test)
{
	auto bit_string = BitString::parse("11111110101011001100110100110110010010100011011");
	EXPECT_EQ("11111110011010110110011011011000101001001101100", bit_string.to_string());
}

TEST(bit_string_decoder_case, decode_definite_length_test)
{
	BitStringDecodeTest({ 0x03, 0x07, 0x02, 0xD9, 0xA6, 0xDA, 0x66, 0x68, 0xFC }, "1101100110100110110110100110011001101000111111");
}

TEST(bit_string_decoder_case, decode_definite_length_with_block_test)
{
	std::vector<byte_type> bytes = { 0x23, 0x10,
		0x03, 0x04, 0x00, 0x02, 0xD9, 0xA6,
		0x03, 0x03, 0x00, 0xDA, 0x66,
		0x03, 0x03, 0x02, 0x68, 0xFC }; 
	
	BitStringDecodeTest(bytes, "101101100110100110110110100110011001101000111111");
}

TEST(bit_string_decoder_case, decode_indefinite_length_with_block_test)
{
	std::vector<byte_type> bytes = { 0x23, 0x80,
		0x03, 0x04, 0x00, 0x02, 0xD9, 0xA6,
		0x03, 0x03, 0x00, 0xDA, 0x66,
		0x03, 0x03, 0x02, 0x68, 0xFC,
		0x00, 0x00 }; 
	
	BitStringDecodeTest(bytes, "101101100110100110110110100110011001101000111111");
}

TEST(bit_string_decoder_case, wrong_place_for_unused_bits_indefinite_length_with_block_test)
{
	std::vector<byte_type> data = { 0x23, 0x80,
		0x03, 0x04, 0x02, 0x02, 0xD9, 0xA6,
		0x03, 0x03, 0x00, 0xDA, 0x66,
		0x03, 0x03, 0x02, 0x68, 0xFC,
		0x00, 0x00 };

	EXPECT_THROW(BitStringDecodeTest(data, ""), Asn1Exception);
}

TEST(bit_string_decoder_case, wrong_place_for_unused_bits_definite_length_with_block_test)
{
	std::vector<byte_type> data = { 0x23, 0x10,
		0x03, 0x04, 0x02, 0x02, 0xD9, 0xA6,
		0x03, 0x03, 0x00, 0xDA, 0x66,
		0x03, 0x03, 0x00, 0x68, 0xFC };

	EXPECT_THROW(BitStringDecodeTest(data, ""), Asn1Exception);
}

TEST(bit_string_case, encode_test)
{
	BitString actual;
	actual.unused_bits = 3;
	actual.bytes = { 0x01, 0x65 };

	test::BasicEncoderTest encoder_test;
	encoder_test.execute(actual, { 0x03, 0x03, 0x03, 0x01, 0x65 });
}

TEST(ber_bit_string_encoder_case, ber_encoder_definite_length_test)
{
	BitString bit_string;
	bit_string.bytes = { 0xD9, 0xA6, 0xDA, 0x66, 0x68, 0xFF };
	bit_string.unused_bits = 2;
	
	test::BasicEncoderTest encoder_test;
	encoder_test.execute(bit_string, { 0x03, 0x07, 0x02, 0xD9, 0xA6, 0xDA, 0x66, 0x68, 0xFF });
}

TEST(ber_bit_string_encoder_case, ber_encoder_definite_length_with_block_test)
{
	size_t block_size{ 2 };

	BitString bit_string;
	bit_string.tag.encoding_form = EncodingForm::CONSTRUCTED;
	bit_string.unused_bits = 2;
	bit_string.bytes = { 0xD9, 0xA6, 0xDA, 0x66, 0x68, 0xFF };

	io::BufferedWriter writer;
	BerEncoder encoder;
	encoder.encode_bit_string(writer, bit_string.tag, bit_string.unused_bits, block_size, bit_string.bytes.data(), bit_string.bytes.size());

	EXPECT_EQ(std::vector<byte_type>({ 0x23, 0x0F, 0x03, 0x03, 0x00, 0xD9, 0xA6, 0x03, 0x03, 0x00, 0xDA, 0x66, 0x03, 0x03, 0x02, 0x68, 0xFF }), writer.bytes);
}

//TEST(ber_bit_string_encoder_case, ber_encoder_indefinite_length_with_block_test)
//{
//	io::BufferedWriter writer;
//	Tag tag = BitString::tag();
//	tag.encodingForm = EncodingForm::CONSTRUCTED;
//	encodeBitString(writer, tag, 2, true);
//	compare({ 0x23, 0x80, 0x03, 0x03, 0x00, 0xD9, 0xA6, 0x03, 0x03, 0x00, 0xDA, 0x66, 0x03, 0x03, 0x02, 0x68, 0xFC, 0x00, 0x00 }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//}
