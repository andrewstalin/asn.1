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

#ifndef _ASN1_TESTS_STRUCTURED_TYPE_TEST_H_
#define _ASN1_TESTS_STRUCTURED_TYPE_TEST_H_

#include "randomize.h"
#include "hex.h"
#include "asn1/type_traits.h"
#include "asn1/decoder.h"
#include "asn1/encoders/encoder.h"
#include "gtest/gtest.h"
#include <vector>

namespace asn1
{
	namespace test
	{
		struct StructeredTypeTestEventHandler : IValueEventHandler
		{
			std::vector<byte_type> bytes;
			Tag explicit_tag;
			bool fire{ false };

			StructeredTypeTestEventHandler(const std::vector<byte_type>& bytes)
				: bytes(bytes)
			{}

			void execute(Asn1Value&& val) override
			{
				fire = true;

				io::BufferedWriter writer(1024 * 4);
				val.encode(DerEncoder(), writer);

				EXPECT_EQ(bytes, writer.bytes);
			}

			void execute(const Tag& tag, Asn1Value&& val) override
			{
				explicit_tag = tag;
				execute(std::move(val));
			}
		};

		template<typename decoder_type>
		struct StructeredTypeTest
		{
			StructeredTypeTestEventHandler value_event_handler;
			std::vector<byte_type> bytes;
			decoder_type decoder;
			bool print_debug{ false };

			StructeredTypeTest(const std::vector<byte_type>& bytes)
				: value_event_handler(bytes), bytes(bytes), decoder(&value_event_handler)
			{}

			StructeredTypeTest(const std::vector<byte_type>& bytes, const std::vector<byte_type>& new_bytes, IDataEventHandler* const data_event_handler)
				: value_event_handler(new_bytes), bytes(bytes), decoder(&value_event_handler, data_event_handler)
			{}

			void execute()
			{
				if (print_debug)
				{
					std::cout << "asn.1 data: " << to_hex(bytes) << std::endl;
				}

				auto test_data = test::generate_test_data(bytes);

				if (print_debug)
				{
					for (auto& chunk : test_data)
					{
						std::cout << "unused bytes: " << std::to_string(chunk.unused_bytes) << ", chunk data: " << to_hex(chunk.chunk) << std::endl;
					}
				}

				//decoder.decode(bytes.data(), bytes.size());
				for (const auto& chunk : test_data)
				{
					auto dr = decoder.decode(chunk.chunk.data(), chunk.chunk.size());
					EXPECT_EQ(chunk.chunk.size() - chunk.unused_bytes, dr.read_bytes);
					EXPECT_EQ(chunk.completed ? asn1::DecodingCode::OK : asn1::DecodingCode::MORE_DATA, dr.code);
				}

				EXPECT_TRUE(value_event_handler.fire);
				EXPECT_TRUE(decoder.finished());
			}
		};
	}
}

#endif