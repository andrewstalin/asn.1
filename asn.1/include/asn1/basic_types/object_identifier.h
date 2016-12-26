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

#ifndef _ASN1_OBJECT_IDENTIFIER_H_
#define _ASN1_OBJECT_IDENTIFIER_H_

#include "asn1/header.h"
#include <vector>
#include <string>

namespace asn1
{
	struct ObjectIdentifier : Asn1Value
	{
		std::vector<uint64_t> numbers;

		ObjectIdentifier()
			: Asn1Value(OBJECT_IDENTIFIER_TAG)
		{}

		ObjectIdentifier(const Tag& tag)
			: Asn1Value(tag)
		{}

		static ObjectIdentifier parse(const std::string& oid);
		static bool validate_oid_numbers(const std::vector<uint64_t>& numbers);

		std::string to_string() const;

		length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
	};

	class ObjectIdentifierDecoder : public BerDecoder
	{
	private:
		enum class State : uint8_t
		{
			INITIAL,
			NUMBERS_DEFINITION,
			COMPLETED
		};

		HeaderDecoder header_decoder_;
		ObjectIdentifier value_;
		IValueEventHandler* const value_event_handler_;
		uint64_t number_{ 0 };
		length_type content_length_{ 0 };
		State state_{ State::INITIAL };

	public:
		explicit ObjectIdentifierDecoder(IValueEventHandler* const event_handler)
			: ObjectIdentifierDecoder(OBJECT_IDENTIFIER_TAG, event_handler)
		{}

		ObjectIdentifierDecoder(const Tag& tag, IValueEventHandler* const event_handler)
			: BerDecoder(tag), value_event_handler_(event_handler)
		{}

		bool finished() const override { return state_ == State::COMPLETED; }

		void reset_state() override
		{
			state_ = State::INITIAL;
			number_ = 0;
			header_decoder_.reset_state();
			content_length_ = 0;
			value_ = ObjectIdentifier();
		}

		DecodingResult decode(const byte_type* data, size_type size) override;
	};
}

#endif