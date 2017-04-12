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

static constexpr Tag ORIGINATOR_INFO_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 0 };
static constexpr Tag UNPROTECTED_ATTRIBUTES_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 1 };

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
	, originator_info(&decoded_element_event_handler_)
	, recipient_infos_decoder(&decoded_element_event_handler_)
	, encrypted_content_info_decoder(&decoded_element_event_handler_)
	, unprotected_attributes(&decoded_element_event_handler_)
{
	reserve(5);
	add(&version_decoder);
	add_optional(&originator_info);
	add(&recipient_infos_decoder);
	add(&encrypted_content_info_decoder);
	add_optional(&unprotected_attributes);
}

pkcs7::EnvelopedDataDecoder::EnvelopedDataDecoder(const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
	: details::SequenceBasedTypeDecoder<EnvelopedData>(tag, event_handler)
	, version_decoder(&decoded_element_event_handler_)
	, originator_info(&decoded_element_event_handler_)
	, recipient_infos_decoder(&decoded_element_event_handler_)
	, encrypted_content_info_decoder(&decoded_element_event_handler_, data_event_handler)
	, unprotected_attributes(&decoded_element_event_handler_)
{
	reserve(5);
	add(&version_decoder);
	add_optional(&originator_info);
	add(&recipient_infos_decoder);
	add(&encrypted_content_info_decoder);
	add_optional(&unprotected_attributes);
}

void pkcs7::EnvelopedDataDecoder::on_decode_element(Asn1Value&& val)
{
	if (val.tag == INTEGER_TAG)
	{
		decoded_value_.version = static_cast<Integer&&>(val);
	}
	else if (val.tag == SET_TAG)
	{
		decoded_value_.recipient_infos = static_cast<RecipientInfoCollection&&>(val);
	}
	else if (val.tag == SEQUENCE_TAG)
	{
		decoded_value_.encrypted_content_info = static_cast<EncryptedContentInfo&&>(val);
	}
}
