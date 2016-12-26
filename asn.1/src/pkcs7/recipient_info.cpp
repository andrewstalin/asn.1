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

//#include "asn1/pkcs7/recipient_info.h"
//#include "asn1/decoders/octet_string_decoder.h"
//
//using namespace asn1;
//
//const Tag ENCRYPTED_CONTENT_TAG = Tag(EncodingForm::CONSTRUCTED, TagClass::CONTEXT_SPECIFIC, 0);
//
//uint64_t pkcs7::RecipientInfo::encode(const IEncoder& encoder, io::IAsn1Writer& writer) const
//{
//	REQUIRED_VALUE(version.get(), "RecipientInfo.Version");
//	REQUIRED_VALUE(issuer_and_serial_number.get(), "RecipientInfo.IssuerAndSerialNumber");
//	REQUIRED_VALUE(key_encryption_algorithm.get(), "RecipientInfo.KeyEncryptionAlgorithm");
//	REQUIRED_VALUE(encrypted_key.get(), "RecipientInfo.EncryptedKey");
//
//	std::vector<EncodedValue> encoded_values = {
//		EncodedValue(version.get()),
//		EncodedValue(issuer_and_serial_number.get()),
//		EncodedValue(key_encryption_algorithm.get()),
//		EncodedValue(encrypted_key.get())
//	};
//
//	return encoder.encode_collection(writer, tag, encoded_values);
//}
//
//pkcs7::RecipientInfoDecoder::RecipientInfoDecoder(const Tag& tag, const OnValueEventHandler& value_event_handler)
//	: SequenceBaseTypeDecoder("RecipientInfo", tag, value_event_handler)
//{
//	add(std::make_unique<IntegerDecoder>([this](std::unique_ptr<IAsn1Value> val)
//	{
//		version_.reset(static_cast<Integer*>(val.release()));
//	}));
//
//	add(std::make_unique<pkcs7::IssuerAndSerialNumberDecoder>([this](std::unique_ptr<IAsn1Value> val)
//	{
//		issuer_and_serial_number_.reset(static_cast<pkcs7::IssuerAndSerialNumber*>(val.release()));
//	}));
//
//	add(std::make_unique<pkix::AlgorithmIdentifierDecoder>([this](std::unique_ptr<IAsn1Value> val)
//	{
//		key_encryption_algorithm_.reset(static_cast<pkix::AlgorithmIdentifier*>(val.release()));
//	}));
//
//	add(std::make_unique<OctetStringDecoder>([this](std::unique_ptr<IAsn1Value> val)
//	{
//		encrypted_key_.reset(static_cast<OctetString*>(val.release()));
//	}));
//}
//
//void pkcs7::RecipientInfoDecoder::clear()
//{
//	version_.reset();
//	issuer_and_serial_number_.reset();
//	key_encryption_algorithm_.reset();
//	encrypted_key_.reset();
//}
//
//std::unique_ptr<IAsn1Value> pkcs7::RecipientInfoDecoder::get_value()
//{
//	auto recipient_info = std::make_unique<RecipientInfo>();
//	recipient_info->version = std::move(version_);
//	recipient_info->issuer_and_serial_number = std::move(issuer_and_serial_number_);
//	recipient_info->key_encryption_algorithm = std::move(key_encryption_algorithm_);
//	recipient_info->encrypted_key = std::move(encrypted_key_);
//	return std::move(recipient_info);
//}