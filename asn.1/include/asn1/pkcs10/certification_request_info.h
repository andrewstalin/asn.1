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

#ifndef _ASN1_CERTIFICATION_REQUEST_INFO_H_
#define _ASN1_CERTIFICATION_REQUEST_INFO_H_

#include "asn1/basic_types/integer.h"
#include "asn1/pkcs7/attribute.h"
#include "asn1/pkix/primitive/name.h"
#include "asn1/pkix/primitive/subject_public_key_info.h"
#include "asn1/basic_types/sequence.h"

namespace asn1
{
	namespace pkcs10
	{
		/*
			SEQUENCE {
				version       INTEGER
				subject       Name,
				subjectPKInfo SubjectPublicKeyInfo,
				attributes [0] implicit Attributes
			}
		*/
		struct CertificationRequestInfo : public Asn1Value
		{
			Integer version;
			pkix::Name subject;
			pkix::SubjectPublicKeyInfo subject_public_key_info;
			pkcs7::AttributeCollection attributes;

			CertificationRequestInfo()
				: CertificationRequestInfo(SEQUENCE_TAG)
			{}

			CertificationRequestInfo(const Tag& tag)
				: Asn1Value(tag)
			{}

			length_type encode(const IEncoder& encoder, io::IAsn1Writer& writer) const override;
		};

		class CertificationRequestInfoDecoder : public details::SequenceBasedTypeDecoder<CertificationRequestInfo>
		{
		private:
			enum class State : uint8_t
			{
				VERSION_DECODING,
				SUBJECT_DECODING,
				SUBJECT_PUBLIC_KEY_INFO_DECODING,
				ATTRIBUTES_DECODING,
			};

			IntegerDecoder version_decoder_;
			pkix::NameDecoder subject_decoder_;
			pkix::SubjectPublicKeyInfoDecoder subject_public_key_info_decoder_;
			pkcs7::AttributeCollectionDecoder attributes_decoder_;
			State state_{ State::VERSION_DECODING };

		public:
			explicit CertificationRequestInfoDecoder(IValueEventHandler* const event_handler)
				: CertificationRequestInfoDecoder(SEQUENCE_TAG, event_handler)
			{}

			CertificationRequestInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler);

			void reset_state() override
			{
				details::SequenceBasedTypeDecoder<CertificationRequestInfo>::reset_state();
				state_ = State::VERSION_DECODING;
			}

		protected:
			void on_decode_element(Asn1Value&& val) override;
			void on_decode_element(const Tag& explicit_tag, Asn1Value&& val) override {}
		};
	}
}

#endif