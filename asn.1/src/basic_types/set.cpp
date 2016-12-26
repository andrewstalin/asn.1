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

#include "asn1/basic_types/set.h"
#include <cassert>

using namespace asn1;

BerDecoder* details::SetTypeDecoder::get_next_decoder(const Tag& tag) const
{
	const auto& decoders = get_decoders();
	BerDecoder* decoder{ nullptr };

	for (auto i = 0u; i < decoders.size(); ++i)
	{
		auto it = processed_.find(i);

		if (it == processed_.end())
		{
			auto current = decoders[i];

			if (current->tag() == tag)
			{
				return current;
			}
			else if (current->tag() == UNDEFINED_TAG)
			{
				decoder = current;
			}
			else if (decoder == nullptr)
			{
				decoder = current;
			}
		}
	}

	return decoder;
}

void details::SetTypeDecoder::mark_as_processed(BerDecoder* decoder)
{
	const auto& decoders = get_decoders();

	for (auto i = 0u; i < decoders.size(); ++i)
	{
		if (decoders[i] == decoder)
		{
			processed_.insert(i);
			return;
		}
	}

	assert(false);
}

bool details::SetTypeDecoder::is_all_proccessed() const
{
	const auto& decoders = get_decoders();
	auto size = processed_.size();
	assert(size <= decoders.size());
	return size == decoders.size();
}