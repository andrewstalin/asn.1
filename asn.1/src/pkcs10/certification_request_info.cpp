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

#include "asn1/pkcs10/certification_request_info.h"

using namespace asn1;

static constexpr Tag ATTRIBUTES_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 0 };

length_type pkcs10::CertificationRequestInfo::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;
	encoded_values.reserve(4);

	encoded_values.emplace_back(&version);
	encoded_values.emplace_back(&subject);
	encoded_values.emplace_back(&subject_public_key_info);

	auto val = const_cast<pkcs7::AttributeCollection*>(&attributes);
	val->tag = ATTRIBUTES_TAG;
	encoded_values.emplace_back(val);

	return encoder.encode_collection(writer, tag, encoded_values);
}

pkcs10::CertificationRequestInfoDecoder::CertificationRequestInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<CertificationRequestInfo>(tag, event_handler)
	, version_decoder_(&decoded_element_event_handler_)
	, subject_decoder_(&decoded_element_event_handler_)
	, subject_public_key_info_decoder_(&decoded_element_event_handler_)
	, attributes_decoder_(ATTRIBUTES_TAG, &decoded_element_event_handler_)
{
	add({
		&version_decoder_,
		&subject_decoder_,
		&subject_public_key_info_decoder_,
		&attributes_decoder_
	});
}

void pkcs10::CertificationRequestInfoDecoder::on_decode_element(Asn1Value&& val)
{
	switch (state_)
	{
	case State::VERSION_DECODING:
		decoded_value_.version = static_cast<Integer&&>(val);
		state_ = State::SUBJECT_DECODING;
		break;
	case State::SUBJECT_DECODING:
		decoded_value_.subject = static_cast<pkix::Name&&>(val);
		state_ = State::SUBJECT_PUBLIC_KEY_INFO_DECODING;
		break;
	case State::SUBJECT_PUBLIC_KEY_INFO_DECODING:
		decoded_value_.subject_public_key_info = static_cast<pkix::SubjectPublicKeyInfo&&>(val);
		state_ = State::ATTRIBUTES_DECODING;
		break;
	case State::ATTRIBUTES_DECODING:
		decoded_value_.attributes = static_cast<pkcs7::AttributeCollection&&>(val);
		break;
	}
}
