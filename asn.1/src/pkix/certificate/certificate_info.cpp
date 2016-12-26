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

#include "asn1/pkix/certificate/certificate_info.h"

using namespace asn1;

static constexpr Tag VERSION_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 0 };
static constexpr Tag ISSUER_UNIQUE_IDENTIFIER_TAG = { EncodingForm::PRIMITIVE, TagClass::CONTEXT_SPECIFIC, 1 };
static constexpr Tag SUBJECT_UNIQUE_IDENTIFIER_TAG = { EncodingForm::PRIMITIVE, TagClass::CONTEXT_SPECIFIC, 2 };
static constexpr Tag EXTENSIONS_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 3 };

length_type pkix::cert::CertificateInfo::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;
	encoded_values.reserve(10);

	if (version.to_int64() != 0)
	{
		encoded_values.emplace_back(VERSION_TAG, &version);
	}

	encoded_values.emplace_back(&serial_number);
	encoded_values.emplace_back(&signature);
	encoded_values.emplace_back(&issuer);
	encoded_values.emplace_back(&validity);
	encoded_values.emplace_back(&subject);
	encoded_values.emplace_back(&subject_public_key_info);

	if (issuer_unique_identifier)
	{
		auto val = const_cast<BitString*>(issuer_unique_identifier.ptr());
		val->tag = ISSUER_UNIQUE_IDENTIFIER_TAG;
		encoded_values.emplace_back(val);
	}

	if (subject_unique_identifier)
	{
		auto val = const_cast<BitString*>(subject_unique_identifier.ptr());
		val->tag = SUBJECT_UNIQUE_IDENTIFIER_TAG;
		encoded_values.emplace_back(val);
	}

	if (extensions)
	{
		encoded_values.emplace_back(EXTENSIONS_TAG, extensions.ptr());
	}

	return encoder.encode_collection(writer, tag, encoded_values);
}

pkix::cert::CertificateInfoDecoder::CertificateInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<CertificateInfo>(tag, event_handler)
	, version_decoder_(VERSION_TAG, &decoded_element_event_handler_)
	, serial_number_decoder_(&decoded_element_event_handler_)
	, signature_decoder_(&decoded_element_event_handler_)
	, issuer_decoder_(&decoded_element_event_handler_)
	, validity_decoder_(&decoded_element_event_handler_)
	, subject_decoder_(&decoded_element_event_handler_)
	, subject_public_key_info_decoder_(&decoded_element_event_handler_)
	, issuer_unique_identifier_decoder_(ISSUER_UNIQUE_IDENTIFIER_TAG, &decoded_element_event_handler_)
	, subject_unique_identifier_decoder_(SUBJECT_UNIQUE_IDENTIFIER_TAG, &decoded_element_event_handler_)
	, extensions_decoder_(EXTENSIONS_TAG, &decoded_element_event_handler_)
{
	reserve(10);
	add_optional(&version_decoder_);
	add({
		&serial_number_decoder_,
		&signature_decoder_,
		&issuer_decoder_,
		&validity_decoder_,
		&subject_decoder_,
		&subject_public_key_info_decoder_
	});
	add_optional(&issuer_unique_identifier_decoder_);
	add_optional(&subject_unique_identifier_decoder_);
	add_optional(&extensions_decoder_);
}

void pkix::cert::CertificateInfoDecoder::on_decode_element(Asn1Value&& val)
{
	switch (state_)
	{
	case State::VERSION_DECODING:
	case State::SERIAL_NUMBER_DECODING:
		decoded_value_.serial_number = static_cast<Integer&&>(val);
		state_ = State::SIGNATURE_DECODING;
		break;
	case State::SIGNATURE_DECODING:
		decoded_value_.signature = static_cast<AlgorithmIdentifier&&>(val);
		state_ = State::ISSUER_DECODING;
		break;
	case State::ISSUER_DECODING:
		decoded_value_.issuer = static_cast<Name&&>(val);
		state_ = State::VALIDITY_DECODING;
		break;
	case State::VALIDITY_DECODING:
		decoded_value_.validity = static_cast<Validity&&>(val);
		state_ = State::SUBJECT_DECODING;
		break;
	case State::SUBJECT_DECODING:
		decoded_value_.subject = static_cast<Name&&>(val);
		state_ = State::SUBJECT_PUBLIC_KEY_INFO_DECODING;
		break;
	case State::SUBJECT_PUBLIC_KEY_INFO_DECODING:
		decoded_value_.subject_public_key_info = static_cast<SubjectPublicKeyInfo&&>(val);
		state_ = State::OPTIONAL_VALUES_DECODING;
		break;
	case State::OPTIONAL_VALUES_DECODING:
		if (val.tag == ISSUER_UNIQUE_IDENTIFIER_TAG)
		{
			decoded_value_.issuer_unique_identifier = static_cast<BitString&&>(val);
		}
		else if (val.tag == SUBJECT_UNIQUE_IDENTIFIER_TAG)
		{
			decoded_value_.subject_unique_identifier = static_cast<BitString&&>(val);
		}
		break;
	}
}

void pkix::cert::CertificateInfoDecoder::on_decode_element(const Tag& explicit_tag, Asn1Value&& val)
{
	if (explicit_tag == VERSION_TAG)
	{
		decoded_value_.version = static_cast<Integer&&>(val);
		state_ = State::SERIAL_NUMBER_DECODING;
	}
	else if (explicit_tag == EXTENSIONS_TAG)
	{
		decoded_value_.extensions = static_cast<ExtensionCollection&&>(val);
	}
}
