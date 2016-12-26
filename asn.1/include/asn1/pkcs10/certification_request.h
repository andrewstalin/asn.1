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

#ifndef _ASN1_CERTIFICATION_REQUEST_H_
#define _ASN1_CERTIFICATION_REQUEST_H_

#include "asn1/pkcs10/certification_request_info.h"
#include "asn1/pkix/primitive/to_be_signed.h"

namespace asn1
{
	namespace pkcs10
	{
		/*
			SEQUENCE  {
				certificationRequestInfo       CertificationRequestInfo,
				signatureAlgorithm   AlgorithmIdentifier,
				signatureValue       BIT STRING
			}
		*/
		using CertificationRequest = asn1::pkix::ToBeSigned<CertificationRequestInfo>;
		using CertificationRequestDecoder = asn1::pkix::ToBeSignedDecoder<CertificationRequestInfo, CertificationRequestInfoDecoder>;
	}
}

#endif