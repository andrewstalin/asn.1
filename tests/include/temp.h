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

//#pragma once
//
//#include "tests.h"
//#include "asn1/value.h"
//#include "asn1/encoders/encoder.h"
//#include "asn1/io/buffered.h"
//
//std::string GetAsn1(std::unique_ptr<asn1::Asn1Value>& val)
//{
//	asn1::io::BufferedWriter writer;
//	val->encode(asn1::DerEncoder(), writer);
//	auto bytes = std::vector<uint8_t>(writer.data(), writer.data() + writer.size());
//	return ToHextStr(bytes);
//}
//
//void ToFile(std::vector<uint8_t>& bytes, const std::wstring& path)
//{
//	auto handle = ::CreateFile(path.c_str(), FILE_GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, 0, nullptr);
//
//	DWORD written_bytes;
//	::WriteFile(handle, bytes.data(), bytes.size(), &written_bytes, nullptr);
//	::CloseHandle(handle);
//}
//
//void ToFile(std::unique_ptr<asn1::IAsn1Value>& val, const std::wstring& path)
//{
//	asn1::io::BufferedWriter writer;
//	val->encode(asn1::DerEncoder(), writer);
//	auto bytes = std::vector<uint8_t>(writer.data(), writer.data() + writer.size());
//
//	ToFile(bytes, path);
//}