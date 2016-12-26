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
////#include "asn1/basic_types/utc_time.h"
//#include "asn1/decoders/sequence_decoder.h"
//#include "asn1/decoders/octet_string_decoder.h"
//#include "asn1/decoders/object_identifier_decoder.h"
//#include "asn1/decoders/integer_decoder.h"
//
//using namespace asn1;
//
////
////TEST(sequence_case, encode_test)
////{
////	std::vector<uint8_t> data = { 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 };
////	io::BufferedWriter writer;
////	Sequence sequence;
////	sequence.add(std::make_shared<OctetString>(data.data(), data.size()));
////	sequence.add(std::make_shared<Integer>(Integer::create(32533421)));
////	sequence.encode(writer);
////	compare({ 0x30, 0x0E, 0x04, 0x06, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47, 0x02, 0x04, 0x01, 0xF0, 0x6B, 0xAD }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
////}
////
//////TEST(sequence_case, encode_test1)
//////{
//////	std::vector<uint8_t> data = { 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47 };
//////	std::string s = "hello fellow";
//////	io::BufferedWriter writer;
//////	Sequence sequence;
//////	sequence.add(CollectionElement(std::make_unique<PrintableString>(std::vector<uint8_t>(s.begin(), s.end()))));
//////	sequence.add(CollectionElement(std::make_unique<Integer>(Integer::create(12345))));
//////	sequence.add(CollectionElement(std::make_unique<Integer>(Integer::create(67890))));
//////	sequence.add(CollectionElement(std::make_unique<UtcTime>(UtcTime::parse("761021075643+0020"))));
//////	sequence.encode(writer);
//////	auto hex = toHextStr(std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//////	//compare({ 0x30, 0x0E, 0x04, 0x06, 0x67, 0x0D, 0xC7, 0x1A, 0x76, 0x47, 0x02, 0x04, 0x01, 0xF0, 0x6B, 0xAD }, std::vector<uint8_t>(writer.data(), writer.data() + writer.size()));
//////}
//
//TEST(sequence_decoder_case, decode_definite_length_test)
//{
//	std::vector<uint8_t> data = { 0x30, 0x1E,
//		0x06, 0x03, 0x55, 0x04, 0x03, 
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3"));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)"));
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//
//	collection_test<SequenceDecoder>(data, decoders);
//}
//
//TEST(sequence_decoder_case, decode_definite_length_optional_test)
//{
//	std::vector<uint8_t> data = { 0x30, 0x1E,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(DecoderStatus::EMPTY_OPTIONAL));
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3"));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)",  DecoderStatus::OPTIONALL));
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>(DecoderStatus::EMPTY_OPTIONAL));
//
//	collection_test<SequenceDecoder>(data, decoders);
//}
//
//TEST(sequence_decoder_case, decode_definite_length_repeatable_test)
//{
//	std::vector<uint8_t> data = { 0x30, 0x23,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x06, 0x03, 0x55, 0x04, 0x04,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	auto first = std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3", DecoderStatus::REPEATED);
//	first->expected_values.push_back("2.5.4.4");
//	decoders.push_back(first);
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)"));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>(DecoderStatus::EMPTY_REPEATED));
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	decoders.push_back(std::make_shared<OctetStringTestDecoder>(DecoderStatus::EMPTY_REPEATED));
//
//	collection_test<SequenceDecoder>(data, decoders);
//}
//
//TEST(sequence_decoder_case, decode_definite_length_any_test)
//{
//	std::vector<uint8_t> data = { 0x30, 0x1E,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3"));
//	std::vector<uint8_t> bytes = { 0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29 };
//	decoders.push_back(std::make_shared<AnyTestDecoder>(bytes));
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//
//	collection_test<SequenceDecoder>(data, decoders);
//}
//
//TEST(sequence_decoder_case, decode_indefinite_length_test)
//{
//	std::vector<uint8_t> data = { 0x30, 0x80,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A, 0x00, 0x00
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3"));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)"));
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//
//	collection_test<SequenceDecoder>(data, decoders);
//}
//
//TEST(sequence_decoder_case, decode_indefinite_length_optional_test)
//{
//	std::vector<uint8_t> data = { 0x30, 0x80,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A, 0x00, 0x00
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(DecoderStatus::EMPTY_OPTIONAL));
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3"));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)", DecoderStatus::OPTIONALL));
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	decoders.push_back(std::make_shared<ObjectIdentifierTestDecoder>(DecoderStatus::EMPTY_OPTIONAL));
//
//	collection_test<SequenceDecoder>(data, decoders);
//}
//
//TEST(sequence_decoder_case, decode_indefinite_length_repeatable_test)
//{
//	std::vector<uint8_t> data = { 0x30, 0x80,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x06, 0x03, 0x55, 0x04, 0x04,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A, 0x00, 0x00
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	auto first = std::make_shared<ObjectIdentifierTestDecoder>("2.5.4.3", DecoderStatus::REPEATED);
//	first->expected_values.push_back("2.5.4.4");
//	decoders.push_back(first);
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)"));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>(DecoderStatus::EMPTY_REPEATED));
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//	decoders.push_back(std::make_shared<OctetStringTestDecoder>(DecoderStatus::EMPTY_REPEATED));
//
//	collection_test<SequenceDecoder>(data, decoders);
//}
//
//TEST(sequence_decoder_case, decode_indefinite_length_any_test)
//{
//	std::vector<uint8_t> data = { 0x30, 0x80,
//		0x06, 0x03, 0x55, 0x04, 0x03,
//		0x13, 0x14, 0x55, 0x43, 0x20, 0x53, 0x4B, 0x42, 0x20, 0x4B, 0x6F, 0x6E, 0x74, 0x75, 0x72, 0x20, 0x28, 0x54, 0x65, 0x73, 0x74, 0x29,
//		0x02, 0x01, 0x0A, 0x00, 0x00
//	};
//
//	std::vector<BaseTestDecoderPtr> decoders;
//	std::vector<uint8_t> bytes = { 0x06, 0x03, 0x55, 0x04, 0x03 };
//	decoders.push_back(std::make_shared<AnyTestDecoder>(bytes));
//	decoders.push_back(std::make_shared<PrintableStringTestDecoder>("UC SKB Kontur (Test)"));
//	decoders.push_back(std::make_shared<IntegerTestDecoder>(10));
//
//	collection_test<SequenceDecoder>(data, decoders);
//}