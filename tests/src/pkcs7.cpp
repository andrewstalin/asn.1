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

#include "structured_type_test.h"
#include "file.h"
#include "asn1/pkcs7/attribute.h"
#include "asn1/pkcs7/issuer_and_serial_number.h"
#include "asn1/pkcs7/signer_info.h"
#include "asn1/pkcs7/signed_data.h"
#include "asn1/pkcs7/recipient_info.h"
#include "asn1/pkcs7/encrypted_content_info.h"
#include "asn1/pkcs7/enveloped_data.h"
#include "asn1/pkcs7/content_info.h"
#include "gtest/gtest.h"

using namespace asn1;

struct DataEventHandler : IDataEventHandler
{
	std::vector<byte_type> bytes;

	void execute(const byte_type* data, size_type size) override
	{
		bytes.insert(bytes.end(), data, data + size);
	}
};

TEST(attribute_case, decode_encode_test)
{
	test::StructeredTypeTest<pkcs7::AttributeDecoder>(test::load_file("data/attribute")).execute();
}

TEST(signer_info, decode_encode_test)
{
	test::StructeredTypeTest<pkcs7::SignerInfoDecoder>(test::load_file("data/signer_info1")).execute();
}

TEST(signer_info, decode_encode_with_optional_test)
{
	test::StructeredTypeTest<pkcs7::SignerInfoDecoder>(test::load_file("data/signer_info2")).execute();
}

TEST(signed_data_case, decode_encode_test)
{
	test::StructeredTypeTest<pkcs7::SignedDataDecoder>(test::load_file("data/signed_data1")).execute();
}

TEST(signed_data_case, decode_encode_with_optional_test)
{
	test::StructeredTypeTest<pkcs7::SignedDataDecoder>(test::load_file("data/signed_data2")).execute();
}

TEST(signed_data_case, streaming_decode_test)
{
	std::vector<byte_type> expected = { 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x66, 0x72, 0x69, 0x65, 0x6E, 0x64 };
	DataEventHandler data_event_handler;
	test::StructeredTypeTest<pkcs7::SignedDataDecoder>(test::load_file("data/signed_data_attach"), test::load_file("data/signed_data_attach2"), &data_event_handler).execute();
	EXPECT_EQ(expected, data_event_handler.bytes);
}

TEST(issuer_and_serial_number_case, decode_encode_test)
{
	test::StructeredTypeTest<pkcs7::IssuerAndSerialNumberDecoder>(test::load_file("data/issuer_and_serial_number")).execute();
}

TEST(encrypted_content_info_case, decode_encode_with_optional_test)
{
	test::StructeredTypeTest<pkcs7::EncryptedContentInfoDecoder>(test::load_file("data/encrypted_content_info1")).execute();
}

TEST(encrypted_content_info_case, decode_encode_test)
{
	test::StructeredTypeTest<pkcs7::EncryptedContentInfoDecoder>(test::load_file("data/encrypted_content_info2")).execute();
}

TEST(encrypted_content_info_case, streaming_decode_test)
{
	std::vector<byte_type> expected = { 0x47, 0xF5, 0x1D, 0x89, 0x83, 0xDB, 0x6F, 0xE4, 0xD6, 0x60, 0x2B, 0x9A, 0xB8 };
	DataEventHandler data_event_handler;
	test::StructeredTypeTest<pkcs7::EncryptedContentInfoDecoder>(test::load_file("data/encrypted_content_info1"), test::load_file("data/encrypted_content_info2"), &data_event_handler).execute();
	EXPECT_EQ(expected, data_event_handler.bytes);
}

TEST(recipient_info_case, decode_encode_test)
{
	test::StructeredTypeTest<pkcs7::RecipientInfoDecoder>(test::load_file("data/recipient_info")).execute();
}

TEST(enveloped_data_case, decode_encode_test)
{
	test::StructeredTypeTest<pkcs7::EnvelopedDataDecoder>(test::load_file("data/enveloped_data1")).execute();
}

TEST(enveloped_data_case, streaming_decode_test)
{
	std::vector<byte_type> expected = { 0x47, 0xF5, 0x1D, 0x89, 0x83, 0xDB, 0x6F, 0xE4, 0xD6, 0x60, 0x2B, 0x9A, 0xB8 };
	DataEventHandler data_event_handler;
	test::StructeredTypeTest<pkcs7::EnvelopedDataDecoder>(test::load_file("data/enveloped_data1"), test::load_file("data/enveloped_data2"), &data_event_handler).execute();
	EXPECT_EQ(expected, data_event_handler.bytes);
}

TEST(content_info, decode_encode_enveloped_data_test)
{
	test::StructeredTypeTest<pkcs7::ContentInfoDecoder<pkcs7::EnvelopedData, pkcs7::EnvelopedDataDecoder>>(test::load_file("data/content_info_enveloped_data1")).execute();
}

TEST(content_info, streaming_enveloped_data_test)
{
	std::vector<byte_type> expected = { 0x47, 0xF5, 0x1D, 0x89, 0x83, 0xDB, 0x6F, 0xE4, 0xD6, 0x60, 0x2B, 0x9A, 0xB8 };
	DataEventHandler data_event_handler;
	test::StructeredTypeTest<pkcs7::ContentInfoDecoder<pkcs7::EnvelopedData, pkcs7::EnvelopedDataDecoder>>(test::load_file("data/content_info_enveloped_data1"), test::load_file("data/content_info_enveloped_data2"), &data_event_handler).execute();
	EXPECT_EQ(expected, data_event_handler.bytes);
}

TEST(content_info, decode_encode_signed_data_detached_test)
{
	test::StructeredTypeTest<pkcs7::ContentInfoDecoder<pkcs7::SignedData, pkcs7::SignedDataDecoder>>(test::load_file("data/content_info_signed_data_detach")).execute();
}

TEST(content_info, decode_encode_signed_data_attached_test)
{
	test::StructeredTypeTest<pkcs7::ContentInfoDecoder<pkcs7::SignedData, pkcs7::SignedDataDecoder>>(test::load_file("data/content_info_signed_data_attach")).execute();
}
