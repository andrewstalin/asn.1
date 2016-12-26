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

#ifndef _ASN1_DECODER_H_
#define _ASN1_DECODER_H_

#include "asn1/type_traits.h"
#include "asn1/tag.h"
#include "asn1/value.h"

namespace asn1
{
	struct IStructuredValueEventHandler
	{
		virtual void execute(const Tag& tag) = 0;
	};

	struct IValueEventHandler
	{
		virtual void execute(Asn1Value&&) = 0;
		virtual void execute(const Tag&, Asn1Value&&) = 0;
	};

	struct IDataEventHandler
	{
		virtual void execute(const byte_type*, size_type) = 0;
	};

	namespace details
	{
		template<typename value_type>
		struct ValueEventHandler : IValueEventHandler
		{
			value_type value;

			void execute(Asn1Value&& val) override
			{
				value = static_cast<value_type&&>(val);
			}

			void execute(const Tag& tag, Asn1Value&& val) override
			{
				value = static_cast<value_type&&>(val);
			}
		};
	}

	enum class DecodingCode : uint8_t
	{
		OK,
		MORE_DATA,
		END_CONTENT,
		UNDEFINED
	};

	struct DecodingResult
	{
		size_type read_bytes{ 0 };
		DecodingCode code{ DecodingCode::UNDEFINED };

		DecodingResult()
		{}

		DecodingResult(size_type read, DecodingCode code)
			: read_bytes(read), code(code)
		{}

		DecodingResult& operator<<(const DecodingResult& dr)
		{
			code = dr.code;
			read_bytes += dr.read_bytes;
			return *this;
		}
	};

	struct BerDecoder
	{
	private:
		bool optional_{ false };
		bool repeatable_{ false };
		Tag tag_;
		//std::shared_ptr<IAsn1Value> default_value_;

	public:
		BerDecoder(const Tag& tag)
			: tag_(tag)
		{}

		virtual ~BerDecoder() {}

		const Tag& tag() const { return tag_; }

		void set_optional(bool val) { optional_ = val; }
		bool optional() const { return optional_; }

		void set_repeatable(bool val) { repeatable_ = val; }
		bool repeatable() const { return repeatable_; }

		virtual bool finished() const = 0;
		virtual void reset_state() = 0;
		virtual DecodingResult decode(const byte_type* data, size_type size) = 0;

	protected:
		void on_decoding_completion(IValueEventHandler* event_handler, Asn1Value&& val) const
		{
			if (event_handler != nullptr)
			{
				event_handler->execute(std::move(val));
			}
		}
	};

	template<typename value_type, typename decoder_type>
	value_type decode(const byte_type* data, size_type size)
	{
		details::ValueEventHandler<value_type> event_handler;

		decoder_type decoder(&event_handler);
		auto decoding_result = decoder.decode(data, size);
		THROW_IF(decoding_result.code != DecodingCode::OK, Asn1Exception(errors::DECODING_ERROR));

		return std::move(event_handler.value);
	}

	template<typename value_type, typename decoder_type>
	value_type decode(const Tag& tag, const byte_type* data, size_type size)
	{
		details::ValueEventHandler<value_type> event_handler;

		decoder_type decoder(tag, &event_handler);
		auto decoding_result = decoder.decode(data, size);
		THROW_IF(decoding_result.code != DecodingCode::OK, Asn1Exception(errors::DECODING_ERROR));

		return std::move(event_handler.value);
	}
}

#endif