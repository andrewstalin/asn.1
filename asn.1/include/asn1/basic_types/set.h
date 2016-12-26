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

#ifndef _ASN1_SET_H_
#define _ASN1_SET_H_

#include "asn1/basic_types/collection.h"
#include <set>

namespace asn1
{
	template<typename value_type>
	struct SetOf : CollectionOf<value_type>
	{
		SetOf()
			: SetOf(SET_TAG)
		{}

		SetOf(const Tag& tag)
			: CollectionOf<value_type>(tag)
		{}
	};

	namespace details
	{
		class SetTypeDecoder : public details::CollectionDecoder
		{
		private:
			std::set<int> processed_;

		public:
			SetTypeDecoder()
				: SetTypeDecoder(SET_TAG)
			{}

			SetTypeDecoder(const Tag& tag)
				: details::CollectionDecoder(tag)
			{}

			virtual void reset_state() override
			{
				details::CollectionDecoder::reset_state();
				processed_.clear();
			}

		protected:
			BerDecoder* get_next_decoder(const Tag& tag) const override;
			void mark_as_processed(BerDecoder* decoder) override;
			bool is_all_proccessed() const override;
		};

		template<typename value_type>
		class SetBasedTypeDecoder : public StructuredDecoder<SetTypeDecoder, value_type>
		{
		public:
			SetBasedTypeDecoder(const Tag& tag, IValueEventHandler* const event_handler)
				: StructuredDecoder<SetTypeDecoder, value_type>(tag, event_handler)
			{}

			virtual ~SetBasedTypeDecoder() {}
		};
	}

	class SetDecoder : public details::SetTypeDecoder
	{
	private:
		IStructuredValueEventHandler* const structured_value_event_handler_;

	public:
		SetDecoder(IStructuredValueEventHandler* const event_handler)
			: details::SetTypeDecoder(SET_TAG), structured_value_event_handler_(event_handler)
		{}

		SetDecoder(const Tag& tag, IStructuredValueEventHandler* const event_handler)
			: details::SetTypeDecoder(tag), structured_value_event_handler_(event_handler)
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

	//todo: optional
	template<typename decoder_type, typename value_type>
	class SetOfDecoder : public details::SetBasedTypeDecoder<SetOf<value_type>>
	{
	private:
		decoder_type decoder_;

	public:
		explicit SetOfDecoder(IValueEventHandler* const event_handler)
			: SetOfDecoder(SET_TAG, event_handler)
		{}

		SetOfDecoder(const Tag& tag, IValueEventHandler* const event_handler)
			: details::SetBasedTypeDecoder<SetOf<value_type>>(tag, event_handler)
			, decoder_(&this->decoded_element_event_handler_)
		{
			this->add_repeatable(&decoder_);
		}

		SetOfDecoder(const Tag& tag, const Tag& element_decoder_tag, IValueEventHandler* const event_handler)
			: details::SetBasedTypeDecoder<SetOf<value_type>>(tag, event_handler)
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
