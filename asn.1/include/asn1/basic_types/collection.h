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

#ifndef _ASN1_COLLECTION_H_
#define _ASN1_COLLECTION_H_

#include "asn1/header.h"
#include <vector>
#include <memory>

namespace asn1
{
	struct Collection : Asn1Value
	{
		std::vector<std::unique_ptr<Asn1Value>> values;

		Collection(const Tag& tag)
			: Asn1Value(tag)
		{}

		length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
		{
			std::vector<EncodedValue> encoded_values;
			encoded_values.reserve(values.size());

			for (auto i = 0u; i < values.size(); ++i)
			{
				encoded_values.emplace_back(values[i].get());
			}

			return encoder.encode_collection(writer, tag, encoded_values);
		}
	};

	template<typename value_type>
	struct CollectionOf : Asn1Value
	{
		std::vector<value_type> values;

		CollectionOf(const Tag& tag)
			: Asn1Value(tag)
		{}

		length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
		{
			std::vector<EncodedValue> encoded_values;
			encoded_values.reserve(values.size());

			for (auto i = 0u; i < values.size(); ++i)
			{
				encoded_values.emplace_back(&values[i]);
			}

			return encoder.encode_collection(writer, tag, encoded_values);
		}
	};

	namespace details
	{
		class CollectionDecoder : public BerDecoder
		{
		private:
			enum class State : uint8_t
			{
				INITIAL,
				ELEMENT_HEADER_DEFINITION,
				DECODER_DEFINITION,
				DECODING_ELEMENT,
				COMPLETED
			};

			length_type content_length_{ 0 };
			std::vector<BerDecoder*> decoders_;
			HeaderDecoder header_decoder_;

			BerDecoder* element_decoder_{ nullptr };
			State state_{ State::INITIAL };

		public:
			CollectionDecoder(const Tag& tag)
				: BerDecoder(tag)
			{}

			bool finished() const override { return state_ == State::COMPLETED; }

			virtual void reset_state() override;
			DecodingResult decode(const byte_type* data, size_type size) override;

		protected:
			const std::vector<BerDecoder*>& get_decoders() const { return decoders_; }

			void reserve(size_t count)
			{
				decoders_.reserve(count);
			}

			void add(BerDecoder* decoder)
			{
				decoders_.push_back(decoder);
			}

			void add(const std::initializer_list<BerDecoder*>& list)
			{
				decoders_.insert(decoders_.end(), list.begin(), list.end());
			}

			void add_optional(BerDecoder* decoder)
			{
				decoder->set_optional(true);
				decoders_.push_back(decoder);
			}

			void add_repeatable(BerDecoder* decoder)
			{
				decoder->set_repeatable(true);
				decoders_.push_back(decoder);
			}

			virtual BerDecoder* get_next_decoder(const Tag& tag) const = 0;
			virtual void mark_as_processed(BerDecoder* decoder) = 0;
			virtual bool is_all_proccessed() const = 0;

			virtual void notify(const Tag& tag) = 0;
		};

		template<typename collection_type, typename value_type>
		class StructuredDecoder : public collection_type
		{
		protected:
			struct OnDecodedElementEventHandler : IValueEventHandler
			{
				StructuredDecoder* decoder;

				OnDecodedElementEventHandler(StructuredDecoder* decoder)
					: decoder(decoder)
				{}

				void execute(Asn1Value&& val) override
				{
					decoder->on_decode_element(std::move(val));
				}

				void execute(const Tag& tag, Asn1Value&& val) override
				{
					decoder->on_decode_element(tag, std::move(val));
				}
			};

			value_type decoded_value_;
			OnDecodedElementEventHandler decoded_element_event_handler_;
			IValueEventHandler* const value_event_handler_;

		public:
			StructuredDecoder(const Tag& tag, IValueEventHandler* const event_handler)
				: collection_type(tag)
				, decoded_element_event_handler_(this)
				, value_event_handler_(event_handler)
			{}

			virtual ~StructuredDecoder() {}

			const value_type& decoded_value() const { return decoded_value_; }
 
			virtual void reset_state() override
			{
				collection_type::reset_state();
				decoded_value_ = value_type();
			}

		protected:
			virtual void on_decode_element(Asn1Value&& val) = 0;
			virtual void on_decode_element(const Tag& tag, Asn1Value&& val) = 0;

			void notify(const Tag& tag) override
			{
				if (value_event_handler_ != nullptr)
				{
					decoded_value_.tag = tag;
					value_event_handler_->execute(std::move(decoded_value_));
				}
			}
		};
	}
}

#endif