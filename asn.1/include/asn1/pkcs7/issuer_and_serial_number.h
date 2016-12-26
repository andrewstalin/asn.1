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

#ifndef _ASN1_ISSUER_AND_SERIAL_NUMBER_H_
#define _ASN1_ISSUER_AND_SERIAL_NUMBER_H_

#include "asn1/basic_types/integer.h"
#include "asn1/pkix/primitive/name.h"

namespace asn1
{
	namespace pkcs7
	{
		/*
			IssuerAndSerialNumber ::= SEQUENCE 
			{
				issuer        Name,
				serialNumber  CertificateSerialNumber
			}

			CertificateSerialNumber ::=INTEGER
		*/
		struct IssuerAndSerialNumber : Asn1Value
		{
			pkix::Name issuer;
			Integer serial_number;

			IssuerAndSerialNumber()
				: IssuerAndSerialNumber(SEQUENCE_TAG)
			{}

			IssuerAndSerialNumber(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
			{
				return encoder.encode_collection(writer, tag,
				{
					EncodedValue(&issuer),
					EncodedValue(&serial_number)
				});
			}
		};

		class IssuerAndSerialNumberDecoder : public details::SequenceBasedTypeDecoder<IssuerAndSerialNumber>
		{
		private:
		private:
			enum class State : uint8_t
			{
				ISSUER_DECODING,
				SERIAL_NUMBER_DECODING
			};

			pkix::NameDecoder issuer_decoder_;
			IntegerDecoder serial_number_decoder_;
			State state_{ State::ISSUER_DECODING };

		public:
			explicit IssuerAndSerialNumberDecoder(IValueEventHandler* const event_handler)
				: IssuerAndSerialNumberDecoder(SEQUENCE_TAG, event_handler)
			{}

			IssuerAndSerialNumberDecoder(const Tag& tag, IValueEventHandler* const event_handler)
				: details::SequenceBasedTypeDecoder<IssuerAndSerialNumber>(tag, event_handler)
				, issuer_decoder_(&decoded_element_event_handler_)
				, serial_number_decoder_(&decoded_element_event_handler_)
			{
				add({
					&issuer_decoder_,
					&serial_number_decoder_
				});
			}
			
			void reset_state() override
			{
				details::SequenceBasedTypeDecoder<IssuerAndSerialNumber>::reset_state();
				state_ = State::ISSUER_DECODING;
			}

		protected:
			void on_decode_element(Asn1Value&& val) override
			{
				if (state_ == State::ISSUER_DECODING)
				{
					decoded_value_.issuer = static_cast<pkix::Name&&>(val);
					state_ = State::SERIAL_NUMBER_DECODING;
				}
				else
				{
					decoded_value_.serial_number = static_cast<Integer&&>(val);
				}
			}

			void on_decode_element(const Tag&, Asn1Value&&) override {}
		};
	}
}

#endif