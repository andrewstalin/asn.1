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

#ifndef _ASN1_CHOICE_H_
#define _ASN1_CHOICE_H_

#include "asn1/header.h"
#include <memory>

namespace asn1
{
	class ChoiceDecoder : public BerDecoder
	{
	private:
		enum class State : uint8_t
		{
			INITIAL,
			DECODER_DEFINITION,
			DECODE_VALUE,
			COMPLETED
		};

		HeaderDecoder header_decoder_;
		std::vector<BerDecoder*> choices_;
		BerDecoder* decoder_{ nullptr };
		State state_{ State::INITIAL };

	public:
		ChoiceDecoder()
			: BerDecoder(UNDEFINED_TAG)
		{}

		void add(BerDecoder* decoder)
		{
			choices_.push_back(decoder);
		}

		void add(const std::initializer_list<BerDecoder*>& list)
		{
			choices_.insert(choices_.end(), list.begin(), list.end());
		}

		bool finished() const override { return state_ == State::COMPLETED; }

		void reset_state() override
		{
			state_ = State::INITIAL;
			header_decoder_.reset_state();
			decoder_ = nullptr;

			for (auto& d : choices_)
			{
				d->reset_state();
			}
		}

		DecodingResult decode(const byte_type* data, size_type size) override;
	};
}

#endif