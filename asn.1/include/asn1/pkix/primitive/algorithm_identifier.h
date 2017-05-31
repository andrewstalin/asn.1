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

#ifndef _ASN1_ALGORITHM_IDENTIFIER_H_
#define _ASN1_ALGORITHM_IDENTIFIER_H_

#include "asn1/basic_types/object_identifier.h"
#include "asn1/basic_types/any.h"
#include "asn1/basic_types/sequence.h"
#include "optional/optional.h"

namespace asn1
{
	namespace pkix
	{
		/*
			AlgorithmIdentifier ::= SEQUENCE 
			{
				algorithm   OBJECT_IDENTIFIER,
				parameters  ANY OPTIONAL
			}
		*/
		struct AlgorithmIdentifier : Asn1Value
		{
			ObjectIdentifier algorithm;
			optional::OptionalValue<Any> parameters;

			AlgorithmIdentifier()
				: Asn1Value(SEQUENCE_TAG)
			{}

			AlgorithmIdentifier(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		class AlgorithmIdentifierDecoder : public details::SequenceBasedTypeDecoder<AlgorithmIdentifier>
		{
		private:
			enum class State : uint8_t
			{
				ALGORITHM_DECODING,
				PARAMETERS_DECODING,
				FINAL
			};

			ObjectIdentifierDecoder algorithm_decoder_;
			AnyDecoder parameters_decoder_;
			State state_{ State::ALGORITHM_DECODING };

		public:
			explicit AlgorithmIdentifierDecoder(IValueEventHandler* const event_handler)
				: AlgorithmIdentifierDecoder(SEQUENCE_TAG, event_handler)
			{}

			AlgorithmIdentifierDecoder(const Tag& tag, IValueEventHandler* const event_handler);

			void reset_state() override
			{
				details::SequenceBasedTypeDecoder<AlgorithmIdentifier>::reset_state();
				state_ = State::ALGORITHM_DECODING;
			}

		protected:
			void on_decode_element(Asn1Value&& val) override;
			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};
	}
}

#endif