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

#ifndef _ASN1_BOOLEAN_H_
#define _ASN1_BOOLEAN_H_

#include "asn1/header.h"

namespace asn1
{
	struct Boolean : Asn1Value
	{
		bool value{ false };

		Boolean()
			: Boolean(false)
		{}

		Boolean(bool value)
			: Boolean(BOOLEAN_TAG, value)
		{}

		Boolean(const Tag& tag, bool value)
			: Asn1Value(tag), value(value)
		{}

		length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
		{
			return encoder.encode_boolean(writer, tag, value);
		}
	};

	class BooleanDecoder : public BerDecoder
	{
	private:
		enum class State : uint8_t
		{
			INITIAL,
			VALUE_DEFINITION,
			COMPLETED
		};

		HeaderDecoder header_decoder_;
		IValueEventHandler* const value_event_handler_;
		State state_{ State::INITIAL };

	public:
		explicit BooleanDecoder(IValueEventHandler* const event_handler)
			: BooleanDecoder(BOOLEAN_TAG, event_handler)
		{}

		BooleanDecoder(const Tag& tag, IValueEventHandler* const event_handler)
			: BerDecoder(tag), value_event_handler_(event_handler)
		{}

		bool finished() const override { return state_ == State::COMPLETED; }

		void reset_state() override
		{
			state_ = State::INITIAL;
			header_decoder_.reset_state();
		}

		DecodingResult decode(const byte_type* data, size_type size) override;
	};
}

#endif