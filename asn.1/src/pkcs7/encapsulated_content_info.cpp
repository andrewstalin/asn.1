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

#include "asn1/pkcs7/encapsulated_content_info.h"

using namespace asn1;

static constexpr Tag CONTENT_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 0 };

length_type pkcs7::EncapsulatedContentInfo::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;
	encoded_values.reserve(2);
	encoded_values.emplace_back(&content_type_oid);

	if (content)
	{
		encoded_values.emplace_back(CONTENT_TAG, content.ptr());
	}

	return encoder.encode_collection(writer, tag, encoded_values);
}

length_type pkcs7::StreamingEncapsulatedContentInfo::evaluate_content_size(const IEncoder& encoder) const
{
	io::NullWriter null_writer;
	auto count = content_type_oid.encode(encoder, null_writer);

	if (content)
	{
		const auto& content_value = content.value();
		length_type content_length = content_value.evaluate_size(encoder);

		if (content_length != INDEFINITE_CONTENT_LENGTH)
		{
			count += content_length;
			count += encoder.encode_header(null_writer, CONTENT_TAG, content_length);
		}
		else
		{
			count = INDEFINITE_CONTENT_LENGTH;
		}
	}

	return count;
}

length_type pkcs7::StreamingEncapsulatedContentInfo::encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	length_type count{ 0 };

	count += content_type_oid.encode(encoder, writer);

	if (content)
	{
		auto& content_value = content.value();
		content_length_ = content_value.evaluate_size(encoder);
		count += encoder.encode_header(writer, CONTENT_TAG, content_length_);
	}

	return count;
}

length_type pkcs7::StreamingEncapsulatedContentInfo::encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer)
{
	return content ? content.value().encode(encoder, data, size, writer) : 0;
}

length_type pkcs7::StreamingEncapsulatedContentInfo::encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	length_type count{ 0 };

	if (content)
	{
		count = content.value().encode_final(encoder, writer);

		if (content_length_ == INDEFINITE_CONTENT_LENGTH)
		{
			count += 2;
			writer.put(0);
			writer.put(0);
		}
	}
	return count;
}

pkcs7::EncapsulatedContentInfoDecoder::EncapsulatedContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<EncapsulatedContentInfo>(tag, event_handler)
	, content_type_oid_decoder_(&decoded_element_event_handler_)
	, content_decoder_(CONTENT_TAG, &decoded_element_event_handler_)
{
	reserve(2);
	add(&content_type_oid_decoder_);
	add_optional(&content_decoder_);
}

pkcs7::EncapsulatedContentInfoDecoder::EncapsulatedContentInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler, IDataEventHandler* const data_event_handler)
	: details::SequenceBasedTypeDecoder<EncapsulatedContentInfo>(tag, event_handler)
	, content_type_oid_decoder_(&decoded_element_event_handler_)
	, content_decoder_(CONTENT_TAG, data_event_handler)
{
	reserve(2);
	add(&content_type_oid_decoder_);
	add_optional(&content_decoder_);
}