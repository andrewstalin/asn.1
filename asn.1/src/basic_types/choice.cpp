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

#include "asn1/basic_types/choice.h"
#include "decoding.h"
#include <cassert>

using namespace asn1;

DecodingResult ChoiceDecoder::decode(const byte_type* data, size_type size)
{
	CHECK_PRECONDITIONS(data, size, finished());
	DecodingResult decoding_result = { 0, DecodingCode::UNDEFINED };

	while (state_ != State::COMPLETED)
	{
		switch (state_)
		{
		case State::INITIAL:
			{
				decoding_result << header_decoder_.decode(data, size);
				CHECK_RESULT(decoding_result);
				state_ = State::DECODER_DEFINITION;
			}
			break;
		case State::DECODER_DEFINITION:
			{
				for (const auto& decoder : choices_)
				{
					if (decoder->tag() == header_decoder_.tag())
					{
						decoder_ = decoder;
						decoder_->decode(header_decoder_.header_data(), header_decoder_.header_size());
						state_ = State::DECODE_VALUE;
						break;
					}
				}

				THROW_IF(decoder_ == nullptr, Asn1Exception(errors::UNKNOWN_CHOICE_VALUE));
			}
			break;
		case State::DECODE_VALUE:
			{
				assert(decoder_ != nullptr);
				decoding_result << decoder_->decode(data + decoding_result.read_bytes, size - decoding_result.read_bytes);
				
				if (decoding_result.code != DecodingCode::OK)
				{
					return decoding_result;
				}

				state_ = State::COMPLETED;
			}
			break;
		default:
			throw Asn1Exception(errors::WRONG_DECODER_STATE);
		}
	}

	return decoding_result;
}
