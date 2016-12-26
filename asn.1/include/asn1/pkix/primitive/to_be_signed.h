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

#ifndef _ASN1_PKIX_SIGNED_H_
#define _ASN1_PKIX_SIGNED_H_

#include "asn1/basic_types/bit_string.h"
#include "asn1/basic_types/sequence.h"
#include "asn1/pkix/primitive/algorithm_identifier.h"

namespace asn1
{
	namespace pkix
	{
		/*
			SEQUENCE TBS
			{
				value		 ANY,
				algorithm   AlgorithmIdentifier,
				hash       BIT STRING
			}
		*/

		template<typename value_type>
		struct ToBeSigned : Asn1Value
		{
			value_type value;
			AlgorithmIdentifier algorithm;
			BitString hash;

			ToBeSigned()
				: Asn1Value(SEQUENCE_TAG)
			{}

			ToBeSigned(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
			{
				return encoder.encode_collection(writer, tag,
				{
					EncodedValue(&value),
					EncodedValue(&algorithm),
					EncodedValue(&hash)
				});
			}
		};

		template<typename value_type, typename decoder_type>
		class ToBeSignedDecoder : public details::SequenceBasedTypeDecoder<ToBeSigned<value_type>>
		{
		private:
			enum class State : uint8_t
			{
				VALUE_DECODING,
				ALGORITHM_DECODING,
				HASH_DECODING
			};

			decoder_type value_decoder_;
			AlgorithmIdentifierDecoder algorithm_decoder_;
			BitStringDecoder hash_decoder_;
			State state_{ State::VALUE_DECODING };

		public:
			explicit ToBeSignedDecoder(IValueEventHandler* const event_handler)
				: ToBeSignedDecoder(SEQUENCE_TAG, event_handler)
			{}

			ToBeSignedDecoder(const Tag& tag, IValueEventHandler* const event_handler)
				: details::SequenceBasedTypeDecoder<ToBeSigned<value_type>>(tag, event_handler)
				, value_decoder_(&this->decoded_element_event_handler_)
				, algorithm_decoder_(&this->decoded_element_event_handler_)
				, hash_decoder_(&this->decoded_element_event_handler_)
			{
				this->add({
					&value_decoder_,
					&algorithm_decoder_,
					&hash_decoder_
				});
			}

			void reset_state() override
			{
				details::SequenceBasedTypeDecoder<ToBeSigned<value_type>>::reset_state();
				state_ = State::VALUE_DECODING;
			}

		protected:
			void on_decode_element(Asn1Value&& val) override
			{
				if (state_ == State::VALUE_DECODING)
				{
					this->decoded_value_.value = static_cast<value_type&&>(val);
					state_ = State::ALGORITHM_DECODING;
				}
				else if (state_ == State::ALGORITHM_DECODING)
				{
					this->decoded_value_.algorithm = static_cast<AlgorithmIdentifier&&>(val);
					state_ = State::HASH_DECODING;
				}
				else
				{
					this->decoded_value_.hash = static_cast<BitString&&>(val);
				}
			}

			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};
	}
}

#endif