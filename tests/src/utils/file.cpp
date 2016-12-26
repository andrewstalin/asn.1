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

#include "file.h"
#include "iostreams/file.h"

std::vector<asn1::byte_type> asn1::test::load_file(const char* path)
{
#ifdef _WIN32
	auto file = iostreams::FileStream<byte_type>::open(path, iostreams::FileAccess::READ, iostreams::FileMode::F_OPEN_EXISTING, iostreams::FileShare::SHARE_READ);
#else
	auto file = iostreams::FileStream<byte_type>::open(path, iostreams::FileAccess::READ, iostreams::FileMode::F_OPEN_EXISTING, iostreams::FileShare::NONE);
#endif
	return file. template read_all<std::vector<byte_type>>();
}