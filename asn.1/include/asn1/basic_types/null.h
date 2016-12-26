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

#ifndef _ASN1_NULL_H_
#define _ASN1_NULL_H_

#include "asn1/header.h"

namespace asn1
{
	struct Null : Asn1Value
	{
		Null()
			: Asn1Value(NULL_VALUE_TAG)
		{}

		Null(const Tag& tag)
			: Asn1Value(tag)
		{}

		length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
		{
			return encoder.encode_null(writer, tag);
		}
	};

	class NullDecoder : public BerDecoder
	{
	private:
		enum class State : uint8_t
		{
			INITIAL,
			COMPLETED
		};

		HeaderDecoder header_decoder_;
		State state_{ State::INITIAL };
		IValueEventHandler* const value_event_handler_;

	public:
		explicit NullDecoder(IValueEventHandler* const event_handler)
			: NullDecoder(NULL_VALUE_TAG, event_handler)
		{}

		NullDecoder(const Tag& tag, IValueEventHandler* const event_handler)
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