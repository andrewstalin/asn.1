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

#ifndef _ASN1_ASN1_WRITER_H_
#define _ASN1_ASN1_WRITER_H_

#include "asn1/type_traits.h"
#include "asn1/error.h"
#include <stdint.h>
#include <vector>

namespace asn1
{
	namespace io
	{
		struct IAsn1Writer
		{
			virtual void put(byte_type ch) = 0;
			virtual void write(const byte_type* data, size_type count) = 0;
		};

		struct BufferedWriter : IAsn1Writer
		{
			std::vector<byte_type> bytes;

			BufferedWriter()
			{}

			explicit BufferedWriter(size_t capacity)
			{
				bytes.reserve(capacity);
			}

			void reset(size_t capacity)
			{
				bytes = std::vector<byte_type>();
				bytes.reserve(capacity);
			}

			void put(byte_type ch) override
			{
				bytes.push_back(ch);
			}

			void write(const byte_type* data, size_type count) override
			{
				if (data != nullptr && count > 0)
				{
					bytes.insert(bytes.end(), data, data + count);
				}
			}
		};

		struct ArrayWriter : IAsn1Writer
		{
			size_t array_size;
			byte_type* ptr;

			ArrayWriter(byte_type* ptr, size_t array_size)
				: array_size(array_size), ptr(ptr)
			{}

			void put(byte_type ch) override
			{
				THROW_IF(array_size == 0, asn1::Asn1Exception(errors::INSUFFICIENT_BUFFER_SIZE));
				*ptr = ch;
				ptr++;
				--array_size;
			}

			void write(const byte_type* data, size_type count) override
			{
				if (data != nullptr && count > 0)
				{
					THROW_IF(array_size < count, asn1::Asn1Exception(errors::INSUFFICIENT_BUFFER_SIZE));
					std::memcpy(ptr, data, static_cast<size_t>(count));
					ptr += count;
					array_size -= static_cast<size_t>(count);
				}
			}
		};

		struct NullWriter : IAsn1Writer
		{
			void put(byte_type ch) override {}
			void write(const byte_type* data, size_type count) override {}
		};
	}
}

#endif