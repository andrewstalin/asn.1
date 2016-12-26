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

#ifndef _ASN1_STREAM_ENCODER_H_
#define _ASN1_STREAM_ENCODER_H_

#include "asn1/value.h"
#include "asn1/error.h"
#include "optional/optional.h"
#include <limits>

namespace asn1
{
	struct StreamBlock
	{
		Tag tag;
		std::vector<byte_type> bytes;
		size_type size;

		StreamBlock(const Tag& tag, size_type block_size)
			: tag(tag), size(block_size)
		{
			bytes.reserve(block_size);
		}
	};

	namespace details
	{
		class StreamEncoder : public StreamingAsn1Value
		{
		protected:
			optional::OptionalValue<StreamBlock> stream_block_;
			length_type content_length_{ INDEFINITE_CONTENT_LENGTH };
			uint8_t unused_bits_{ (std::numeric_limits<uint8_t>::max)() };

		public:
			StreamEncoder(const Tag& tag)
				: StreamingAsn1Value(tag)
			{}

			StreamEncoder(const Tag& tag, length_type content_length)
				: StreamingAsn1Value(tag), content_length_(content_length)
			{}

			void set_content_length(length_type val) { content_length_ = val; }

			length_type evaluate_content_size(const IEncoder& encoder) const override;

		protected:
			virtual Tag get_encoded_tag() override;

			length_type encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer) override;
			length_type encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer) override;
			length_type encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer) override;

		private:
			length_type encode_buffer(const IEncoder& encoder, io::IAsn1Writer& writer, bool completed);
		};
	}
}

#endif
