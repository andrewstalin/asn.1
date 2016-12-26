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

#ifndef _ASN1_CERTIFICATE_REVOCATION_LIST_H_
#define _ASN1_CERTIFICATE_REVOCATION_LIST_H_

#include "asn1/pkix/certificate/tbs_cert_revocation_list.h"
#include "asn1/pkix/primitive/to_be_signed.h"

namespace asn1
{
	namespace pkix
	{
		namespace cert
		{

			/*
				SEQUENCE  
				{
					tbsCertList          TBSCertList,
					signatureAlgorithm   AlgorithmIdentifier,
					signatureValue       BIT STRING
				}
			*/

			using CertificateRevocationList = asn1::pkix::ToBeSigned<TBSCertificateRevocationList>;
			using CertificateRevocationListDecoder = asn1::pkix::ToBeSignedDecoder<TBSCertificateRevocationList, TBSCertificateRevocationListDecoder>;
		}
	}
}

#endif