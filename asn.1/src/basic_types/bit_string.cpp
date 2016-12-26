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

#include "asn1/basic_types/bit_string.h"
#include "decoding.h"

using namespace asn1;

BitString BitString::parse(const std::string& bits)
{
	BitString bit_string;
	size_t count = bits.size() / CHAR_BIT + (bits.size() % CHAR_BIT == 0 ? 0 : 1);

	bit_string.bytes.resize(count);
	char bit_index{ 0 };
	size_t byte_index{ 0 };

	for (const auto& ch : bits)
	{
		bit_string.bytes[byte_index] |= ch == '1' ? (1 << bit_index) : 0;
		++bit_index;

		if (bit_index == CHAR_BIT)
		{
			bit_index = 0;
			++byte_index;
		}
	}

	return bit_string;
}

std::string BitString::to_string() const
{
	auto bytes_count = bytes.size();
	auto bits_count = static_cast<size_t>(bytes_count * CHAR_BIT - unused_bits);
	size_t first_bit_index{ 0 };

	for (auto i = 0u; i < bytes_count;)
	{
		auto b = bytes[i++];

		if (b > 0)
		{
			if (i == bytes_count && unused_bits > 0)
			{
				b &= ~unused_bits;
			}

			for (auto j = 0u; j < CHAR_BIT; ++j)
			{
				if (b & (0x80 >> j))
				{
					break;
				}

				++first_bit_index;
			}

			break;
		}

		first_bit_index += CHAR_BIT;
	}
	std::string result;

	if (bits_count > first_bit_index)
	{
		result.resize(bits_count - first_bit_index);
		size_t string_bit_index{ 0 };

		for (auto i = first_bit_index; i < bits_count; ++i)
		{
			const auto& byte = bytes[i / CHAR_BIT];
			result.at(string_bit_index++) = (byte & (0x80 >> (i % CHAR_BIT))) ? '1' : '0';
		}
	}

	return result;
}

DecodingResult BitStringDecoder::decode(const byte_type* data, size_type size)
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

			//check without encoding form (for constructed form)
			if (header_decoder_.tag().tag_class != tag().tag_class 
				|| header_decoder_.tag().identifier != tag().identifier)
			{
				throw Asn1Exception(errors::WRONG_TAG);
			}

			content_length_ = header_decoder_.content_length();
			value_.tag = header_decoder_.tag();

			if (content_length_ != INDEFINITE_CONTENT_LENGTH)
			{
				value_.bytes.reserve(static_cast<size_t>(content_length_));
			}

			if (header_decoder_.tag().encoding_form == EncodingForm::CONSTRUCTED)
			{
				constructed_type_decoder_ = std::make_unique<BitStringDecoder>(&constructed_type_event_handler_);
				state_ = State::BYTES_DEFINITION;
			}
			else
			{
				// The string can't be encoded in a primitive form with indefinite length
				THROW_IF(content_length_ == INDEFINITE_CONTENT_LENGTH, Asn1Exception(errors::WRONG_ELEMENT_LENGTH));
				state_ = State::UNUSED_BITS_DEFINITION;
			}
		}
		break;
		case State::UNUSED_BITS_DEFINITION:
		{
			uint8_t byte;
			READ_BYTE(decoding_result, data, byte);
			--content_length_;

			// The number of unused bits can be between 0 and 7
			THROW_IF(byte > 7, Asn1Exception(errors::WRONG_UNUSED_BITS_VALUE));


			value_.unused_bits = byte;
			state_ = State::BYTES_DEFINITION;
		}
		break;
		case State::BYTES_DEFINITION:
		{
			if (header_decoder_.tag().encoding_form == EncodingForm::CONSTRUCTED)
			{
				while (true)
				{
					auto dr = constructed_type_decoder_->decode(data + decoding_result.read_bytes, size - decoding_result.read_bytes);
					decoding_result << dr;

					if (content_length_ != INDEFINITE_CONTENT_LENGTH)
					{
						content_length_ -= dr.read_bytes;
					}

					if (decoding_result.code != DecodingCode::OK && decoding_result.code != DecodingCode::END_CONTENT)
					{
						return decoding_result;
					}

					if (content_length_ == INDEFINITE_CONTENT_LENGTH)
					{
						if (decoding_result.code == DecodingCode::END_CONTENT)
						{
							state_ = State::COMPLETED;
							decoding_result.code = DecodingCode::OK;
							on_decoding_completion(value_event_handler_, std::move(value_));

							return decoding_result;
						}
					}
					else
					{
						// Unexpected termination of the data
						THROW_IF(decoding_result.code == DecodingCode::END_CONTENT, Asn1Exception(errors::WRONG_STRUCTURE));

						if (content_length_ == 0)
						{
							state_ = State::COMPLETED;
							decoding_result.code = DecodingCode::OK;
							on_decoding_completion(value_event_handler_, std::move(value_));

							return decoding_result;
						}
					}

					constructed_type_decoder_->reset_state();
				}
			}
			else
			{
				auto remaining_size = size - decoding_result.read_bytes;

				if (content_length_ <= remaining_size)
				{
					value_.bytes.insert(value_.bytes.end(), data + decoding_result.read_bytes, data + decoding_result.read_bytes + static_cast<size_type>(content_length_));
					decoding_result.read_bytes += static_cast<size_type>(content_length_);
					content_length_ = 0;
					decoding_result.code = DecodingCode::OK;
					state_ = State::COMPLETED;
					on_decoding_completion(value_event_handler_, std::move(value_));
				}
				else
				{
					value_.bytes.insert(value_.bytes.end(), data + decoding_result.read_bytes, data + decoding_result.read_bytes + remaining_size);
					decoding_result.read_bytes += remaining_size;
					decoding_result.code = DecodingCode::MORE_DATA;
					content_length_ -= remaining_size;
				}

				return decoding_result;
			}
		}
		break;
		default:
			throw Asn1Exception(errors::WRONG_DECODER_STATE);
		}
	}

	return decoding_result;
}

void BitStringDecoder::consume(Asn1Value&& val)
{
	auto& bit_string = static_cast<BitString&>(val);
	value_.bytes.insert(value_.bytes.end(), bit_string.bytes.begin(), bit_string.bytes.end());

	THROW_IF(value_.unused_bits != 0, Asn1Exception(errors::WRONG_STRUCTURE));
	value_.unused_bits = bit_string.unused_bits;
}