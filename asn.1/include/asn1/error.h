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

#ifndef _ASN1_ERROR_H_
#define _ASN1_ERROR_H_

#include "liberror/exception.h"

namespace asn1
{
	namespace errors
	{
		DECLARE_ERROR_INFO(WRONG_STRUCTURE, 1, "wrong ASN1 value structure");
		DECLARE_ERROR_INFO(WRONG_TAG, 2, "wrong ASN1 value tag");
		DECLARE_ERROR_INFO(WRONG_ELEMENT_LENGTH, 3, "invalid ASN1 value length");
		DECLARE_ERROR_INFO(MISSING_COLLECTION_ELEMENT, 4, "required collection element not found");
		DECLARE_ERROR_INFO(DUPLICATE_COLLECTION_ELEMENT, 5, "specify duplicate collection element");
		DECLARE_ERROR_INFO(UNEXPECTED_SET_ELEMENT, 6, "specify unexpected set element");
		DECLARE_ERROR_INFO(WRONG_UNUSED_BITS_VALUE, 7, "specify invalid BitString unused bits value");
		DECLARE_ERROR_INFO(INVALID_OBJECT_IDENTIFIER_VALUE, 8, "invalid ObjectIdentifier value");
		DECLARE_ERROR_INFO(INVALID_STRING_VALUE, 9, "invalid String value");
		DECLARE_ERROR_INFO(UNKNOWN_CHOICE_VALUE, 10, "specify invalid Choice value");
		DECLARE_ERROR_INFO(UNKNOWN_ASN1_VALUE, 11, "unknown asn1 value type");
		DECLARE_ERROR_INFO(MORE_DATA, 12, "not enough data for encode ASN1 value");
		DECLARE_ERROR_INFO(REQUIRED_COLLECTION_ELEMENT, 13, "required value must be initialized");
		DECLARE_ERROR_INFO(UNEXPECTED_PKI_ELEMENT, 14, "specify unexpected public key infrastructure value");
		DECLARE_ERROR_INFO(DATA_SIZE_TOO_BIG, 15, "data size too big");
		DECLARE_ERROR_INFO(INSUFFICIENT_BUFFER_SIZE, 16, "insufficient buffer size");
		DECLARE_ERROR_INFO(DECODING_ERROR, 17, "an error occurred while decoding asn.1 value");
		DECLARE_ERROR_INFO(DATA_TYPE_TOO_SMALL, 18, "insufficient data type size");
		DECLARE_ERROR_INFO(WRONG_CONTENT_LENGTH_FOR_DER_ENCODER, 19, "DER encoding disallow using undefined length");
		DECLARE_ERROR_INFO(WRONG_DECODER_STATE, 20, "wrong ASN1 decoder state");
	}


	class Asn1Exception : public liberror::Exception
	{
	public:
		Asn1Exception(const liberror::ErrorInfo& error_info)
			: Exception(error_info)
		{}

		const char* category() const override { return "ASN1"; }
	};
}

#endif