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

//#pragma once
//
//#include "asn1/basic_types/any.h"
//#include "asn1/basic_types/bit_string.h"
//#include "asn1/basic_types/boolean.h"
//#include "asn1/basic_types/integer.h"
//#include "asn1/basic_types/object_identifier.h"
//#include "asn1/basic_types/octet_string.h"
//#include "asn1/basic_types/printable_string.h"
//#include "asn1/decoders/any_decoder.h"
//#include "asn1/decoders/boolean_decoder.h"
//#include "asn1/decoders/bit_string_decoder.h"
//#include "asn1/decoders/integer_decoder.h"
//#include "asn1/decoders/object_identifier_decoder.h"
//#include "asn1/decoders/octet_string_decoder.h"
//
//using namespace asn1;
//
//enum class DecoderStatus{ DEFAULT, OPTIONALL, REPEATED, EMPTY_REPEATED, EMPTY_OPTIONAL };
//
//struct BaseTestDecoder
//{
//	DecoderStatus decoder_status{ DecoderStatus::DEFAULT };
//
//	virtual void check_value(std::unique_ptr<IAsn1Value> val, int index) const = 0;
//
//	virtual std::unique_ptr<IBerDecoder> get_decoder(int& fire) const = 0;
//
//	virtual int size() const = 0;
//
//	BaseTestDecoder(DecoderStatus status)
//		: decoder_status(status)
//	{
//	}
//
//protected:
//	std::function<void(std::unique_ptr<IAsn1Value> val)> get_handler(int& fire) const
//	{
//		switch (decoder_status)
//		{
//		case DecoderStatus::OPTIONALL:
//		case DecoderStatus::DEFAULT:
//			return [&fire, this](std::unique_ptr<IAsn1Value> val)
//			{
//				check_value(std::move(val), fire); ++fire;
//			};
//			break;
//		case DecoderStatus::REPEATED:
//			return [&fire, this](std::unique_ptr<IAsn1Value> val)
//			{
//				check_value(std::move(val), fire); ++fire;
//			};
//			break;
//		case DecoderStatus::EMPTY_OPTIONAL:
//		case DecoderStatus::EMPTY_REPEATED:
//			return [&fire](std::unique_ptr<IAsn1Value> val)
//			{
//				--fire;
//			};
//			break;
//		default:
//			return [](std::unique_ptr<IAsn1Value> val)
//			{
//				EXPECT_TRUE(false);
//			};
//			break;
//		}
//	}
//};
//
//using BaseTestDecoderPtr = std::shared_ptr<BaseTestDecoder>;
//
//template<typename T, typename E>
//struct TestDecoderValue : public BaseTestDecoder
//{
//	std::vector<T> expected_values;
//
//	explicit TestDecoderValue(DecoderStatus status = DecoderStatus::DEFAULT)
//		: BaseTestDecoder(status)
//	{
//	}
//
//	explicit TestDecoderValue(const T& value, DecoderStatus status = DecoderStatus::DEFAULT)
//		: BaseTestDecoder(status)
//	{
//		expected_values.push_back(value);
//	}
//
//	std::unique_ptr<IBerDecoder> get_decoder(int& fire) const override
//	{
//		return std::make_unique<E>(get_handler(fire));
//	}
//
//	virtual void check_value(std::unique_ptr<IAsn1Value> val, int index) const override
//	{
//		auto actual = get_value(std::move(val));
//		EXPECT_EQ(expected_values.at(index), actual);
//	}
//
//	static T get_value(std::unique_ptr<IAsn1Value> val)
//	{
//		return T();
//	}
//
//	int size() const override
//	{
//		return expected_values.size();
//	}
//};
//
//using AnyTestDecoder = TestDecoderValue<std::vector<uint8_t>, AnyDecoder>;
//using BitStringTestDecoder = TestDecoderValue<std::string, BitStringDecoder>;
//using BooleanTestDecoder = TestDecoderValue<bool, BooleanDecoder>;
//using IntegerTestDecoder = TestDecoderValue<int64_t, IntegerDecoder>;
//using ObjectIdentifierTestDecoder = TestDecoderValue<std::string, ObjectIdentifierDecoder>;
//using OctetStringTestDecoder = TestDecoderValue<std::vector<uint8_t>, OctetStringDecoder>;
//class PrintableStringDecoder {};
//using PrintableStringTestDecoder = TestDecoderValue<std::string, PrintableStringDecoder>;
//
//template<>
//std::vector<uint8_t> AnyTestDecoder::get_value(std::unique_ptr<IAsn1Value> val)
//{
//	auto any = static_cast<Any*>(val.get());
//	return any->bytes;
//}
//
//template<>
//std::string BitStringTestDecoder::get_value(std::unique_ptr<IAsn1Value> val)
//{
//	auto bit_string = static_cast<BitString*>(val.get());
//	return bit_string->to_string();
//}
//
//template<>
//bool BooleanTestDecoder::get_value(std::unique_ptr<IAsn1Value> val)
//{
//	auto boolean = static_cast<Boolean*>(val.get());
//	return boolean->value;
//}
//
//template<>
//int64_t IntegerTestDecoder::get_value(std::unique_ptr<IAsn1Value> val)
//{
//	auto integer = static_cast<Integer*>(val.get());
//	return integer->to_int64();
//}
//
//template<>
//std::string ObjectIdentifierTestDecoder::get_value(std::unique_ptr<IAsn1Value> val)
//{
//	auto object_identifier = static_cast<ObjectIdentifier*>(val.get());
//	return object_identifier->to_string();
//}
//
//template<>
//std::vector<uint8_t> OctetStringTestDecoder::get_value(std::unique_ptr<IAsn1Value> val)
//{
//	auto octet_string = static_cast<OctetString*>(val.get());
//	return octet_string->bytes;
//}
//
//template<>
//std::string PrintableStringTestDecoder::get_value(std::unique_ptr<IAsn1Value> val)
//{
//	auto printable_string = static_cast<PrintableString*>(val.get());
//	return printable_string->to_string();
//}
//
//template<>
//std::unique_ptr<IBerDecoder> PrintableStringTestDecoder::get_decoder(int& fire) const
//{
//	return std::make_unique<OctetStringDecoder>(Tag::printable_string(), get_handler(fire));
//}
//
//template<typename T>
//void collection_test(const std::vector<uint8_t>& data, const std::vector<BaseTestDecoderPtr>& decoders)
//{
//	int success = 0;
//
//	T collection_decoder([&success](std::unique_ptr<IAsn1Value>& val)
//	{
//		++success;
//	});
//
//	std::vector<int> fire(decoders.size(), 0);
//
//	for (auto i = 0u; i < decoders.size(); ++i)
//	{
//		auto decoder = decoders.at(i);
//
//		switch (decoder->decoder_status)
//		{
//		case DecoderStatus::DEFAULT:
//			collection_decoder.add(decoder->get_decoder(fire.at(i)));
//			break;
//		case DecoderStatus::EMPTY_OPTIONAL:
//		case DecoderStatus::OPTIONALL:
//			collection_decoder.add_optional(decoder->get_decoder(fire.at(i)));
//			break;
//		case DecoderStatus::EMPTY_REPEATED:
//		case DecoderStatus::REPEATED:
//			collection_decoder.add_repeatable(decoder->get_decoder(fire.at(i)));
//			break;
//		default:
//			EXPECT_TRUE(false);
//			break;
//		}
//	}
//
//	TestDecoder(&collection_decoder, data);
//
//	for (auto i = 0u; i < decoders.size(); ++i)
//	{
//		EXPECT_EQ(decoders.at(i)->size(), fire.at(i));
//	}
//
//	EXPECT_EQ(success, 1);
//}