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

#include "asn1/pkcs7/enveloped_data.h"

using namespace asn1;

length_type pkcs7::EnvelopedData::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	return encoder.encode_collection(writer, tag,
	{
		EncodedValue(&version),
		EncodedValue(&recipient_infos),
		EncodedValue(&encrypted_content_info)
	});
}

length_type pkcs7::StreamingEnvelopedData::evaluate_content_size(const IEncoder& encoder) const
{
	length_type count{ 0 };
	io::NullWriter null_writer;
	count += version.encode(encoder, null_writer);
	count += recipient_infos.encode(encoder, null_writer);
	auto content_length = encrypted_content_info.evaluate_size(encoder);
	count = content_length != INDEFINITE_CONTENT_LENGTH ? count + content_length : INDEFINITE_CONTENT_LENGTH;
	return count;
}

length_type pkcs7::StreamingEnvelopedData::encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	length_type count{ 0 };

	count += version.encode(encoder, writer);
	count += recipient_infos.encode(encoder, writer);

	return count;
}

length_type pkcs7::StreamingEnvelopedData::encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer)
{
	return encrypted_content_info.encode(encoder, data, size, writer);
}

length_type pkcs7::StreamingEnvelopedData::encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	return encrypted_content_info.encode_final(encoder, writer);
}

pkcs7::EnvelopedDataDecoder::EnvelopedDataDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<EnvelopedData>(tag, event_handler)
	, version_decoder(&decoded_element_event_handler_)
	, recipient_infos_decoder(&decoded_element_event_handler_)
	, encrypted_content_info_decoder(&decoded_element_event_handler_)
{
	add({ 
		&version_decoder,
		&recipient_infos_decoder,
		&encrypted_content_info_decoder
	});
}

pkcs7::EnvelopedDataDecoder::EnvelopedDataDecoder(const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
	: details::SequenceBasedTypeDecoder<EnvelopedData>(tag, event_handler)
	, version_decoder(&decoded_element_event_handler_)
	, recipient_infos_decoder(&decoded_element_event_handler_)
	, encrypted_content_info_decoder(&decoded_element_event_handler_, data_event_handler)
{
	add({
		&version_decoder,
		&recipient_infos_decoder,
		&encrypted_content_info_decoder
	});
}

void pkcs7::EnvelopedDataDecoder::on_decode_element(Asn1Value&& val)
{
	switch (state_)
	{
	case State::VERSION_DECODING:
		decoded_value_.version = static_cast<Integer&&>(val);
		state_ = State::RECIPIENT_INFOS_DECODING;
		break;
	case State::RECIPIENT_INFOS_DECODING:
		decoded_value_.recipient_infos = static_cast<RecipientInfoCollection&&>(val);
		state_ = State::ENCRYPTED_CONTENT_INFO_DECODING;
		break;
	case State::ENCRYPTED_CONTENT_INFO_DECODING:
		decoded_value_.encrypted_content_info = static_cast<EncryptedContentInfo&&>(val);
		break;
	}
}