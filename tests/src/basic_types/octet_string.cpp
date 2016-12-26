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
//#include "asn1/basic_types/octet_string.h"
//#include "asn1/decoders/octet_string_decoder.h"
//#include "asn1/io/buffered.h"
//
//using namespace asn1;
//////
//////TEST(octet_string_case, encode_test)
//////{
//////	io::BufferedWriter writer;
//////	OctetString string;
//////
//////	compare({ 0x03, 0x03, 0x07, 0xB2, 0x80 }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//////}
//
////void encodeOctetString(io::BufferedWriter& writer, const Tag& tag, uint64_t block = INDEFINITE_CONTENT_LENGTH, bool indefinite = false)
////{
////	std::vector<std::vector<uint8_t>> data = { { 0x04, 0x0F }, { 0x6A, 0x4D, 0x97 }, { 0x67 }, { 0x0D, 0xC7, 0x1A, 0x76, 0x47 } };
////	BerOctetStringEncoder encoder(indefinite ? INDEFINITE_CONTENT_LENGTH :  std::accumulate(data.begin(), data.end(), 0, [](size_t x, std::vector<uint8_t> y) { return x + y.size(); }), block);
////	Header::encode(writer, tag, encoder.encodedSize());
////	encoder.encode(writer, data[0].data(), data[0].size());
////	encoder.encode(writer, data[1].data(), data[1].size());
////	encoder.encode(writer, data[2].data(), data[2].size());
////	encoder.encode(writer, data[3].data(), data[3].size(), true);
////}
////
////TEST(ber_octet_string_encoder_case, ber_encoder_definite_length_test)
////{
////	io::BufferedWriter writer;
////	encodeOctetString(writer, OctetString::tag());
////	compare({ 0x04, 0x0B, 0x04, 0x0F, 0x6A, 0x4D, 0x97, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
////}
////
////TEST(ber_octet_string_encoder_case, ber_encoder_definite_length_with_block_test)
////{
////	io::BufferedWriter writer;
////	Tag tag = OctetString::tag();
////	tag.encodingForm = EncodingForm::CONSTRUCTED;
////	encodeOctetString(writer, tag, 2);
////	compare({ 0x24, 0x17, 0x04, 0x02, 0x04, 0x0F, 0x04, 0x02, 0x6A, 0x4D, 0x04, 0x02, 0x97, 0x67, 0x04, 0x02, 0x0D, 0xC7, 0x04, 0x02, 0x1A, 0x76, 0x04, 0x01, 0x47 }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
////}
////
////TEST(ber_octet_string_encoder_case, ber_encoder_indefinite_length_with_block_test)
////{
////	io::BufferedWriter writer;
////	Tag tag = OctetString::tag();
////	tag.encodingForm = EncodingForm::CONSTRUCTED;
////	encodeOctetString(writer, tag, 2, true);
////	compare({ 0x24, 0x80, 0x04, 0x02, 0x04, 0x0F, 0x04, 0x02, 0x6A, 0x4D, 0x04, 0x02, 0x97, 0x67, 0x04, 0x02, 0x0D, 0xC7, 0x04, 0x02, 0x1A, 0x76, 0x04, 0x01, 0x47, 0x00, 0x00 }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
////}
//
//
//
//
//
//
//
////TEST(ber_octet_string_encoder_case, ber_encoder_definite_length_test)
////{
////	std::vector<uint8_t> expected = { 0x04, 0x04, 0x30, 0x80, 0x24, 0x80 };
////	std::vector<uint8_t> data = { 0x30, 0x80, 0x24, 0x80 };
////	io::BufferedWriter writer;
////	BerOctetStringEncoder encoder;
////	encoder.encode(writer, Tag::octet_string(), data.data(), data.size());
////
////	auto actual = std::vector<uint8_t>(writer.data(), writer.data() + writer.size());
////	CompareVectors(expected, actual);
////}
////
////TEST(ber_octet_string_encoder_case, ber_encoder_indefinite_length_test)
////{
////	std::vector<uint8_t> expected = { 0x04, 0x80, 0x04, 0x02, 0x30, 0x80, 0x04, 0x02, 0x24, 0x80, 0x04, 0x01, 0x56, 0x00, 0x00 };
////	std::vector<uint8_t> data = { 0x30, 0x80, 0x24, 0x80, 0x56 };
////	io::BufferedWriter writer;
////	BerOctetStringEncoder encoder;
////	encoder.set_block_size(2);
////	encoder.encode(writer, Tag::octet_string(), data.data(), data.size());
////
////	auto actual = std::vector<uint8_t>(writer.data(), writer.data() + writer.size());
////	CompareVectors(expected, actual);
////}
//
//TEST(octet_string_decoder_case, decode_definite_length_test)
//{
//	std::vector<uint8_t> data = { 0x04, 0x0F, 0x6A, 0x4D, 0x97, 0x43, 0x5B, 0x09, 0x0D, 0xC7, 0x1A, 0x76, 0x47, 0x72, 0x63, 0x47, 0xA4 };
//	std::vector<uint8_t> expected = { 0x6A, 0x4D, 0x97, 0x43, 0x5B, 0x09, 0x0D, 0xC7, 0x1A, 0x76, 0x47, 0x72, 0x63, 0x47, 0xA4 };
//	bool fire{ false };
//
//	OctetStringDecoder decoder([&fire, &expected](std::unique_ptr<IAsn1Value>& val)
//	{
//		fire = true;
//		auto octet_string = static_cast<OctetString*>(val.get());
//		CompareVectors(expected, std::vector<uint8_t>(octet_string->bytes.data(), octet_string->bytes.data() + octet_string->bytes.size()));
//	});
//
//	TestDecoder(&decoder, data);
//	EXPECT_TRUE(fire);
//}
//
//TEST(octet_string_decoder_case, decode_definite_length_with_block_test)
//{
//	std::vector<uint8_t> data = { 0x24, 0x19, 0x04, 0x03, 0x6A, 0x4D, 0x97, 0x04, 0x03, 0x43, 0x5B, 0x09, 0x04, 0x03, 0x0D, 0xC7, 0x1A, 0x04, 0x03, 0x76, 0x47, 0x72, 0x04, 0x03, 0x63, 0x47, 0xA4 };
//	std::vector<uint8_t> expected = { 0x6A, 0x4D, 0x97, 0x43, 0x5B, 0x09, 0x0D, 0xC7, 0x1A, 0x76, 0x47, 0x72, 0x63, 0x47, 0xA4 };
//	Tag tag = Tag::octet_string();
//	tag.encoding_form = EncodingForm::CONSTRUCTED;
//	bool fire{ false };
//
//	OctetStringDecoder decoder(tag, [&fire, &expected](std::unique_ptr<IAsn1Value>& val)
//	{
//		fire = true;
//		auto octet_string = static_cast<OctetString*>(val.get());
//		CompareVectors(expected, std::vector<uint8_t>(octet_string->bytes.data(), octet_string->bytes.data() + octet_string->bytes.size()));
//	});
//
//	TestDecoder(&decoder, data);
//	EXPECT_TRUE(fire);
//}
//
//TEST(octet_string_decoder_case, decode_indefinite_length_with_block_test)
//{
//	std::vector<uint8_t> data = { 0x24, 0x80, 0x04, 0x03, 0x6A, 0x4D, 0x97, 0x04, 0x02, 0x14, 0x43, 0x00, 0x00 };
//	std::vector<uint8_t> expected = { 0x6A, 0x4D, 0x97, 0x14, 0x43 };
//	Tag tag = Tag::octet_string();
//	tag.encoding_form = EncodingForm::CONSTRUCTED;
//	bool fire{ false };
//
//	OctetStringDecoder decoder(tag, [&fire, &expected](std::unique_ptr<IAsn1Value>& val)
//	{
//		fire = true;
//		auto octet_string = static_cast<OctetString*>(val.get());
//		CompareVectors(expected, std::vector<uint8_t>(octet_string->bytes.data(), octet_string->bytes.data() + octet_string->bytes.size()));
//	});
//
//	TestDecoder(&decoder, data);
//	EXPECT_TRUE(fire);
//}