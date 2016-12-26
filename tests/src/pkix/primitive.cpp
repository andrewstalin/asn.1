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

#include "structured_type_test.h"
#include "file.h"
#include "asn1/pkix/primitive/algorithm_identifier.h"
#include "asn1/pkix/primitive/subject_public_key_info.h"
#include "asn1/pkix/primitive/attribute_type_and_value.h"
#include "asn1/pkix/primitive/name.h"
#include "asn1/pkix/primitive/relative_distinguished_name.h"
#include "babel/encoding.h"
#include "gtest/gtest.h"

using namespace asn1;

template<typename value_type>
struct RDNValueEventHandler : IValueEventHandler
{
	std::string expected;
	bool fire{ false };

	RDNValueEventHandler(const std::string& expected)
		: expected(expected)
	{}

	void execute(Asn1Value&& val) override
	{
		fire = true;
		const auto& rdn = static_cast<value_type&>(val);
		EXPECT_EQ(expected, rdn.to_string());
	}

	void execute(const Tag& tag, Asn1Value&& val) override {}
};

TEST(algorithm_identifier_case, decode_encode_test)
{
	test::StructeredTypeTest<pkix::AlgorithmIdentifierDecoder>(test::load_file("data/algorithm_identifier1")).execute();
}

TEST(algorithm_identifier_case, decode_encode_with_parameters_test)
{
	test::StructeredTypeTest<pkix::AlgorithmIdentifierDecoder>(test::load_file("data/algorithm_identifier2")).execute();
}

TEST(subject_public_key_info_case, decode_encode_test)
{
	test::StructeredTypeTest<pkix::SubjectPublicKeyInfoDecoder>(test::load_file("data/subject_public_key_info")).execute();
}

TEST(attribute_type_and_value_case, decode_encode_test)
{
	test::StructeredTypeTest<pkix::AttributeTypeAndValueDecoder>(test::load_file("data/attribute_type_and_value")).execute();
}

TEST(relative_distinguished_name_case, decode_encode_test)
{
	test::StructeredTypeTest<pkix::RelativeDistinguishedNameDecoder>(test::load_file("data/relative_distinguished_name")).execute();
}

TEST(relative_distinguished_name_case, to_string_test)
{
	RDNValueEventHandler<pkix::RelativeDistinguishedName> value_event_handler("STREET=Проспект Космонавтов д. 56 + ИНН=006663003127");
	auto bytes = test::load_file("data/relative_distinguished_name");
	
	pkix::RelativeDistinguishedNameDecoder decoder(&value_event_handler);
	auto decoding_result = decoder.decode(bytes.data(), bytes.size());

	ASSERT_EQ(decoding_result.code, DecodingCode::OK);
	EXPECT_TRUE(value_event_handler.fire);
}

TEST(name_case, decode_encode_test)
{
	test::StructeredTypeTest<pkix::NameDecoder>(test::load_file("data/name")).execute();
}

TEST(name_case, to_string_test)
{
	RDNValueEventHandler<pkix::Name> value_event_handler("ОГРН=1026605606620,\rИНН=006663003127,\rSTREET=Проспект Космонавтов д. 56,\rE=ca@skbkontur.ru,\rC=RU,\rS=66 Свердловская область,\rL=Екатеринбург,\rO=ЗАО \"ПФ \"СКБ Контур\",\rOU=Удостоверяющий центр,\rCN=SKB Kontur production CA 3");
	auto bytes = test::load_file("data/name");

	pkix::NameDecoder decoder(&value_event_handler);
	auto decoding_result = decoder.decode(bytes.data(), bytes.size());

	ASSERT_EQ(decoding_result.code, DecodingCode::OK);
	EXPECT_TRUE(value_event_handler.fire);
}