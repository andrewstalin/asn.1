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
//#include "asn1/encoders/encoder.h"
//#include "asn1/encoders/stream_encoder.h"
//
//using namespace asn1;
//
//template<typename E>
//void StreamEncodeTest(E& stream_encoder, const std::vector<uint8_t>& data, const std::vector<uint8_t>& expected)
//{
//	auto test_data = SplitTestData(data);
//	io::BufferedWriter writer;
//	auto random_bit = GenerateRandomNumber(0, 1);
//
//	for (auto i = 0u; i < test_data.size(); ++i)
//	{
//		auto chunk = test_data[i];
//		stream_encoder.encode(writer, chunk.data(), chunk.size(), i == test_data.size() - 1 && random_bit > 0);
//	}
//
//	if (random_bit == 0)
//	{
//		stream_encoder.encode(writer, nullptr, 0, true);
//	}
//
//	EXPECT_EQ(0, stream_encoder.encode(writer, data.data(), data.size()));
//	EXPECT_EQ(0, stream_encoder.encode(writer, data.data(), data.size(), true));
//	CompareVectors(expected, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//}
//
//TEST(stream_encoder_case, primitive_octet_encode_test)
//{
//	std::vector<uint8_t> data = { 0x89, 0x0B, 0x87, 0x0F, 0x6A, 0x4D, 0x97, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 };
//	std::vector<uint8_t> expected = { 0x04, 0x0D, 0x89, 0x0B, 0x87, 0x0F, 0x6A, 0x4D, 0x97, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 };
//	DerEncoder encoder;
//	OctetStreamEncoder stream_encoder(Tag::octet_string(), data.size(), &encoder);
//	StreamEncodeTest<OctetStreamEncoder>(stream_encoder, data, expected);
//}
//
//TEST(stream_encoder_case, structured_octet_encode_test)
//{
//	std::vector<uint8_t> data = { 0x89, 0x0B, 0x87, 0x0F, 0x6A, 0x4D, 0x97, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 };
//	std::vector<uint8_t> expected = { 0x04, 0x15, 
//		0x04, 0x04, 0x89, 0x0B, 0x87, 0x0F, 
//		0x04, 0x04, 0x6A, 0x4D, 0x97, 0x67, 
//		0x04, 0x04, 0x0D, 0xC7, 0x1A, 0x76, 
//		0x04, 0x01, 0x47 };
//	BerEncoder encoder;
//	OctetStreamEncoder stream_encoder(Tag::octet_string(), data.size(), { Tag::octet_string(), 4 }, &encoder);
//	StreamEncodeTest<OctetStreamEncoder>(stream_encoder, data, expected);
//}
//
//TEST(stream_encoder_case, undefined_structured_octet_encode_test)
//{
//	std::vector<uint8_t> data = { 0x89, 0x0B, 0x87, 0x0F, 0x6A, 0x4D, 0x97, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 };
//	std::vector<uint8_t> expected = { 0x04, 0x80,
//		0x04, 0x04, 0x89, 0x0B, 0x87, 0x0F,
//		0x04, 0x04, 0x6A, 0x4D, 0x97, 0x67,
//		0x04, 0x04, 0x0D, 0xC7, 0x1A, 0x76,
//		0x04, 0x01, 0x47,
//		0x00, 0x00 };
//	BerEncoder encoder;
//	OctetStreamEncoder stream_encoder(Tag::octet_string(), INDEFINITE_CONTENT_LENGTH, { Tag::octet_string(), 4 }, &encoder);
//	StreamEncodeTest<OctetStreamEncoder>(stream_encoder, data, expected);
//}
//
//TEST(stream_encoder_case, primitive_bit_encode_test)
//{
//	std::vector<uint8_t> data = { 0x89, 0x0B, 0x87, 0x0F, 0x6A, 0x4D, 0x97, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 };
//	std::vector<uint8_t> expected = { 0x03, 0x0E, 0x01, 0x89, 0x0B, 0x87, 0x0F, 0x6A, 0x4D, 0x97, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 };
//	DerEncoder encoder;
//	BitStreamEncoder stream_encoder(Tag::bit_string(), data.size(), &encoder);
//	stream_encoder.set_unused_bits(1);
//	StreamEncodeTest<BitStreamEncoder>(stream_encoder, data, expected);
//}
//
//TEST(stream_encoder_case, structured_bit_encode_test)
//{
//	std::vector<uint8_t> data = { 0x89, 0x0B, 0x87, 0x0F, 0x6A, 0x4D, 0x97, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 };
//	std::vector<uint8_t> expected = { 0x03, 0x19, 
//		0x03, 0x05, 0x00, 0x89, 0x0B, 0x87, 0x0F,
//		0x03, 0x05, 0x00, 0x6A, 0x4D, 0x97, 0x67, 
//		0x03, 0x05, 0x00, 0x0D, 0xC7, 0x1A, 0x76, 
//		0x03, 0x02, 0x03, 0x47 };
//	BerEncoder encoder;
//	BitStreamEncoder stream_encoder(Tag::bit_string(), data.size(), { Tag::bit_string(), 4 }, &encoder);
//	stream_encoder.set_unused_bits(3);
//	StreamEncodeTest<BitStreamEncoder>(stream_encoder, data, expected);
//}
//
//TEST(stream_encoder_case, undefined_structured_bit_encode_test)
//{
//	std::vector<uint8_t> data = { 0x89, 0x0B, 0x87, 0x0F, 0x6A, 0x4D, 0x97, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 };
//	std::vector<uint8_t> expected = { 0x03, 0x80, 
//		0x03, 0x05, 0x00, 0x89, 0x0B, 0x87, 0x0F,
//		0x03, 0x05, 0x00, 0x6A, 0x4D, 0x97, 0x67,
//		0x03, 0x05, 0x00, 0x0D, 0xC7, 0x1A, 0x76,
//		0x03, 0x02, 0x05, 0x47,
//		0x00, 0x00 };
//	BerEncoder encoder;
//	BitStreamEncoder stream_encoder(Tag::bit_string(), INDEFINITE_CONTENT_LENGTH, { Tag::bit_string(), 4 }, &encoder);
//	stream_encoder.set_unused_bits(5);
//	StreamEncodeTest<BitStreamEncoder>(stream_encoder, data, expected);
//}