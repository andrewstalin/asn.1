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

//#ifndef _ASN1_UNIVERSAL_DECODER_H_
//#define _ASN1_UNIVERSAL_DECODER_H_
//
//#include "asn1/header.h"
//#include "asn1/basic_types/collection.h"
//#include "optional/optional.h"
//#include <memory>
//
//namespace asn1
//{
//	class UniversalDecoder : public BerDecoder
//	{
//	private:
//		enum class State : uint8_t
//		{
//			INITIAL,
//			DECODER_DEFINITION,
//			DECODE_VALUE,
//			COMPLETED
//		};
//
//		struct ValueEventHandler : IValueEventHandler
//		{
//			UniversalDecoder* decoder;
//
//			ValueEventHandler(UniversalDecoder* decoder)
//				: decoder(decoder)
//			{}
//
//			void execute(Asn1Value&& val) override;
//			void execute(const Tag& tag, Asn1Value&& val) override;
//		};
//
//		const IValueEventHandler* value_event_handler_;
//		State state_{ State::INITIAL };
//
//		HeaderDecoder header_decoder_;
//		std::unique_ptr<BerDecoder> decoder_;
//
//		ValueEventHandler collection_value_event_handler_;
//		optional::OptionalValue<UniversalDecoder> collection_values_decoder_;
//		optional::OptionalValue<Collection> collection_;
//
//	public:
//		UniversalDecoder(const IValueEventHandler* event_handler)
//			: BerDecoder(Tag::UNDEFINED())
//			, value_event_handler_(event_handler)
//			, collection_value_event_handler_(this)
//		{}
//
//		bool finished() const override { return state_ == State::COMPLETED; }
//
//		void reset_state() override
//		{
//			state_ = State::INITIAL;
//			header_decoder_.reset_state();
//			collection_.clear();
//			decoder_.reset();
//		}
//
//		DecodingResult decode(const byte_type* data, size_type size) override;
//	};
//}
//
//#endif