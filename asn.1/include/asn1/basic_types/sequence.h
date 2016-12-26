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

#ifndef _ASN1_SEQUENCE_H_
#define _ASN1_SEQUENCE_H_

#include "asn1/basic_types/collection.h"

namespace asn1
{
	template<typename value_type>
	struct SequenceOf : CollectionOf<value_type>
	{
		SequenceOf()
			: SequenceOf(SEQUENCE_TAG)
		{}

		SequenceOf(const Tag& tag)
			: CollectionOf<value_type>(tag)
		{}
	};

	namespace details
	{
		class SequenceTypeDecoder : public details::CollectionDecoder
		{
		private:
			size_t index_{ 0 };

		public:
			SequenceTypeDecoder()
				: SequenceTypeDecoder(SEQUENCE_TAG)
			{}

			SequenceTypeDecoder(const Tag& tag)
				: details::CollectionDecoder(tag)
			{}

			virtual void reset_state() override
			{
				details::CollectionDecoder::reset_state();
				index_ = 0;
			}

		protected:
			BerDecoder* get_next_decoder(const Tag& tag) const override
			{
				const auto& decoders = get_decoders();
				return decoders[index_];
			}

			void mark_as_processed(BerDecoder* decoder) override { ++index_; }
			bool is_all_proccessed() const override { return index_ >= get_decoders().size(); }
		};

		template<typename value_type>
		class SequenceBasedTypeDecoder : public StructuredDecoder<SequenceTypeDecoder, value_type>
		{
		public:
			SequenceBasedTypeDecoder(const Tag& tag, IValueEventHandler* const event_handler)
				: StructuredDecoder<SequenceTypeDecoder, value_type>(tag, event_handler)
			{}

			virtual ~SequenceBasedTypeDecoder() {}
		};
	}

	class SequenceDecoder : public details::SequenceTypeDecoder
	{
	private:
		IStructuredValueEventHandler* const structured_value_event_handler_;

	public:
		SequenceDecoder(IStructuredValueEventHandler* const event_handler)
			: details::SequenceTypeDecoder(), structured_value_event_handler_(event_handler)
		{}

		SequenceDecoder(const Tag& tag, IStructuredValueEventHandler* const event_handler)
			: details::SequenceTypeDecoder(tag), structured_value_event_handler_(event_handler)
		{}

	protected:
		void notify(const Tag& tag) override
		{
			if (structured_value_event_handler_ != nullptr)
			{
				structured_value_event_handler_->execute(tag);
			}
		}
	};

	template<typename decoder_type, typename value_type>
	class SequenceOfDecoder : public details::SequenceBasedTypeDecoder<SequenceOf<value_type>>
	{
	private:
		decoder_type decoder_;

	public:
		explicit SequenceOfDecoder(IValueEventHandler* const event_handler)
			: SequenceOfDecoder(SEQUENCE_TAG, event_handler)
		{}

		SequenceOfDecoder(const Tag& tag, IValueEventHandler* const event_handler)
			: details::SequenceBasedTypeDecoder<SequenceOf<value_type>>(tag, event_handler)
			, decoder_(&this->decoded_element_event_handler_)
		{
			this->add_repeatable(&decoder_);
		}

		SequenceOfDecoder(const Tag& tag, const Tag& element_decoder_tag, IValueEventHandler* const event_handler)
			: details::SequenceBasedTypeDecoder<SequenceOf<value_type>>(tag, event_handler)
			, decoder_(element_decoder_tag, &this->decoded_element_event_handler_)
		{
			this->add_repeatable(&decoder_);
		}

	protected:
		void on_decode_element(Asn1Value&& val) override
		{
			this->decoded_value_.values.push_back(static_cast<value_type&&>(val));
		}

		void on_decode_element(const Tag&, Asn1Value&&) override {}
	};
}

#endif
