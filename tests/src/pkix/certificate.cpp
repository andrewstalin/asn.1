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
#include "asn1/pkix/certificate/extension.h"
#include "asn1/pkix/certificate/validity.h"
#include "asn1/pkix/certificate/certificate_info.h"
#include "asn1/pkix/certificate/revoked_certificate.h"
#include "asn1/pkix/certificate/tbs_cert_revocation_list.h"
#include "asn1/pkix/certificate/certificate_revocation_list.h"
#include "asn1/pkix/certificate/certificate.h"
#include "gtest/gtest.h"

using namespace asn1;

TEST(extension_case, decode_encode_test)
{
	test::StructeredTypeTest<pkix::cert::ExtensionDecoder>(test::load_file("data/extension1")).execute();
}

TEST(extension_case, decode_encode_with_critical_test)
{
	test::StructeredTypeTest<pkix::cert::ExtensionDecoder>(test::load_file("data/extension2")).execute();
}

TEST(validity_case, decode_encode_test)
{
	test::StructeredTypeTest<pkix::cert::ValidityDecoder>(test::load_file("data/validity")).execute();
}

TEST(certificate_info_case, decode_encode_test)
{
	test::StructeredTypeTest<pkix::cert::CertificateInfoDecoder>(test::load_file("data/certificate_info1")).execute();
}

TEST(certificate_info_case, decode_encode_with_optional_test)
{
	test::StructeredTypeTest<pkix::cert::CertificateInfoDecoder>(test::load_file("data/certificate_info2")).execute();
}

TEST(revoked_certificate_case, decode_encode_with_optional_test)
{
	test::StructeredTypeTest<pkix::cert::RevokedCertificateDecoder>(test::load_file("data/revoked_certificate")).execute();
}

TEST(tbs_cert_revocation_list_case, decode_encode_test)
{
	test::StructeredTypeTest<pkix::cert::TBSCertificateRevocationListDecoder>(test::load_file("data/tbs_cert_revocation_list2")).execute();
}

TEST(tbs_cert_revocation_list_case, decode_encode_with_optional_test)
{
	test::StructeredTypeTest<pkix::cert::TBSCertificateRevocationListDecoder>(test::load_file("data/tbs_cert_revocation_list1")).execute();
}

TEST(certificate_revocation_list_case, decode_encode_test)
{
	test::StructeredTypeTest<pkix::cert::CertificateRevocationListDecoder>(test::load_file("data/certificate_revocation_list")).execute();
}

TEST(certificate_case, decode_encode_test)
{
	test::StructeredTypeTest<pkix::cert::CertificateDecoder>(test::load_file("data/certificate")).execute();
}