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

#include "asn1/pkix/primitive/name.h"
#include <sstream>

using namespace asn1;

std::string pkix::Name::to_string() const
{
	std::stringstream ss;

	size_t i{ 0 };

	while (i < values.size())
	{
		ss << values[i++].to_string();

		if (i < values.size())
		{
			ss << ",\r";
		}
	}

	return ss.str();
}

std::vector<std::vector<pkix::RDNItem>> pkix::Name::parts() const
{
	std::vector<std::vector<pkix::RDNItem>> result;
	result.reserve(values.size());

	for (const auto& val : values)
	{
		result.push_back(val.parts());
	}

	return result;
}