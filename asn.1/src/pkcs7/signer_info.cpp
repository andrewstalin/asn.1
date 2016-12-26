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

#include "asn1/pkcs7/signer_info.h"

using namespace asn1;

static constexpr Tag AUTHENTICATED_ATTRIBUTES_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 0 };
static constexpr Tag UNAUTHENTICATED_ATTRIBUTES_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 1 };

length_type pkcs7::SignerInfo::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;
	encoded_values.reserve(7);
	encoded_values.emplace_back(&version);
	encoded_values.emplace_back(&issuer_and_serial_number);
	encoded_values.emplace_back(&digest_algorithm);

	if (authenticated_attributes)
	{
		auto val = const_cast<AttributeCollection*>(authenticated_attributes.ptr());
		val->tag = AUTHENTICATED_ATTRIBUTES_TAG;
		encoded_values.emplace_back(val);
	}

	encoded_values.emplace_back(&digest_encryption_algorithm);
	encoded_values.emplace_back(&encrypted_digest);

	if (unauthenticated_attributes)
	{
		auto val = const_cast<AttributeCollection*>(unauthenticated_attributes.ptr());
		val->tag = UNAUTHENTICATED_ATTRIBUTES_TAG;
		encoded_values.emplace_back(val);
	}

	return encoder.encode_collection(writer, tag, encoded_values);
}

pkcs7::SignerInfoDecoder::SignerInfoDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<SignerInfo>(tag, event_handler)
	, version_decoder_(&decoded_element_event_handler_)
	, issuer_and_serial_number_decoder_(&decoded_element_event_handler_)
	, digest_algorithm_decoder_(&decoded_element_event_handler_)
	, authenticated_attributes_decoder_(AUTHENTICATED_ATTRIBUTES_TAG, &decoded_element_event_handler_)
	, digest_encryption_algorithm_decoder_(&decoded_element_event_handler_)
	, encrypted_digest_decoder_(&decoded_element_event_handler_)
	, unauthenticated_attributes_decoder_(UNAUTHENTICATED_ATTRIBUTES_TAG, &decoded_element_event_handler_)
{
	reserve(7);
	add({
		&version_decoder_,
		&issuer_and_serial_number_decoder_,
		&digest_algorithm_decoder_
	});
	add_optional(&authenticated_attributes_decoder_);
	add({
		&digest_encryption_algorithm_decoder_,
		&encrypted_digest_decoder_
	});
	add_optional(&unauthenticated_attributes_decoder_);
}

void pkcs7::SignerInfoDecoder::on_decode_element(Asn1Value&& val)
{
	switch (state_)
	{
	case State::VERSION_DECODING:
		decoded_value_.version = static_cast<Integer&&>(val);
		state_ = State::ISSUER_AND_SERIAL_NUMBER_DECODING;
		break;
	case State::ISSUER_AND_SERIAL_NUMBER_DECODING:
		decoded_value_.issuer_and_serial_number = static_cast<IssuerAndSerialNumber&&>(val);
		state_ = State::DIGEST_ALGORITHM_DECODING;
		break;
	case State::DIGEST_ALGORITHM_DECODING:
		decoded_value_.digest_algorithm = static_cast<pkix::AlgorithmIdentifier&&>(val);
		state_ = State::AUTHENTICATED_ATTRIBUTES_DECODING;
		break;
	case State::AUTHENTICATED_ATTRIBUTES_DECODING:
		if (val.tag != AUTHENTICATED_ATTRIBUTES_TAG)
		{
			decoded_value_.digest_encryption_algorithm = static_cast<pkix::AlgorithmIdentifier&&>(val);
			state_ = State::ENCRYPTED_DIGEST_DECODING;
		}
		else
		{
			decoded_value_.authenticated_attributes = static_cast<AttributeCollection&&>(val);
			state_ = State::DIGEST_ENCRYPTION_ALGORITHM_DECODING;
		}
		break;
	case State::DIGEST_ENCRYPTION_ALGORITHM_DECODING:
		decoded_value_.digest_encryption_algorithm = static_cast<pkix::AlgorithmIdentifier&&>(val);
		state_ = State::ENCRYPTED_DIGEST_DECODING;
		break;
	case State::ENCRYPTED_DIGEST_DECODING:
		decoded_value_.encrypted_digest = static_cast<OctetString&&>(val);
		state_ = State::UNAUTHENTICATED_ATTRIBUTES_DECODING;
		break;
	case State::UNAUTHENTICATED_ATTRIBUTES_DECODING:
		decoded_value_.unauthenticated_attributes = static_cast<AttributeCollection&&>(val);
		break;
	}
}