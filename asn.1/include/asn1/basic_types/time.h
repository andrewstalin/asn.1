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

#ifndef _ASN1_TIME_H_
#define _ASN1_TIME_H_

#include "asn1/basic_types/octet_string.h"
#include "asn1/basic_types/choice.h"

namespace asn1
{
	//todo: refactor parse
	//todo: refactor ctr
	//todo: validate undefined
	struct Time : Asn1Value
	{
		enum class TimeType : uint8_t
		{
			UTC,
			GENERALIZED,
			UNDEFINED
		};

		uint64_t fraction{ 0 };
		int16_t offset{ 0 };

		uint16_t year{ 0 };
		uint8_t month{ 0 };
		uint8_t day{ 0 };
		uint8_t hour{ 0 };
		uint8_t minute{ 0 };
		uint8_t second{ 0 };

		TimeType type;
		bool utc{ false };

		Time()
			: Time(TimeType::UNDEFINED, UNDEFINED_TAG)
		{}

		Time(TimeType type, const Tag& tag)
			: Asn1Value(tag), type(type)
		{}

		Time(TimeType type, const Tag& tag, uint16_t year, uint8_t month, uint8_t day, int16_t offset)
			: Time(type, tag, year, month, day, 0, 0, 0, offset)
		{}

		Time(TimeType type, const Tag& tag, uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, int16_t offset)
			: Asn1Value(tag)
			, offset(offset)
			, year(year)
			, month(month)
			, day(day)
			, hour(hour)
			, minute(minute)
			, second(second)
			, type(type)
			, utc(type == TimeType::UTC ? true : false)
		{}

		static Time parse(const Tag& tag, const byte_type* data, length_type size);

		length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
		{
			return type == Time::TimeType::UTC ? encoder.encode_utc_time(writer, tag, *this) : encoder.encode_generalized_time(writer, tag, *this);
		}

		std::string to_string() const;
	};

	class TimeDecoder : public BerDecoder
	{
	private:
		struct TimeValueEventHandler : IValueEventHandler
		{
			IValueEventHandler* const event_handler;

			TimeValueEventHandler(IValueEventHandler* const event_handler)
				: event_handler(event_handler)
			{}

			void execute(Asn1Value&& val) override
			{
				if (event_handler != nullptr)
				{
					auto& string = static_cast<OctetString&>(val);
					event_handler->execute(Time::parse(val.tag, string.bytes.data(), string.bytes.size()));
				}
			}

			void execute(const Tag&, Asn1Value&&) override {}
		};

		TimeValueEventHandler time_value_event_handler_;
		OctetStringDecoder decoder_;

	public:
		TimeDecoder(const Tag& tag, IValueEventHandler* const event_handler)
			: BerDecoder(tag), time_value_event_handler_(event_handler), decoder_(tag, &time_value_event_handler_)
		{}

		bool finished() const override { return decoder_.finished(); }

		void reset_state() override
		{
			decoder_.reset_state();
		}

		DecodingResult decode(const byte_type* data, size_type size) override
		{
			return decoder_.decode(data, size);
		}
	};

	class TimePairDecoder : protected ChoiceDecoder
	{
	private:
		TimeDecoder utc_time_decoder_;
		TimeDecoder generalized_time_decoder_;

	public:
		explicit TimePairDecoder(IValueEventHandler* const event_handler)
			: utc_time_decoder_(UTC_TIME_TAG, event_handler)
			, generalized_time_decoder_(GENERALIZED_TIME_TAG, event_handler)
		{
			add({
				&utc_time_decoder_,
				&generalized_time_decoder_
			});
		}

		bool finished() const override { return ChoiceDecoder::finished(); }

		void reset_state() override
		{
			ChoiceDecoder::reset_state();
		}

		DecodingResult decode(const byte_type* data, size_type size) override
		{
			return ChoiceDecoder::decode(data, size);
		}
	};
}

#endif
