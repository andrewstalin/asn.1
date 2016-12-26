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

//#include "asn1/universal_decoder.h"
//#include "asn1/basic_types/bit_string.h"
//#include "asn1/basic_types/boolean.h"
//#include "asn1/basic_types/integer.h"
//#include "asn1/basic_types/null.h"
//#include "asn1/basic_types/object_identifier.h"
//#include "asn1/basic_types/octet_string.h"
//#include "asn1/basic_types/sequence.h"
//#include "asn1/basic_types/set.h"
//#include "asn1/basic_types/time.h"
//#include "decoding.h"
//#include <cassert>
//
//using namespace asn1;
//
//bool IsString(const Tag& tag)
//{
//	return tag == Tag::OCTET_STRING() 
//		|| tag == Tag::BMP_STRING() 
//		|| tag == Tag::UTF8_STRING() 
//		|| tag == Tag::IA5_STRING() 
//		|| tag == Tag::PRINTABLE_STRING()
//		|| tag == Tag::T61_STRING()
//		|| tag == Tag::NUMERIC_STRING();
//}
//
//void UniversalDecoder::ValueEventHandler::execute(Asn1Value&& val)
//{
//	assert(decoder.collection_);
//	auto& collection = decoder.collection.value();
//
//
//}
//
//DecodingResult UniversalDecoder::decode(const byte_type* data, size_type size)
//{
//	CHECK_PRECONDITIONS(data, size, finished());
//	DecodingResult decoding_result = { 0, DecodingCode::UNDEFINED };
//
//	while (state_ != State::COMPLETED)
//	{
//		switch (state_)
//		{
//		case State::INITIAL:
//			{
//				decoding_result << header_decoder_.decode(data, size);
//				CHECK_RESULT(decoding_result);
//				state_ = State::DECODER_DEFINITION;
//			}
//			break;
//		case State::DECODER_DEFINITION:
//			{
//				const Tag& header_tag = header_decoder_.tag();
//
//				if (header_tag == Tag::BIT_STRING())
//				{
//					decoder_ = std::make_unique<BitStringDecoder>(value_event_handler_);
//				}
//				else if (header_tag == Tag::BOOLEAN())
//				{
//					decoder_ = std::make_unique<BooleanDecoder>(value_event_handler_);
//				}
//				else if (header_tag == Tag::INTEGER())
//				{
//					decoder_ = std::make_unique<IntegerDecoder>(value_event_handler_);
//				}
//				else if (header_tag == Tag::NULL_VALUE())
//				{
//					decoder_ = std::make_unique<NullDecoder>(value_event_handler_);
//				}
//				else if (header_tag == Tag::OBJECT_IDENTIFIER())
//				{
//					decoder_ = std::make_unique<ObjectIdentifierDecoder>(value_event_handler_);
//				}
//				else if (header_tag == Tag::GENERALIZED_TIME())
//				{
//					decoder_ = std::make_unique<TimeDecoder>(Tag::GENERALIZED_TIME(), value_event_handler_);
//				}
//				else if (header_tag == Tag::UTC_TIME())
//				{
//					decoder_ = std::make_unique<TimeDecoder>(Tag::UTC_TIME(), value_event_handler_);
//				}
//				else if (IsString(header_tag))
//				{
//					decoder_ = std::make_unique<OctetStringDecoder>(header_tag, value_event_handler_);
//				}
//				else if (header_tag == SEQUENCE_TAG)
//				{
//				
//					collection_values_decoder_ = std::make_unique<UniversalDecoder>(&collection_value_event_handler_);
//					decoder_ = std::make_unique<SequenceDecoder>();
//					static_cast<SequenceDecoder*>(decoder_.get());
//				}
//				else if (header_tag == SET_TAG)
//				{
//					collection_.emplace(header_tag);
//					decoder_ = std::make_unique<SetDecoder>([this]()
//					{
//						notify(std::move(collection_.value()));
//					});
//					static_cast<SetDecoder*>(decoder_.get())->add_repeatable(UniversalDecoder([this](IAsn1Value&& val)
//					{
//						collection_.value().elements.push_back(std::move(val));
//					}));
//				}
//				else
//				{
//					throw Asn1Exception(errors::UNKNOWN_ASN1_VALUE);
//				}
//
//				decoder_->decode(header_decoder_.header_data(), header_decoder_.header_size());
//				state_ = State::DECODE_VALUE;
//			}
//			break;
//		case State::DECODE_VALUE:
//			{
//				assert(decoder_ != nullptr);
//				decoding_result << decoder_->decode(data + decoding_result.read_bytes, size - decoding_result.read_bytes);
//
//				CHECK_RESULT(decoding_result);
//			
//				state_ = State::COMPLETED;
//			}
//			break;
//		}
//	}
//
//	return decoding_result;
//}