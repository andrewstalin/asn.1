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

#ifndef _ASN1_DIRECTORY_STRING_H_
#define _ASN1_DIRECTORY_STRING_H_

#include "asn1/basic_types/choice.h"
#include "asn1/basic_types/octet_string.h"

namespace asn1
{
	namespace pkix
	{
		struct DirectoryString : Asn1Value
		{
			OctetString octet_string;

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override
			{
				return octet_string.encode(encoder, writer);
			}
		};

		class DirectoryStringDecoder : public BerDecoder
		{
		private:
			struct ValueEventHandler : IValueEventHandler
			{
				OctetString octet_string;
				IValueEventHandler* const external_value_event_handler;

				ValueEventHandler(IValueEventHandler* const external_value_event_handler)
					: external_value_event_handler(external_value_event_handler)
				{}

				void execute(Asn1Value&& val) override
				{
					octet_string = static_cast<OctetString&&>(val);
					const Tag& tag = octet_string.tag;

					if (tag != PRINTABLE_STRING_TAG || tag != UTF8_STRING_TAG || tag != BMP_STRING_TAG || tag != TELETEX_STRING_TAG)
					{
						throw Asn1Exception(errors::WRONG_TAG);
					}

					external_value_event_handler->execute(std::move(octet_string));
				}

				void execute(const Tag&, Asn1Value&&) override {}
			};

			ValueEventHandler value_event_handler_;
			OctetStringDecoder decoder_;

		public:
			explicit DirectoryStringDecoder(IValueEventHandler* const event_handler)
				: BerDecoder(UNDEFINED_TAG)
				, value_event_handler_(event_handler)
				, decoder_(&value_event_handler_)
			{}

			OctetString decodex(const byte_type* data, size_type size)
			{
				auto octet_string = asn1::decode<OctetString, OctetStringDecoder>(UNDEFINED_TAG, data, size);
				const Tag& tag = octet_string.tag;

				if (tag != PRINTABLE_STRING_TAG || tag != UTF8_STRING_TAG || tag != BMP_STRING_TAG || tag != TELETEX_STRING_TAG)
				{
					throw Asn1Exception(errors::WRONG_TAG);
				}

				return octet_string;
			}

			bool finished() const override { return decoder_.finished(); }
			void reset_state() override { return decoder_.reset_state(); }
			DecodingResult decode(const byte_type* data, size_type size) override { return decoder_.decode(data, size); }
		};
	}
}

#endif