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

//#include "tests.h"
//#include "value_testers.h"
//#include "asn1/decoders/set_decoder.h"
//#include "asn1/decoders/octet_string_decoder.h"
//#include "asn1/decoders/object_identifier_decoder.h"
//#include "asn1/decoders/integer_decoder.h"
//
//using namespace asn1;
////
////TEST(set_case, encode_test)
////{
////	std::vector<uint8_t> data = { 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 };
////	io::BufferedWriter writer;
////	Set set;
////	set.add(std::make_shared<OctetString>(data.data(), data.size()));
////	set.add(std::make_shared<Integer>(Integer::create(32533421)));
////	set.encode(writer);
////	compare({ 0x31, 0x0E, 0x04, 0x06, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47, 0x02, 0x04, 0x01, 0xF0, 0x6B, 0xAD }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
////}
////
////void decodeSetTest(const std::vector<std::vector<uint8_t>>& data)
////{
////	SetDecoder decoder;
////	decoder.add(std::make_unique<PrintableStringDecoder>());
////
////	auto repeatableDecoder = std::make_unique<IntegerDecoder>();
////	repeatableDecoder->set_repeatable(true);
////	decoder.add(std::move(repeatableDecoder));
////
////	decoder.add(std::make_unique<UtcTimeDecoder>());
////
////	auto optionalDecoder = std::make_unique<OctetStringDecoder>();
////	optionalDecoder->set_optional(true);
////	decoder.add(std::move(optionalDecoder));
////
////	checkDecodingResult(data[0].size(), DecodingCode::MORE_DATA, decoder.decode(data[0].data(), data[0].size()));
////	checkDecodingResult(data[1].size(), DecodingCode::MORE_DATA, decoder.decode(data[1].data(), data[1].size()));
////	checkDecodingResult(data[2].size(), DecodingCode::MORE_DATA, decoder.decode(data[2].data(), data[2].size()));
////	checkDecodingResult(data[3].size(), DecodingCode::MORE_DATA, decoder.decode(data[3].data(), data[3].size()));
////	checkDecodingResult(data[4].size(), DecodingCode::OK, decoder.decode(data[4].data(), data[4].size()));
////	auto value = std::static_pointer_cast<Set>(decoder.value());
////	EXPECT_EQ(4, value->size());
////
////	EXPECT_TRUE(value->at(0)->value != nullptr);
////	EXPECT_EQ(12345, static_cast<int64_t>(*std::static_pointer_cast<Integer>(value->at(0)->value)));
////
////	EXPECT_TRUE(value->at(1)->value != nullptr);
////	EXPECT_EQ(std::string("hello fellow"), std::static_pointer_cast<PrintableString>(value->at(1)->value)->toString());
////
////	EXPECT_TRUE(value->at(2)->value != nullptr);
////	EXPECT_EQ(67890, static_cast<int64_t>(*std::static_pointer_cast<Integer>(value->at(2)->value)));
////
////	EXPECT_TRUE(value->at(3)->value != nullptr);
////	auto utc = std::static_pointer_cast<UtcTime>(value->at(3)->value);
////	EXPECT_EQ(76, utc->year());
////	EXPECT_EQ(10, utc->month());
////	EXPECT_EQ(21, utc->day());
////	EXPECT_EQ(7, utc->hour());
////	EXPECT_EQ(56, utc->minute());
////	EXPECT_EQ(43, utc->second());
////	EXPECT_EQ(20, utc->offset());
////}
////
////TEST(set_decoder_case, decode_definite_length_test)
////{
////	decodeSetTest({ { 0x31, 0x2A, 0x02, 0x02, 0x30, 0x39 }, { 0x13, 0x0C, 0x68, 0x65, 0x6C }, { 0x6C, 0x6F, 0x20, 0x66, 0x65, 0x6C, 0x6C, 0x6F, 0x77 }, { 0x02, 0x03, 0x01, 0x09, 0x32, 0x17, 0x11, 0x37, 0x36, 0x31, 0x30, 0x32, 0x31, 0x30, 0x37 }, { 0x35, 0x36, 0x34, 0x33, 0x2B, 0x30, 0x30, 0x32, 0x30 } });
////}
////
////TEST(set_decoder_case, decode_undefinite_length_test)
////{
////	decodeSetTest({ { 0x31, 0x80, 0x02, 0x02, 0x30, 0x39 }, { 0x13, 0x0C, 0x68, 0x65, 0x6C }, { 0x6C, 0x6F, 0x20, 0x66, 0x65, 0x6C, 0x6C, 0x6F, 0x77 }, { 0x02, 0x03, 0x01, 0x09, 0x32, 0x17, 0x11, 0x37, 0x36, 0x31, 0x30, 0x32, 0x31, 0x30, 0x37 }, { 0x35, 0x36, 0x34, 0x33, 0x2B, 0x30, 0x30, 0x32, 0x30, 0x00, 0x00 } });
////}
//
//TEST(set_decoder_case, decode_definite_length_test)
//{
//	std::vector<uint8_t> data = { 0x31, 0x1E,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3"));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)"));
//
//	collection_test<SetDecoder>(data, decoders);
//}
//
//TEST(set_decoder_case, decode_definite_length_any_test)
//{
//	std::vector<uint8_t> data = { 0x31, 0x1E,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3"));
//	std::vector<uint8_t> bytes = { 0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29 };
//	decoders.push_back(std::make_shared<AnyTestDecoder>(bytes));
//
//	collection_test<SetDecoder>(data, decoders);
//}
//
//TEST(set_decoder_case, decode_definite_length_optional_test)
//{
//	std::vector<uint8_t> data = { 0x31, 0x1E,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<OctetStringTestDecoder>(DecoderStatus::EMPTY_OPTIONAL));
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3"));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)", DecoderStatus::OPTIONALL));
//	decoders.push_back(std::make_shared<BooleanTestDecoder>(DecoderStatus::EMPTY_OPTIONAL));
//
//	collection_test<SetDecoder>(data, decoders);
//}
//
//TEST(set_decoder_case, decode_definite_length_repeatable_test)
//{
//	std::vector<uint8_t> data = { 0x31, 0x23,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x06, 0x03, 0x55, 0x04, 0x04,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<BooleanTestDecoder>(DecoderStatus::EMPTY_REPEATED));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)"));
//	auto first = std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3", DecoderStatus::REPEATED);
//	first->expected_values.push_back("2.5.4.4");
//	decoders.push_back(first);
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	decoders.push_back(std::make_shared<OctetStringTestDecoder>(DecoderStatus::EMPTY_REPEATED));
//
//	collection_test<SetDecoder>(data, decoders);
//}
//
//TEST(set_decoder_case, decode_indefinite_length_test)
//{
//	std::vector<uint8_t> data = { 0x31, 0x80,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A, 0x00, 0x00
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3"));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)"));
//
//	collection_test<SetDecoder>(data, decoders);
//}
//
//TEST(set_decoder_case, decode_indefinite_length_optional_test)
//{
//	std::vector<uint8_t> data = { 0x31, 0x80,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A, 0x00, 0x00
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<OctetStringTestDecoder>(DecoderStatus::EMPTY_OPTIONAL));
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3"));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)", DecoderStatus::OPTIONALL));
//	decoders.push_back(std::make_shared<BooleanTestDecoder>(DecoderStatus::EMPTY_OPTIONAL));
//
//	collection_test<SetDecoder>(data, decoders);
//}
//
//TEST(set_decoder_case, decode_indefinite_length_repeatable_test)
//{
//	std::vector<uint8_t> data = { 0x31, 0x80,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x06, 0x03, 0x55, 0x04, 0x04,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A, 0x00, 0x00
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<BooleanTestDecoder>(DecoderStatus::EMPTY_REPEATED));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)"));
//	auto first = std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3", DecoderStatus::REPEATED);
//	first->expected_values.push_back("2.5.4.4");
//	decoders.push_back(first);
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	decoders.push_back(std::make_shared<OctetStringTestDecoder>(DecoderStatus::EMPTY_REPEATED));
//
//	collection_test<SetDecoder>(data, decoders);
//}
//
//TEST(set_decoder_case, decode_indefinite_length_any_test)
//{
//	std::vector<uint8_t> data = { 0x31, 0x80,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A, 0x00, 0x00
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	std::vector<uint8_t> bytes = { 0x06, 0x03, 0x55, 0x04, 0x03 };
//	decoders.push_back(std::make_shared<AnyTestDecoder>(bytes));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)"));
//
//	collection_test<SetDecoder>(data, decoders);
//}
