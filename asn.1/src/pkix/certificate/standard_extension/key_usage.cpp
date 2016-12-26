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

#include "asn1/pkix/certificate/standard_extension/key_usage.h"

asn1::pkix::cert::KeyUsage asn1::pkix::cert::decode_key_usage(const byte_type* data, size_type size)
{
	auto bit_string = asn1::decode<BitString, BitStringDecoder>(data, size);
	THROW_IF(bit_string.bytes.empty(), Asn1Exception(errors::WRONG_STRUCTURE));

	uint16_t result{ 0 };
	auto bits = bit_string.to_string();
	
	if (!bits.empty())
	{
		for (auto i = 0u; i < bits.size(); ++i)
		{
			if (bits[i] == '1')
			{
				result |= static_cast<uint16_t>(1 << i);
			}
		}
	}

	return static_cast<KeyUsage>(result);
}