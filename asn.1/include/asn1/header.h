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

#ifndef _ASN1_HEADER_H_
#define _ASN1_HEADER_H_

#include "asn1/tag.h"
#include "asn1/decoder.h"
#include <vector>

namespace asn1
{
	//todo: is it needed
	struct Header
	{
		Tag tag;
		uint64_t content_length{ 0 };

		Header(const Tag& tag, uint64_t length)
			: tag(tag), content_length(length)
		{}
	};

	class HeaderDecoder
	{
	private:
		enum class State : uint8_t
		{
			INITIAL,
			END_CONTENT_DEFINITION,
			IDENTIFIER_DEFINITION,
			LENGTH_MODE_DEFINITION,
			LENGTH_DEFINITION,
			COMPLETED
		};

		State state_{ State::INITIAL };
		length_type content_length_{ INDEFINITE_CONTENT_LENGTH };
		length_type length_bytes_{ 0 }; // the number bytes of length
		Tag tag_;

		std::vector<byte_type> buffer_; // cache header data (for collections)

	public:
		HeaderDecoder()
		{
			buffer_.reserve(10);
		}

		HeaderDecoder(const HeaderDecoder&) = default;
		HeaderDecoder& operator=(const HeaderDecoder&) = default;

		HeaderDecoder(HeaderDecoder&& val)
			: state_(val.state_),
			content_length_(val.content_length_),
			length_bytes_(val.length_bytes_),
			tag_(std::move(val.tag_)),
			buffer_(std::move(val.buffer_))
		{
			val.content_length_ = INDEFINITE_CONTENT_LENGTH;
			val.length_bytes_ = 0;
			val.state_ = State::COMPLETED;
		}

		const Tag& tag() const { return tag_; }

		length_type content_length() const { return content_length_; }

		const byte_type* header_data() const { return buffer_.empty() ? nullptr : buffer_.data(); }
		size_t header_size() const { return buffer_.size(); }

		bool finished() const { return state_ == State::COMPLETED; }

		void reset_state()
		{
			tag_ = Tag();
			content_length_ = INDEFINITE_CONTENT_LENGTH;
			length_bytes_ = 0;
			state_ = State::INITIAL;
			buffer_.clear();
		}

		DecodingResult decode(const byte_type* data, size_type size);
	};
}

#endif