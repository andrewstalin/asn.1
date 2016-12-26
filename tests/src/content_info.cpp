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
//#include "asn1/pkcs7/content_info.h"
//#include "asn1/pkcs7/signed_data.h"
//#include "asn1/pkcs7/enveloped_data.h"
//
//using namespace asn1;
//
//TEST(content_info, decode_encode_signed_data_test)
//{
//	Asn1ValueTest<pkcs7::ContentInfoDecoder>(FromFile(L"data/content_info_signed_data_detach"));
//	Asn1ValueTest<pkcs7::ContentInfoDecoder>(FromFile(L"data/content_info_signed_data_attach"));
//	Asn1ValueTest<pkcs7::ContentInfoDecoder>(FromFile(L"data/content_info_signed_data_indefinite_length"));
//}
//
//TEST(content_info, streaming_signed_data_test)
//{
//	std::vector<uint8_t> expected = { 0x56, 0x54, 0x20, 0x69, 0x73, 0x20, 0x6E, 0x6F, 0x74, 0x20, 0x65, 0x6E, 0x61, 0x62, 0x6C, 0x65, 0x64, 0x20, 0x69, 0x6E, 0x20, 0x74, 0x68, 0x65, 0x20, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x2C, 0x20, 0x48, 0x41, 0x58, 0x4D, 0x20, 0x6E, 0x6F, 0x74, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x0A, 0x0D, 0x0A, 0x00, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x20, 0x48, 0x41, 0x58, 0x4D, 0x20, 0x72, 0x65, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x20, 0x31, 0x2E, 0x31, 0x20, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x0A, 0x0D, 0x0A, 0x00, 0x54, 0x68, 0x69, 0x73, 0x20, 0x6C, 0x6F, 0x67, 0x20, 0x63, 0x6F, 0x6C, 0x6C, 0x65, 0x63, 0x74, 0x73, 0x20, 0x72, 0x75, 0x6E, 0x6E, 0x69, 0x6E, 0x67, 0x20, 0x73, 0x74, 0x61, 0x74, 0x75, 0x73, 0x20, 0x6F, 0x66, 0x20, 0x48, 0x41, 0x58, 0x4D, 0x20, 0x64, 0x72, 0x69, 0x76, 0x65, 0x72, 0x2E, 0x0A, 0x0A, 0x0D, 0x0A, 0x00 };
//	Asn1ValueDecodeStreamingTest<pkcs7::ContentInfoDecoder>(FromFile(L"data/content_info_signed_data_attach"), expected);
//}
//
//TEST(content_info, streaming_signed_data_indefinite_length_test)
//{
//	std::vector<uint8_t> expected = { 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x21 };
//	Asn1ValueDecodeStreamingTest<pkcs7::ContentInfoDecoder>(FromFile(L"data/content_info_signed_data_indefinite_length"), expected);
//}