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

#include "asn1/pkcs7/content_info.h"
#include "asn1/pkcs7/signed_data.h"
#include "asn1/pkcs7/enveloped_data.h"

using namespace asn1;

static constexpr Tag PKCS7_CONTENT_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 0 };

template<typename content_type>
length_type pkcs7::ContentInfo<content_type>::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;
	encoded_values.reserve(2);
	encoded_values.emplace_back(EncodedValue(&content_type_oid));

	if (pkcs7_content)
	{
		encoded_values.emplace_back(PKCS7_CONTENT_TAG, pkcs7_content.ptr());
	}

	return encoder.encode_collection(writer, tag, encoded_values);
}

template<typename content_type>
length_type pkcs7::StreamingContentInfo<content_type>::evaluate_content_size(const IEncoder& encoder) const
{
	io::NullWriter null_writer;
	auto count = content_type_oid.encode(encoder, null_writer);

	if (pkcs7_content)
	{
		const auto& content_value = pkcs7_content.value();
		length_type content_length = content_value.evaluate_size(encoder);
		
		if (content_length != INDEFINITE_CONTENT_LENGTH)
		{
			count += content_length;
			count += encoder.encode_header(null_writer, PKCS7_CONTENT_TAG, content_length);
		}
		else
		{
			count = INDEFINITE_CONTENT_LENGTH;
		}
	}

	return count;
}

template<typename content_type>
length_type pkcs7::StreamingContentInfo<content_type>::encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	length_type count{ 0 };
	count += content_type_oid.encode(encoder, writer);

	if (pkcs7_content)
	{
		auto& content_value = pkcs7_content.value();
		content_length_ = content_value.evaluate_size(encoder);
		count += encoder.encode_header(writer, PKCS7_CONTENT_TAG, content_length_);
	}

	return count;
}

template<typename content_type>
length_type pkcs7::StreamingContentInfo<content_type>::encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer)
{
	return pkcs7_content ? pkcs7_content.value().encode(encoder, data, size, writer) : 0;
}

template<typename content_type>
length_type pkcs7::StreamingContentInfo<content_type>::encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	length_type count{ 0 };

	if (pkcs7_content)
	{
		count = pkcs7_content.value().encode_final(encoder, writer);

		if (content_length_ == INDEFINITE_CONTENT_LENGTH)
		{
			count += 2;
			writer.put(0);
			writer.put(0);
		}
	}

	return count;
}

template<typename content_type, typename decoder_type>
pkcs7::ContentInfoDecoder<content_type, decoder_type>::ContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<ContentInfo<content_type>>(tag, event_handler)
	, content_type_oid_decoder_(&this->decoded_element_event_handler_)
	, pkcs7_content_decoder_(PKCS7_CONTENT_TAG, &this->decoded_element_event_handler_)
{
	this->reserve(2);
	this->add(&content_type_oid_decoder_);
	this->add_optional(&pkcs7_content_decoder_);
}

template<typename content_type, typename decoder_type>
pkcs7::ContentInfoDecoder<content_type, decoder_type>::ContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
	: details::SequenceBasedTypeDecoder<ContentInfo<content_type>>(tag, event_handler)
	, content_type_oid_decoder_(&this->decoded_element_event_handler_)
	, pkcs7_content_decoder_(PKCS7_CONTENT_TAG, &this->decoded_element_event_handler_, data_event_handler)
{
	this->reserve(2);
	this->add(&content_type_oid_decoder_);
	this->add_optional(&pkcs7_content_decoder_);
}

template struct pkcs7::ContentInfo<pkcs7::EnvelopedData>;
template struct pkcs7::ContentInfo<pkcs7::SignedData>;

template struct pkcs7::StreamingContentInfo<pkcs7::StreamingSignedData>;
template struct pkcs7::StreamingContentInfo<pkcs7::StreamingEnvelopedData>;

template class pkcs7::ContentInfoDecoder<pkcs7::EnvelopedData, pkcs7::EnvelopedDataDecoder>;
template class pkcs7::ContentInfoDecoder<pkcs7::SignedData, pkcs7::SignedDataDecoder>;
