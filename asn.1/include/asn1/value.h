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

#ifndef _ASN1_VALUE_H_
#define _ASN1_VALUE_H_

#include "asn1/tag.h"
#include "asn1/io/writer.h"
#include "asn1/encoders/encoder.h"

namespace asn1
{
	enum class SPECIAL_NUMBERS : uint8_t
	{ 
		PLUS_INFINITY, 
		MINUS_INFINITY, 
		NOT_A_NUMBER, 
		MINUS_ZERO, 
		UNKNOWN 
	};

	struct Asn1Value
	{
		Tag tag;

		Asn1Value(const Tag& tag)
			: tag(tag)
		{}

		Asn1Value(const Asn1Value&) = default;
		Asn1Value& operator=(const Asn1Value&) = default;

		Asn1Value(Asn1Value&&) = default;
		Asn1Value& operator=(Asn1Value&&) = default;

		virtual ~Asn1Value() {}

		virtual length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const = 0;
	};

	class StreamingAsn1Value
	{
	private:
		enum class State : uint8_t
		{
			INITIAL,
			ENCODING,
			FINAL
		};

		Tag tag_;
		State state_{ State::INITIAL };
		bool undefined_length_{ false };

	public:
		StreamingAsn1Value(const Tag& tag)
			: tag_(tag)
		{}

		StreamingAsn1Value(const StreamingAsn1Value&) = default;
		StreamingAsn1Value& operator=(const StreamingAsn1Value&) = default;

		StreamingAsn1Value(StreamingAsn1Value&&) = default;
		StreamingAsn1Value& operator=(StreamingAsn1Value&&) = default;

		virtual ~StreamingAsn1Value() {}

		const Tag& tag() const { return tag_; }

		length_type evaluate_size(const IEncoder& encoder) const
		{
			auto content_length = evaluate_content_size(encoder);

			if (content_length != INDEFINITE_CONTENT_LENGTH)
			{
				io::NullWriter null_writer;
				content_length += encoder.encode_header(null_writer, tag_, content_length);
			}

			return content_length;
		}

		virtual length_type evaluate_content_size(const IEncoder& encoder) const = 0;

		length_type encode(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer)
		{
			length_type count{ 0 };

			switch (state_)
			{
			case State::INITIAL:
				count += encode_first(encoder, writer);
				state_ = State::ENCODING;
			case State::ENCODING:
				count += encode_part(encoder, data, size, writer);
				break;
			case State::FINAL:
				break;
			}

			return count;
		}

		length_type encode_final(const IEncoder& encoder, io::IAsn1Writer& writer)
		{
			length_type count{ 0 };

			switch (state_)
			{
			case State::INITIAL:
				count += encode_first(encoder, writer);
			case State::ENCODING:
				count += encode_last(encoder, writer);
				state_ = State::FINAL;
				break;
			case State::FINAL:
				break;
			}

			return count;
		}

		virtual void reset_state() { state_ = State::INITIAL; }

	protected:
		virtual Tag get_encoded_tag() { return tag_; }

		virtual length_type encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer) = 0;
		virtual length_type encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer) = 0;
		virtual length_type encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer) = 0;

	private:
		length_type encode_first(const IEncoder& encoder, io::IAsn1Writer& writer)
		{
			auto content_length = evaluate_content_size(encoder);

			if (content_length == INDEFINITE_CONTENT_LENGTH)
			{
				undefined_length_ = true;
			}

			auto count = encoder.encode_header(writer, get_encoded_tag(), content_length);
			count += encode_first_part(encoder, writer);
			return count;
		}

		length_type encode_last(const IEncoder& encoder, io::IAsn1Writer& writer)
		{
			auto count = encode_last_part(encoder, writer);

			if (undefined_length_)
			{
				count += 2;
				writer.put(0);
				writer.put(0);
			}

			return count;
		}

	};
}

#endif