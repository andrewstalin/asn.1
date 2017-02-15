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

#include "asn1/pkix/certificate/ess_cert_id_v2.h"

using namespace asn1;

length_type pkix::cert::ESSCertIDV2GeneralNames::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	return encoder.encode_collection(writer, tag,
	{
		EncodedValue(Tag(EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 4), &issuer)
	});
}

length_type pkix::cert::ESSCertIDV2IssuerSerial::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	return encoder.encode_collection(writer, tag,
	{
		EncodedValue(&issuer),
		EncodedValue(&serial_number)
	});
}

length_type pkix::cert::ESSCertIDV2::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
{
	std::vector<EncodedValue> encoded_values;

	encoded_values.reserve(3);
	encoded_values.emplace_back(&hash_algorithm);
	encoded_values.emplace_back(&cert_hash);

	if (issuer_serial)
	{
		encoded_values.emplace_back(issuer_serial.ptr());
	}

	return encoder.encode_collection(writer, tag, encoded_values);
}
