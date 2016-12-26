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

#include "asn1/pkix/certificate/revoked_certificate.h"

using namespace asn1;

length_type pkix::cert::RevokedCertificate::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_elements;
	encoded_elements.reserve(3);

	encoded_elements.emplace_back(&user_certificate);
	encoded_elements.emplace_back(&revocation_date);

	if (crl_entry_extensions)
	{
		encoded_elements.emplace_back(crl_entry_extensions.ptr());
	}

	return encoder.encode_collection(writer, tag, encoded_elements);
}

pkix::cert::RevokedCertificateDecoder::RevokedCertificateDecoder(const Tag& tag, IValueEventHandler* const event_handler)
	: details::SequenceBasedTypeDecoder<RevokedCertificate>(tag, event_handler)
	, user_certificate_decoder_(&decoded_element_event_handler_)
	, revocation_date_decoder_(&decoded_element_event_handler_)
	, crl_entry_extensions_decoder_(&decoded_element_event_handler_)
{
	reserve(3);
	add({
		&user_certificate_decoder_,
		reinterpret_cast<BerDecoder*>(&revocation_date_decoder_)
	});
	add_optional(&crl_entry_extensions_decoder_);
}

void pkix::cert::RevokedCertificateDecoder::on_decode_element(Asn1Value&& val)
{
	if (val.tag == INTEGER_TAG)
	{
		decoded_value_.user_certificate = static_cast<Integer&&>(val);
	}
	else if (val.tag == UTC_TIME_TAG || val.tag == GENERALIZED_TIME_TAG)
	{
		decoded_value_.revocation_date = static_cast<Time&&>(val);
	}
	else if (val.tag == SEQUENCE_TAG)
	{
		decoded_value_.crl_entry_extensions = static_cast<ExtensionCollection&&>(val);
	}
}
