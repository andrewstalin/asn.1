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

#ifndef _ASN1_INTEGER_H_
#define _ASN1_INTEGER_H_

#include "asn1/header.h"
#include <vector>
#include <string>

namespace asn1
{
	struct Integer : Asn1Value
	{
		std::vector<byte_type> bytes;

		Integer()
			: Asn1Value(INTEGER_TAG)
		{}

		Integer(const Tag& tag)
			: Asn1Value(tag)
		{}

		Integer(int64_t val)
			: Integer(val, INTEGER_TAG)
		{}

		Integer(int64_t val, const Tag& tag);

		Integer& operator=(int64_t val);

		int64_t to_int64() const;
		std::string to_hex_string() const;

		length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
		{
			return encoder.encode_integer(writer, tag, bytes);
		}
	};

	class IntegerDecoder : public BerDecoder
	{
	private:
		enum class State : uint8_t
		{
			INITIAL,
			BYTES_DEFINITION,
			COMPLETED
		};

		HeaderDecoder header_decoder_;
		Integer value_;
		IValueEventHandler* const value_event_handler_;
		uint64_t content_length_{ 0 };
		State state_{ State::INITIAL };

	public:
		explicit IntegerDecoder(IValueEventHandler* const event_handler)
			: IntegerDecoder(INTEGER_TAG, event_handler)
		{}

		IntegerDecoder(const Tag& tag, IValueEventHandler* const event_handler)
			: BerDecoder(tag), value_event_handler_(event_handler)
		{}

		bool finished() const override { return state_ == State::COMPLETED; }

		void reset_state() override
		{
			state_ = State::INITIAL;
			header_decoder_.reset_state();
			content_length_ = 0;
			value_ = Integer();
		}

		DecodingResult decode(const byte_type* data, size_type size) override;
	};
}
#endif