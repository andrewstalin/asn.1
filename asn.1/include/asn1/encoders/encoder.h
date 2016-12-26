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

#ifndef _ASN1_ENCODER_H_
#define _ASN1_ENCODER_H_

#include "asn1/tag.h"
#include "asn1/io/writer.h"
#include <vector>

namespace asn1
{
	struct Time;
	struct Asn1Value;

	struct EncodedValue
	{
		Tag explicit_tag{ UNDEFINED_TAG };
		const Asn1Value* const value;
		Asn1Value* default_value{ nullptr };

		explicit EncodedValue(const Asn1Value* const val)
			: value(val)
		{}

		EncodedValue(const Tag& explicit_tag, const Asn1Value* const val)
			: explicit_tag(explicit_tag), value(val)
		{}

		EncodedValue(const Asn1Value* const val, Asn1Value* default_val)
			: value(val), default_value(default_val)
		{}

		EncodedValue(const EncodedValue&) = default;
		EncodedValue(EncodedValue&&) = default;

		EncodedValue& operator=(const EncodedValue&) = delete;
		EncodedValue& operator=(EncodedValue&&) = delete;
	};

	struct IEncoder
	{
		length_type encode_boolean(io::IAsn1Writer& writer, const Tag& tag, bool value) const;
		length_type encode_null(io::IAsn1Writer& writer, const Tag& tag) const;
		length_type encode_integer(io::IAsn1Writer& writer, const Tag& tag, const std::vector<byte_type>& bytes) const;
		length_type encode_object_identifier(io::IAsn1Writer& writer, const Tag& tag, const std::vector<uint64_t>& numbers) const;
		length_type encode_octet_string(io::IAsn1Writer& writer, const Tag& tag, const byte_type* data, size_type size) const;
		length_type encode_bit_string(io::IAsn1Writer& writer, const Tag& tag, uint8_t unused_bits, const byte_type* data, size_type size) const;
		length_type encode_collection(io::IAsn1Writer& writer, const Tag& tag, const std::vector<EncodedValue>& elements) const;

		virtual length_type encode_header(io::IAsn1Writer& writer, const Tag& tag, length_type content_length) const = 0;
		virtual length_type encode_utc_time(io::IAsn1Writer& writer, const Tag& tag, const Time& time) const = 0;
		virtual length_type encode_generalized_time(io::IAsn1Writer& writer, const Tag& tag, const Time& time) const = 0;

	protected:
		length_type encode_tag(io::IAsn1Writer& writer, const Tag& tag) const;
	};

	struct BerEncoder : public IEncoder
	{
		length_type encode_header(io::IAsn1Writer& writer, const Tag& tag, length_type content_length) const override;
		length_type encode_utc_time(io::IAsn1Writer& writer, const Tag& tag, const Time& time) const override;
		length_type encode_generalized_time(io::IAsn1Writer& writer, const Tag& tag, const Time& time) const override;

		length_type encode_octet_string(io::IAsn1Writer& writer, const Tag& tag, size_type block_size, const byte_type* data, size_type size) const;
		length_type encode_bit_string(io::IAsn1Writer& writer, const Tag& tag, uint8_t unused_bits, size_type block_size, const byte_type* data, size_type size) const;
	};

	struct DerEncoder : public IEncoder
	{
		length_type encode_header(io::IAsn1Writer& writer, const Tag& tag, length_type content_length) const override;
		length_type encode_utc_time(io::IAsn1Writer& writer, const Tag& tag, const Time& time) const override;
		length_type encode_generalized_time(io::IAsn1Writer& writer, const Tag& tag, const Time& time) const override;
	};
}

#endif
