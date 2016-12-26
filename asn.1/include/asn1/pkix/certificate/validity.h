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

#ifndef _ASN1_VALIDITY_H_
#define _ASN1_VALIDITY_H_

#include "asn1/basic_types/choice.h"
#include "asn1/basic_types/time.h"
#include "asn1/basic_types/sequence.h"

namespace asn1
{
	namespace pkix
	{
		namespace cert
		{
			/*
				Validity ::= SEQUENCE 
				{
					notBefore  Time,
					notAfter   Time
				}

				Time ::= CHOICE 
				{
					utcTime          UTCTime,
					generalizedTime  GeneralizedTime
				}
			*/
			struct Validity : Asn1Value
			{
				Time not_before;
				Time not_after;

				Validity()
					: Validity(SEQUENCE_TAG)
				{}

				Validity(const Tag& tag)
					: Asn1Value(tag)
				{}

				length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
				{
					return encoder.encode_collection(writer, tag, 
					{ 
						EncodedValue(&not_before), 
						EncodedValue(&not_after) 
					});
				}
			};

			class ValidityDecoder : public details::SequenceBasedTypeDecoder<Validity>
			{
			private:
				enum class State : uint8_t
				{
					NOT_BEFORE_DECODING,
					NOT_AFTER_DECODING
				};

				TimePairDecoder not_before_decoder_;
				TimePairDecoder not_after_decoder_;
				State state_{ State::NOT_BEFORE_DECODING };

			public:
				explicit ValidityDecoder(IValueEventHandler* const event_handler)
					: ValidityDecoder(SEQUENCE_TAG, event_handler)
				{}

				ValidityDecoder(const Tag& tag, IValueEventHandler* const event_handler);

				void reset_state() override
				{
					details::SequenceBasedTypeDecoder<Validity>::reset_state();
					state_ = State::NOT_BEFORE_DECODING;
				}

			protected:
				void on_decode_element(Asn1Value&& val) override;
				void on_decode_element(const Tag&, Asn1Value&&) override {}
			};
		}
	}
}

#endif