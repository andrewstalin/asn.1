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

#include "asn1/pkcs7/signed_data.h"

using namespace asn1;

static constexpr Tag CERTIFICATES_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 0 };
static constexpr Tag CRLS_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 1 };

length_type pkcs7::SignedData::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;
	encoded_values.reserve(6);
	encoded_values.emplace_back(&version);
	encoded_values.emplace_back(&digest_algorithms);
	encoded_values.emplace_back(&content_info);

	if (certificates)
	{
		auto val = const_cast<SetOf<pkix::cert::Certificate>*>(certificates.ptr());
		val->tag = CERTIFICATES_TAG;
		encoded_values.emplace_back(val);
	}

	if (crls)
	{
		auto val = const_cast<SetOf<pkix::cert::CertificateRevocationList>*>(crls.ptr());
		val->tag = CRLS_TAG;
		encoded_values.emplace_back(val);
	}

	encoded_values.emplace_back(&signer_infos);
	return encoder.encode_collection(writer, tag, encoded_values);
}

length_type pkcs7::StreamingSignedData::evaluate_content_size(const IEncoder& encoder) const
{
	length_type count{ 0 };
	auto content_length = content_info.evaluate_size(encoder);

	if (content_length != INDEFINITE_CONTENT_LENGTH)
	{
		io::NullWriter null_writer;
		count += version.encode(encoder, null_writer);
		count += digest_algorithms.encode(encoder, null_writer);
		count += content_length;
		count += signer_infos.encode(encoder, null_writer);

		if (certificates)
		{
			count += certificates.value().encode(encoder, null_writer);
		}

		if (crls)
		{
			count += crls.value().encode(encoder, null_writer);
		}
	}
	else
	{
		count = INDEFINITE_CONTENT_LENGTH;
	}

	return count;
}

length_type pkcs7::StreamingSignedData::encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	length_type count{ 0 };

	count += version.encode(encoder, writer);
	count += digest_algorithms.encode(encoder, writer);

	return count;
}

length_type pkcs7::StreamingSignedData::encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer)
{
	return content_info.encode(encoder, data, size, writer);
}

length_type pkcs7::StreamingSignedData::encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	auto count = content_info.encode_final(encoder, writer);

	if (certificates)
	{
		auto val = const_cast<SetOf<pkix::cert::Certificate>*>(certificates.ptr());
		val->tag = CERTIFICATES_TAG;
		count += val->encode(encoder, writer);
	}

	if (crls)
	{
		auto val = const_cast<SetOf<pkix::cert::CertificateRevocationList>*>(crls.ptr());
		val->tag = CRLS_TAG;
		count += val->encode(encoder, writer);
	}

	count += signer_infos.encode(encoder, writer);
	return count;
}

pkcs7::SignedDataDecoder::SignedDataDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<SignedData>(tag, event_handler)
	, version_decoder_(&decoded_element_event_handler_)
	, digest_algorithms_decoder_(&decoded_element_event_handler_)
	, content_info_decoder_(&decoded_element_event_handler_)
	, certificates_decoder_(CERTIFICATES_TAG, &decoded_element_event_handler_)
	, crls_decoder_(CRLS_TAG, &decoded_element_event_handler_)
	, signer_infos_decoder_(&decoded_element_event_handler_)
{
	reserve(6);
	add({
		&version_decoder_,
		&digest_algorithms_decoder_,
		&content_info_decoder_
	});
	add_optional(&certificates_decoder_);
	add_optional(&crls_decoder_);
	add(&signer_infos_decoder_);
}

pkcs7::SignedDataDecoder::SignedDataDecoder(const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
	: details::SequenceBasedTypeDecoder<SignedData>(tag, event_handler)
	, version_decoder_(&decoded_element_event_handler_)
	, digest_algorithms_decoder_(&decoded_element_event_handler_)
	, content_info_decoder_(&decoded_element_event_handler_, data_event_handler)
	, certificates_decoder_(CERTIFICATES_TAG, &decoded_element_event_handler_)
	, crls_decoder_(CRLS_TAG, &decoded_element_event_handler_)
	, signer_infos_decoder_(&decoded_element_event_handler_)
{
	reserve(6);
	add({
		&version_decoder_,
		&digest_algorithms_decoder_,
		&content_info_decoder_
	});
	add_optional(&certificates_decoder_);
	add_optional(&crls_decoder_);
	add(&signer_infos_decoder_);
}

void pkcs7::SignedDataDecoder::on_decode_element(Asn1Value&& val)
{
	switch (state_)
	{
	case State::VERSION_DECODING:
		decoded_value_.version = static_cast<Integer&&>(val);
		state_ = State::DIGEST_ALGORITHMS_DECODING;
		break;
	case State::DIGEST_ALGORITHMS_DECODING:
		decoded_value_.digest_algorithms = static_cast<SetOf<pkix::AlgorithmIdentifier>&&>(val);
		state_ = State::CONTENT_INFO_DECODING;
		break;
	case State::CONTENT_INFO_DECODING:
		decoded_value_.content_info = static_cast<EncapsulatedContentInfo&&>(val);
		state_ = State::CERTIFICATES_DECODING;
		break;
	case State::CERTIFICATES_DECODING:
		if (val.tag == CERTIFICATES_TAG)
		{
			decoded_value_.certificates = static_cast<SetOf<pkix::cert::Certificate>&&>(val);
			state_ = State::CRLS_DECODING;
		}
		else if (val.tag == CRLS_TAG)
		{
			decoded_value_.crls = static_cast<SetOf<pkix::cert::CertificateRevocationList>&&>(val);
			state_ = State::SIGNER_INFOS_DECODING;
		}
		else
		{
			decoded_value_.signer_infos = static_cast<SetOf<SignerInfo>&&>(val);
		}
		break;
	case State::CRLS_DECODING:
		if (val.tag == CRLS_TAG)
		{
			decoded_value_.crls = static_cast<SetOf<pkix::cert::CertificateRevocationList>&&>(val);
		}
		else
		{
			decoded_value_.signer_infos = static_cast<SetOf<SignerInfo>&&>(val);
		}

		state_ = State::SIGNER_INFOS_DECODING;
		break;
	case State::SIGNER_INFOS_DECODING:
		decoded_value_.signer_infos = static_cast<SetOf<SignerInfo>&&>(val);
		break;
	}
}
