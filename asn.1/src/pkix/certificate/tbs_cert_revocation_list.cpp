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

#include "asn1/pkix/certificate/tbs_cert_revocation_list.h"

using namespace asn1;

static constexpr Tag CRL_EXTENSIONS_TAG = { EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 0 };

length_type pkix::cert::TBSCertificateRevocationList::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_elements;
	encoded_elements.reserve(7);

	if (version)
	{
		encoded_elements.emplace_back(version.ptr());
	}

	encoded_elements.emplace_back(&signature);
	encoded_elements.emplace_back(&issuer);
	encoded_elements.emplace_back(&this_update);

	if (next_update)
	{
		encoded_elements.emplace_back(next_update.ptr());
	}

	if (revoked_certificates)
	{
		encoded_elements.emplace_back(revoked_certificates.ptr());
	}

	if (crl_extensions)
	{
		encoded_elements.emplace_back(CRL_EXTENSIONS_TAG, crl_extensions.ptr());
	}

	return encoder.encode_collection(writer, tag, encoded_elements);
}

pkix::cert::TBSCertificateRevocationListDecoder::TBSCertificateRevocationListDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<TBSCertificateRevocationList>(tag, event_handler)
	, version_decoder_(&decoded_element_event_handler_)
	, signature_decoder_(&decoded_element_event_handler_)
	, issuer_decoder_(&decoded_element_event_handler_)
	, this_update_decoder_(&decoded_element_event_handler_)
	, next_update_decoder_(&decoded_element_event_handler_)
	, revoked_certificates_decoder_(&decoded_element_event_handler_)
	, crl_extensions_decoder_(CRL_EXTENSIONS_TAG, &decoded_element_event_handler_)
{
	reserve(7);
	add_optional(&version_decoder_);
	add({
		&signature_decoder_,
		&issuer_decoder_,
		reinterpret_cast<BerDecoder*>(&this_update_decoder_)
	});
	add_optional(reinterpret_cast<BerDecoder*>(&next_update_decoder_));
	add_optional(&revoked_certificates_decoder_);
	add_optional(&crl_extensions_decoder_);
}

void pkix::cert::TBSCertificateRevocationListDecoder::on_decode_element(Asn1Value&& val)
{
	if (val.tag == INTEGER_TAG)
	{
		decoded_value_.version = static_cast<Integer&&>(val);
	}
	else
	{
		switch (state_)
		{
		case State::SIGNATURE_DECODING:
			decoded_value_.signature = static_cast<AlgorithmIdentifier&&>(val);
			state_ = State::ISSUER_DECODING;
			break;
		case State::ISSUER_DECODING:
			decoded_value_.issuer = static_cast<Name&&>(val);
			state_ = State::THIS_UPDATE_DECODING;
			break;
		case State::THIS_UPDATE_DECODING:
			decoded_value_.this_update = static_cast<Time&&>(val);
			state_ = State::NEXT_UPDATE_DECODING;
			break;
		case State::NEXT_UPDATE_DECODING:
			decoded_value_.next_update = static_cast<Time&&>(val);
			state_ = State::OPTIONAL_VALUES_DECODING;
			break;
		case State::OPTIONAL_VALUES_DECODING:
			decoded_value_.revoked_certificates = static_cast<RevokedCertificateCollection&&>(val);
			break;
		}
	}
}

void pkix::cert::TBSCertificateRevocationListDecoder::on_decode_element(const Tag& explicit_tag, Asn1Value&& val)
{
	if (explicit_tag == CRL_EXTENSIONS_TAG)
	{
		decoded_value_.crl_extensions = static_cast<ExtensionCollection&&>(val);
	}
}