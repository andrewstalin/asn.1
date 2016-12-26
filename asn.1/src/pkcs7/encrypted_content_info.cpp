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

#include "asn1/pkcs7/encrypted_content_info.h"

using namespace asn1;

static constexpr Tag ENCRYPTED_CONTENT_TAG = { EncodingForm::PRIMITIVE, TagClass::CONTEXT_SPECIFIC, 0 };

length_type pkcs7::EncryptedContentInfo::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;
	encoded_values.reserve(3);
	encoded_values.emplace_back(&content_type);
	encoded_values.emplace_back(&content_encryption_algorithm);

	if (encrypted_content)
	{
		auto val = const_cast<OctetString*>(encrypted_content.ptr());
		val->tag = ENCRYPTED_CONTENT_TAG;
		encoded_values.emplace_back(val);
	}

	return encoder.encode_collection(writer, tag, encoded_values);
}

length_type pkcs7::StreamingEncryptedContentInfo::evaluate_content_size(const IEncoder& encoder) const
{
	io::NullWriter null_writer;
	auto count = content_type.encode(encoder, null_writer);
	count += content_encryption_algorithm.encode(encoder, null_writer);

	if (content)
	{
		const auto& content_value = content.value();
		length_type content_length = content_value.evaluate_size(encoder);
		count = content_length != INDEFINITE_CONTENT_LENGTH ? count + content_length : INDEFINITE_CONTENT_LENGTH;
	}

	return count;
}

length_type pkcs7::StreamingEncryptedContentInfo::encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	length_type count{ 0 };

	count += content_type.encode(encoder, writer);
	count += content_encryption_algorithm.encode(encoder, writer);

	return count;
}

length_type pkcs7::StreamingEncryptedContentInfo::encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer)
{
	return content ? content.value().encode(encoder, data, size, writer) : 0;
}

length_type pkcs7::StreamingEncryptedContentInfo::encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	return content ? content.value().encode_final(encoder, writer) : 0;
}

pkcs7::EncryptedContentInfoDecoder::EncryptedContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<EncryptedContentInfo>(tag, event_handler)
	, content_type_decoder(&decoded_element_event_handler_)
	, content_encryption_algorithm_decoder(&decoded_element_event_handler_)
	, encrypted_content_decoder(ENCRYPTED_CONTENT_TAG, &decoded_element_event_handler_)
{
	add({
		&content_type_decoder,
		&content_encryption_algorithm_decoder,
	});
	add_optional(&encrypted_content_decoder);
}

pkcs7::EncryptedContentInfoDecoder::EncryptedContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
	: details::SequenceBasedTypeDecoder<EncryptedContentInfo>(tag, event_handler)
	, content_type_decoder(&decoded_element_event_handler_)
	, content_encryption_algorithm_decoder(&decoded_element_event_handler_)
	, encrypted_content_decoder(ENCRYPTED_CONTENT_TAG, data_event_handler)
{
	add({
		&content_type_decoder,
		&content_encryption_algorithm_decoder
	});
	add_optional(&encrypted_content_decoder);
}

void pkcs7::EncryptedContentInfoDecoder::on_decode_element(Asn1Value&& val)
{
	switch (state_)
	{
	case State::CONTENT_TYPE_DECODING:
		decoded_value_.content_type = static_cast<ObjectIdentifier&&>(val);
		state_ = State::CONTENT_ENCRYPTION_ALGORITHM_DECODING;
		break;
	case State::CONTENT_ENCRYPTION_ALGORITHM_DECODING:
		decoded_value_.content_encryption_algorithm = static_cast<pkix::AlgorithmIdentifier&&>(val);
		state_ = State::ENCRYPTED_CONTENT_DECODING;
		break;
	case State::ENCRYPTED_CONTENT_DECODING:
		decoded_value_.encrypted_content = static_cast<OctetString&&>(val);
		break;
	}
}