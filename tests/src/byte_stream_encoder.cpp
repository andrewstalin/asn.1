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
//#include "asn1/basic_types/bit_string.h"
//#include "asn1/basic_types/byte_collection.h"
//#include "asn1/io/buffered.h"
//
//using namespace asn1;
//
//TEST(byte_stream_encoder_case, encode_definite_length_test)
//{
//	std::vector<std::vector<uint8_t>> data = { { 0x04, 0x0F }, { 0x6A, 0x4D, 0x97 }, { 0x67 }, { 0x0D, 0xC7, 0x1A, 0x76, 0x47 } };
//	io::BufferedWriter writer;
//	details::ByteStreamEncoder<OctetString, false> encoder(11);
//	encoder.encode(writer, data[0].data(), data[0].size());
//	encoder.encode(writer, data[1].data(), data[1].size());
//	encoder.encode(writer, data[2].data(), data[2].size());
//	encoder.encode(writer, data[3].data(), data[3].size(), true);
//	compare({ 0x04, 0x0B, 0x04, 0x0F, 0x6A, 0x4D, 0x97, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//}
//
//TEST(byte_stream_encoder_case, encode_bit_definite_length_test)
//{
//	std::vector<std::vector<uint8_t>> data = { { 0xD9 }, { 0xA6, 0xDA, 0x66 }, { 0x68, 0xFF } };
//	io::BufferedWriter writer;
//	details::ByteStreamEncoder<BitString, true> encoder(6);
//	encoder.unused(5);
//	encoder.encode(writer, data[0].data(), data[0].size());
//	encoder.encode(writer, data[1].data(), data[1].size());
//	encoder.encode(writer, data[2].data(), data[2].size(), true);
//	compare({ 0x03, 0x06, 0x05, 0xD9, 0xA6, 0xDA, 0x66, 0x68, 0xE0 }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//}
//
//TEST(byte_stream_encoder_case, encode_definite_length_with_block_test)
//{
//	std::vector<std::vector<uint8_t>> data = { { 0x04, 0x0F }, { 0x6A, 0x4D, 0x97 }, { 0x67 }, { 0x0D, 0xC7, 0x1A, 0x76, 0x47 }, { 0x91 } };
//	io::BufferedWriter writer;
//	details::ByteStreamEncoder<OctetString, false> encoder(12, 3);
//	encoder.encode(writer, data[0].data(), data[0].size());
//	encoder.encode(writer, data[1].data(), data[1].size());
//	encoder.encode(writer, data[2].data(), data[2].size());
//	encoder.encode(writer, data[3].data(), data[3].size());
//	encoder.encode(writer, data[4].data(), data[4].size(), true);
//	compare({ 0x24, 0x14, 0x04, 0x03, 0x04, 0x0F, 0x6A, 0x04, 0x03, 0x4D, 0x97, 0x67, 0x04, 0x03, 0x0D, 0xC7, 0x1A, 0x04, 0x03, 0x76, 0x47, 0x91 }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//}
//
//TEST(byte_stream_encoder_case, encode_bit_definite_length_with_block_test)
//{
//	std::vector<std::vector<uint8_t>> data = { { 0xD9 }, { 0xA6, 0xDA, 0x66 }, { 0x68, 0xFF } };
//	io::BufferedWriter writer;
//	details::ByteStreamEncoder<BitString, true> encoder(6, 2);
//	encoder.unused(3);
//	encoder.encode(writer, data[0].data(), data[0].size());
//	encoder.encode(writer, data[1].data(), data[1].size());
//	encoder.encode(writer, data[2].data(), data[2].size(), true);
//	compare({ 0x23, 0x0F, 0x03, 0x03, 0x00, 0xD9, 0xA6, 0x03, 0x03, 0x00, 0xDA, 0x66, 0x03, 0x03, 0x03, 0x68, 0xF8 }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//}
//
//TEST(byte_stream_encoder_case, encode_indefinite_length_with_block_test)
//{
//	std::vector<std::vector<uint8_t>> data = { { 0x04, 0x0F }, { 0x6A, 0x4D, 0x97 }, { 0x67 }, { 0x0D, 0xC7, 0x1A, 0x76, 0x47 }, { 0x91 } };
//	io::BufferedWriter writer;
//	details::ByteStreamEncoder<OctetString, false> encoder(INDEFINITE_CONTENT_LENGTH, 5);
//	encoder.encode(writer, data[0].data(), data[0].size());
//	encoder.encode(writer, data[1].data(), data[1].size());
//	encoder.encode(writer, data[2].data(), data[2].size());
//	encoder.encode(writer, data[3].data(), data[3].size());
//	encoder.encode(writer, data[4].data(), data[4].size(), true);
//	compare({ 0x24, 0x80, 0x04, 0x05, 0x04, 0x0F, 0x6A, 0x4D, 0x97, 0x04, 0x05, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x04, 0x02, 0x47, 0x91, 0x00, 0x00 }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//}
//
//TEST(byte_stream_encoder_case, encode_bit_indefinite_length_with_block_test)
//{
//	std::vector<std::vector<uint8_t>> data = { { 0xD9 }, { 0xA6, 0xDA, 0x66 }, { 0x68, 0xFF } };
//	io::BufferedWriter writer;
//	details::ByteStreamEncoder<BitString, true> encoder(INDEFINITE_CONTENT_LENGTH, 4);
//	encoder.encode(writer, data[0].data(), data[0].size());
//	encoder.encode(writer, data[1].data(), data[1].size());
//	encoder.encode(writer, data[2].data(), data[2].size(), true);
//	compare({ 0x23, 0x80, 0x03, 0x05, 0x00, 0xD9, 0xA6, 0xDA, 0x66, 0x03, 0x03, 0x00, 0x68, 0xFF, 0x00, 0x00 }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//}