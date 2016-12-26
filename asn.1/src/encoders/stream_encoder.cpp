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

#include "asn1/encoders/stream_encoder.h"
#include "asn1/error.h"

using namespace asn1;

length_type details::StreamEncoder::evaluate_content_size(const IEncoder& encoder) const
{
	if (content_length_ == INDEFINITE_CONTENT_LENGTH)
	{
		return INDEFINITE_CONTENT_LENGTH;
	}

	length_type result{ 0 };
	auto is_bit_string = unused_bits_ != (std::numeric_limits<uint8_t>::max)();

	if (stream_block_)
	{
		const auto& block = stream_block_.value();

		// calculate total structured content length
		io::NullWriter null_writer;
		auto header_size = encoder.encode_header(null_writer, block.tag, block.size);
		auto blocks_count = content_length_ / block.size;
		result += content_length_ + blocks_count * header_size;

		// add unused bits byte
		if (is_bit_string)
		{
			result += blocks_count;
		}

		// not even number of blocks
		if (content_length_ % block.size > 0)
		{
			if (is_bit_string)
			{
				++result;
			}

			result += encoder.encode_header(null_writer, block.tag, content_length_ - block.size * blocks_count);
		}
	}
	else
	{
		result = content_length_;

		// add unused bits byte
		if (is_bit_string)
		{
			++result;
		}
	}

	return result;
}

Tag asn1::details::StreamEncoder::get_encoded_tag()
{
	auto tag = StreamingAsn1Value::get_encoded_tag();

	if (stream_block_)
	{
		tag.encoding_form = EncodingForm::CONSTRUCTED;
	}

	return tag;
}

length_type details::StreamEncoder::encode_first_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	THROW_IF(content_length_ == INDEFINITE_CONTENT_LENGTH && !stream_block_, Asn1Exception(errors::WRONG_STRUCTURE));
	length_type count{ 0 };

	// write unused bits byte
	if (unused_bits_ != (std::numeric_limits<uint8_t>::max)() && !stream_block_)
	{
		writer.put(unused_bits_);
		++count;
	}

	return count;
}

length_type details::StreamEncoder::encode_part(const IEncoder& encoder, const byte_type* data, size_type size, io::IAsn1Writer& writer)
{
	length_type count{ 0 };

	if (data != nullptr && size > 0)
	{
		if (stream_block_)
		{
			auto& block = stream_block_.value();

			// if we have data to write and all bytes received
			if (content_length_ == INDEFINITE_CONTENT_LENGTH || content_length_ > 0)
			{
				while (size > 0)
				{
					// block is ready for write
					if (block.bytes.size() == block.size)
					{
						count += encode_buffer(encoder, writer, false);
					}

					auto remaining_bytes_count = block.size - block.bytes.size();
					auto consumed = remaining_bytes_count >= size ? size : remaining_bytes_count;
					block.bytes.insert(block.bytes.end(), data, data + consumed);
					data += consumed;
					size -= consumed;
					count += consumed;

					if (content_length_ != INDEFINITE_CONTENT_LENGTH)
					{
						content_length_ -= consumed;
					}
				}
			}
		}
		else
		{
			if (content_length_ > 0)
			{
				size_type read_bytes{ content_length_ >= size ? size : static_cast<size_type>(content_length_) };
				writer.write(data, read_bytes);
				content_length_ -= read_bytes;
				count += read_bytes;
			}
		}
	}

	return count;
}

length_type details::StreamEncoder::encode_last_part(const IEncoder& encoder, io::IAsn1Writer& writer)
{
	length_type count{ 0 };

	if (stream_block_)
	{
		auto& block = stream_block_.value();

		// write not empty block
		if (!block.bytes.empty())
		{
			count += encode_buffer(encoder, writer, true);
		}
	}
	else
	{
		THROW_IF(content_length_ != 0, Asn1Exception(errors::MORE_DATA));
	}

	return count;
}

uint64_t details::StreamEncoder::encode_buffer(const IEncoder& encoder, io::IAsn1Writer& writer, bool completed)
{
	auto& block = stream_block_.value();
	auto is_bit_string = unused_bits_ != (std::numeric_limits<uint8_t>::max)();
	auto buffer_size = block.bytes.size();
	uint64_t count{ buffer_size };
	count += encoder.encode_header(writer, block.tag, buffer_size + (is_bit_string ? 1 : 0));

	// write unused bits byte
	if (is_bit_string)
	{
		completed ? writer.put(unused_bits_) : writer.put(0);
		++count;
	}

	writer.write(block.bytes.data(), buffer_size);
	block.bytes.clear();
	return count;
}