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

#ifndef _ASN1_TESTS_BASIC_DECODER_TEST_H_
#define _ASN1_TESTS_BASIC_DECODER_TEST_H_

#include "asn1/decoder.h"
#include <vector>
#include <functional>

namespace asn1
{
	namespace test
	{
		struct BasicDecoderTestEventHandler : IValueEventHandler
		{
			std::function<void(Asn1Value&& val)> assertion;
			Tag explicit_tag;
			bool fire{ false };

			BasicDecoderTestEventHandler()
			{}

			BasicDecoderTestEventHandler(const std::function<void(Asn1Value&& val)>& assertion)
				: assertion(assertion)
			{}

			void execute(Asn1Value&& val) override
			{
				fire = true;

				if (assertion)
				{
					assertion(std::move(val));
				}
			}

			void execute(const Tag& tag, Asn1Value&& val) override
			{
				explicit_tag = tag;
				execute(std::move(val));
			}
		};

		struct StreamDecoderTestDataHandler : IDataEventHandler
		{
			std::vector<byte_type> result;

			void execute(const byte_type* data, size_type size) override
			{
				result.insert(result.end(), data, data + size);
			}
		};

		struct BasicDecoderTest
		{
			asn1::BerDecoder* decoder;
			bool print_debug{ false };

			BasicDecoderTest(asn1::BerDecoder* decoder)
				: decoder(decoder)
			{}

			void execute(const std::vector<asn1::byte_type>& bytes);
		};
	}
}

#endif