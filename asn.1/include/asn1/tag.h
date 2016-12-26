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

#ifndef _ASN1_TAG_H_
#define _ASN1_TAG_H_

#include <cstdint>

namespace asn1
{
	enum class EncodingForm : uint8_t
	{ 
		PRIMITIVE = 0, 
		CONSTRUCTED, 
		UNDEFINED
	};

	enum class TagClass : uint8_t
	{ 
		UNIVERSAL = 0, 
		APPLICATION, 
		CONTEXT_SPECIFIC, 
		PRIVATE, 
		UNDEFINED
	};

	struct Tag
	{
		EncodingForm encoding_form;
		TagClass tag_class;
		uint64_t identifier;
		
		Tag()
			: encoding_form(EncodingForm::UNDEFINED), tag_class(TagClass::UNDEFINED), identifier(UINT64_MAX)
		{}

		constexpr Tag(EncodingForm encoding_form, TagClass tag_class, uint64_t identifier)
			: encoding_form(encoding_form), tag_class(tag_class), identifier(identifier)
		{}

		Tag(const Tag& tag) = default;
		Tag& operator=(const Tag& tag) = default;

		Tag(Tag&& tag)
			: encoding_form(tag.encoding_form), tag_class(tag.tag_class), identifier(tag.identifier)
		{
			tag.encoding_form = EncodingForm::UNDEFINED;
			tag.tag_class = TagClass::UNDEFINED;
			tag.identifier = UINT64_MAX;
		}

		Tag& operator=(Tag&& tag)
		{
			encoding_form = tag.encoding_form;
			tag_class = tag.tag_class;
			identifier = tag.identifier;

			tag.encoding_form = EncodingForm::UNDEFINED;
			tag.tag_class = TagClass::UNDEFINED;
			tag.identifier = UINT64_MAX;
			
			return *this;
		}

		bool operator==(const Tag& tag) const { return encoding_form == tag.encoding_form && tag_class == tag.tag_class && identifier == tag.identifier; }
		bool operator!=(const Tag& tag) const { return !operator==(tag); }
	};

	constexpr Tag UNDEFINED_TAG = { EncodingForm::UNDEFINED, TagClass::UNDEFINED, UINT64_MAX };
	constexpr Tag BMP_STRING_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 30 };
	constexpr Tag IA5_STRING_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 22 };
	constexpr Tag NUMERIC_STRING_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 18 };
	constexpr Tag T61_STRING_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 20 };
	constexpr Tag PRINTABLE_STRING_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 19 };
	constexpr Tag UTF8_STRING_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 12 };
	constexpr Tag TELETEX_STRING_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 20 };
	constexpr Tag VIDEOTEX_STRING_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 21 };
	constexpr Tag VISIBLE_STRING_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 26 };
	constexpr Tag BOOLEAN_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 1 };
	constexpr Tag INTEGER_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 2 };
	constexpr Tag BIT_STRING_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 3 };
	constexpr Tag UTC_TIME_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 23 };
	constexpr Tag GENERALIZED_TIME_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 24 };
	constexpr Tag OCTET_STRING_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 4 };
	constexpr Tag NULL_VALUE_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 5 };
	constexpr Tag OBJECT_IDENTIFIER_TAG = { EncodingForm::PRIMITIVE, TagClass::UNIVERSAL, 6 };
	constexpr Tag SEQUENCE_TAG = { EncodingForm::CONSTRUCTED, TagClass::UNIVERSAL, 16 };
	constexpr Tag SET_TAG = { EncodingForm::CONSTRUCTED, TagClass::UNIVERSAL, 17 };
}

#endif