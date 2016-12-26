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

#include "randomize.h"
#include <random>

uint8_t asn1::test::generate_random_number(uint8_t min_val, uint8_t max_val)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min_val, max_val);
	return static_cast<uint8_t>(dis(gen));
}

std::vector<std::vector<asn1::byte_type>> asn1::test::split_data(const std::vector<asn1::byte_type>& data)
{
	std::vector<std::vector<asn1::byte_type>> result;
	uint8_t index{ 0 };

	while (index < data.size())
	{
		auto chunk_length = generate_random_number(1, 5);

		if (index + chunk_length >= data.size())
		{
			chunk_length = static_cast<uint8_t>(data.size() - index);
		}

		auto chunk = std::vector<asn1::byte_type>(data.data() + index, data.data() + index + chunk_length);
		result.push_back(chunk);
		index += chunk_length;
	}

	return result;
}

std::vector<asn1::byte_type> asn1::test::generate_random_sequence()
{
	uint8_t count_bytes{ generate_random_number(3, 10) };
	std::vector<asn1::byte_type> result;
	result.reserve(count_bytes);

	for (auto i = 0u; i < count_bytes; ++i)
	{
		result.emplace_back(generate_random_number(0, UINT8_MAX));
	}

	return result;
}

std::vector<asn1::test::StepDecoding> asn1::test::generate_test_data(const std::vector<asn1::byte_type>& data)
{
	std::vector<StepDecoding> result;
	std::vector<asn1::byte_type> test_data(data.begin(), data.end());
	auto random_sequence = test::generate_random_sequence();
	test_data.insert(test_data.end(), random_sequence.begin(), random_sequence.end());
	uint32_t index{ 0 };

	while (index < test_data.size())
	{
		StepDecoding step_decoding;
		auto chunk_length = test::generate_random_number(1, 5);

		if (index + chunk_length >= test_data.size())
		{
			chunk_length = static_cast<uint8_t>(test_data.size() - index);
		}

		if (index + chunk_length >= data.size())
		{
			step_decoding.completed = true;
			step_decoding.unused_bytes = index > data.size() ? chunk_length : static_cast<uint8_t>(index + chunk_length - data.size());
		}

		step_decoding.chunk = std::vector<asn1::byte_type>(test_data.data() + index, test_data.data() + index + chunk_length);
		result.push_back(step_decoding);
		index += chunk_length;
	}

	return result;
}