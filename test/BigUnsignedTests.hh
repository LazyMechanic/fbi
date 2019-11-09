#pragma once

#pragma warning(push)
// Disable gtest warnings
#pragma warning(disable : 26495)
#pragma warning(disable : 26812)

#include <array>
#include <numeric>
#include <string>

#include <gtest/gtest.h>

#include <fbi/fbi.hh>

using namespace fbi;

namespace bigunsigned {
template <typename T>
void testPrimitiveConstructor()
{
    T primitive = 0;
    for (auto i = 0;; primitive |= T{ 0x1 } << i, ++i) {
        ASSERT_EQ(BigUnsigned{ primitive }, primitive);

        if (primitive == std::numeric_limits<T>::max()) {
            break;
        }
    }
}

template <typename T>
void testConvertToPrimitive(const std::function<T(const BigUnsigned&)>& converter)
{
    T primitive = 0;
    for (auto i = 0;; primitive |= T{ 0x1 } << i, ++i) {
        BigUnsigned bigInt{ primitive };
        ASSERT_EQ(converter(bigInt), primitive);

        if (primitive == std::numeric_limits<T>::max()) {
            break;
        }
    }
}

template <typename T>
void testConvertFromStringPrimitive()
{
    T primitive = 0;
    for (auto i = 0;; primitive |= T{ 0x1 } << i, ++i) {
        std::string str = std::to_string(primitive);
        ASSERT_EQ(BigUnsigned{ str }, primitive);

        if (primitive == std::numeric_limits<T>::max()) {
            break;
        }
    }
}

template <std::size_t N>
void testConvertFromString(const std::string& str, const std::array<unsigned long long, N>& blocks)
{
    BigUnsigned bigInt{ str };
    ASSERT_EQ(bigInt.getLength(), blocks.size());
    for (std::size_t i = 0; i < blocks.size(); ++i) {
        ASSERT_EQ(bigInt.getBlock(i), blocks[i]);
    }
}

template <std::size_t N>
void testConvertToString(const std::string& str, std::array<unsigned long long, N> blocks)
{
    BigUnsigned bigInt{ blocks.data(), blocks.size() };
    ASSERT_EQ(bigUnsignedToString(bigInt), str);
}

template <std::size_t N>
void testConvertToStringMemberFunc(const std::string& str, std::array<unsigned long long, N> blocks)
{
    BigUnsigned bigInt{ blocks.data(), blocks.size() };
    ASSERT_EQ(bigInt.toString(), str);
}
} // namespace bigunsigned

TEST(BigUnsignedConstruct, Default)
{
    /* Default constructors */
    ASSERT_EQ(BigUnsigned{}, 0);
}

TEST(BigUnsignedConstruct, FromPrimitive)
{
    /* Constructor from primitive */

    using namespace bigunsigned;

    testPrimitiveConstructor<unsigned long long>();
    testPrimitiveConstructor<unsigned long>();
    testPrimitiveConstructor<unsigned int>();
    testPrimitiveConstructor<unsigned short>();
    testPrimitiveConstructor<unsigned char>();

    testPrimitiveConstructor<long long>();
    testPrimitiveConstructor<long>();
    testPrimitiveConstructor<int>();
    testPrimitiveConstructor<short>();
    testPrimitiveConstructor<char>();
}

TEST(BigUnsignedConstruct, FromBlockArray)
{
    /* Block-array constructors */

    /* Zero-blocks */
    {
        std::array<BigUnsigned::Blk, 1> blocks{ 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        ASSERT_EQ(bigInt, 0);
        ASSERT_EQ(bigInt.bitLength(), 0);
        ASSERT_EQ(bigInt.getLength(), 0);
    }
    {
        std::array<BigUnsigned::Blk, 3> blocks{ 0, 0, 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        ASSERT_EQ(bigInt, 0);
        ASSERT_EQ(bigInt.bitLength(), 0);
        ASSERT_EQ(bigInt.getLength(), 0);
    }
    {
        std::array<BigUnsigned::Blk, 10> blocks{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        ASSERT_EQ(bigInt, 0);
        ASSERT_EQ(bigInt.bitLength(), 0);
        ASSERT_EQ(bigInt.getLength(), 0);
    }

    {
        std::array<BigUnsigned::Blk, 2> blocks{ 7777777, 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        ASSERT_EQ(bigInt, 7777777);
        ASSERT_EQ(bigInt.bitLength(), 23);
        ASSERT_EQ(bigInt.getLength(), 1);
        ASSERT_EQ(bigInt.getBlock(0), 7777777);
    }
    {
        std::array<BigUnsigned::Blk, 3> blocks{ 7777777, 0, 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        ASSERT_EQ(bigInt, 7777777);
        ASSERT_EQ(bigInt.bitLength(), 23);
        ASSERT_EQ(bigInt.getLength(), 1);
        ASSERT_EQ(bigInt.getBlock(0), 7777777);
    }
    {
        std::array<BigUnsigned::Blk, 10> blocks{ 7777777, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        ASSERT_EQ(bigInt, 7777777);
        ASSERT_EQ(bigInt.bitLength(), 23);
        ASSERT_EQ(bigInt.getLength(), 1);
        ASSERT_EQ(bigInt.getBlock(0), 7777777);
    }

    /* Big integer which do not fit into primitive types */
    {
        /*
         * Big integer in memory:
         * 000....000|000....100|000....011
         * \~~~64~~~/ \~~~64~~~/ \~~~64~~~/
         *     =0         =4         =3
         *                   \~~~~~67~~~~~/
         */
        std::array<BigUnsigned::Blk, 3> blocks{ 3, 4, 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        ASSERT_EQ(bigInt.bitLength(), 67);
        ASSERT_EQ(bigInt.getLength(), 2);
        ASSERT_EQ(bigInt.getBlock(0), 3);
        ASSERT_EQ(bigInt.getBlock(1), 4);
    }
    {
        /*
         * Big integer in memory:
         * 000....01100010101|000....0101010011000101011000111|000....0100|000....011|000....0110110010010010010
         * \~~~~~~~64~~~~~~~/ \~~~~~~~~~~~~~~64~~~~~~~~~~~~~~/ \~~~64~~~~/ \~~~64~~~/ \~~~~~~~~~~64~~~~~~~~~~~~/
         *        =789                   =11111111                 =4          =3             =222354
         *         \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~266~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/
         */
        std::array<BigUnsigned::Blk, 5> blocks{ 222354, 3, 4, 11111111, 789 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        ASSERT_EQ(bigInt.bitLength(), 266);
        ASSERT_EQ(bigInt.getLength(), 5);
        ASSERT_EQ(bigInt.getBlock(0), 222354);
        ASSERT_EQ(bigInt.getBlock(1), 3);
        ASSERT_EQ(bigInt.getBlock(2), 4);
        ASSERT_EQ(bigInt.getBlock(3), 11111111);
        ASSERT_EQ(bigInt.getBlock(4), 789);
    }
}

TEST(BigUnsignedConstruct, FromString)
{
    // clang-format off

    ASSERT_EQ(BigUnsigned{ "1" }.toString(), "1");
    ASSERT_EQ(BigUnsigned{ "10" }.toString(), "10");
    ASSERT_EQ(BigUnsigned{ "100" }.toString(), "100");
    ASSERT_EQ(BigUnsigned{ "1000" }.toString(), "1000");
    ASSERT_EQ(BigUnsigned{ "10000" }.toString(), "10000");
    ASSERT_EQ(BigUnsigned{ "100000" }.toString(), "100000");
    ASSERT_EQ(BigUnsigned{ "1000000" }.toString(), "1000000");
    ASSERT_EQ(BigUnsigned{ "10000000" }.toString(), "10000000");
    ASSERT_EQ(BigUnsigned{ "100000000" }.toString(), "100000000");
    ASSERT_EQ(BigUnsigned{ "1000000000" }.toString(), "1000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000" }.toString(), "10000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000" }.toString(), "100000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000" }.toString(), "1000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000" }.toString(), "10000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000" }.toString(), "100000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000" }.toString(), "1000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000" }.toString(), "10000000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000000" }.toString(), "100000000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000000" }.toString(), "1000000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000000" }.toString(), "10000000000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000000000" }.toString(), "100000000000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000000000" }.toString(), "1000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000000000" }.toString(), "10000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000000000000" }.toString(), "100000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000000000000" }.toString(), "1000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000000000000" }.toString(), "10000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000000000000000" }.toString(), "100000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000000000000000" }.toString(), "1000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000000000000000" }.toString(), "10000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000000000000000000" }.toString(), "100000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000000000000000000" }.toString(), "1000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000000000000000000" }.toString(), "10000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000000000000000000000" }.toString(), "100000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000000000000000000000000" }.toString(), "100000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000000000000000000000000000" }.toString(), "100000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000000000000000000000000000000" }.toString(), "100000000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000000000000000000000000000000000" }.toString(), "100000000000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "100000000000000000000000000000000000000000000000" }.toString(), "100000000000000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "1000000000000000000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000000000000000000");
    ASSERT_EQ(BigUnsigned{ "10000000000000000000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000000000000000000");

    // clang-format on
}

TEST(BigUnsignedConvert, ToPrimitive)
{
    /* Convert to primitive */

    using namespace bigunsigned;

    testConvertToPrimitive<unsigned long long>([](const auto& b) { return b.toUnsignedLongLong(); });
    testConvertToPrimitive<unsigned long>([](const auto& b) { return b.toUnsignedLong(); });
    testConvertToPrimitive<unsigned int>([](const auto& b) { return b.toUnsignedInt(); });
    testConvertToPrimitive<unsigned short>([](const auto& b) { return b.toUnsignedShort(); });
    testConvertToPrimitive<unsigned char>([](const auto& b) { return b.toUnsignedChar(); });

    testConvertToPrimitive<long long>([](const auto& b) { return b.toLongLong(); });
    testConvertToPrimitive<long>([](const auto& b) { return b.toLong(); });
    testConvertToPrimitive<int>([](const auto& b) { return b.toInt(); });
    testConvertToPrimitive<short>([](const auto& b) { return b.toShort(); });
    testConvertToPrimitive<char>([](const auto& b) { return b.toChar(); });
}

TEST(BigUnsignedConvert, ToString)
{
    using namespace bigunsigned;
    // clang-format off
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 0 }), "0");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 1 }), "1");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 10 }), "10");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 100 }), "100");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 1000 }), "1000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 10000 }), "10000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 100000 }), "100000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 1000000 }), "1000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 10000000 }), "10000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 100000000 }), "100000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 1000000000 }), "1000000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 10000000000 }), "10000000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 100000000000 }), "100000000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 1000000000000 }), "1000000000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 10000000000000 }), "10000000000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 100000000000000 }), "100000000000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 1000000000000000 }), "1000000000000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 10000000000000000 }), "10000000000000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 100000000000000000 }), "100000000000000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 1000000000000000000 }), "1000000000000000000");
    ASSERT_EQ(bigUnsignedToString(BigUnsigned{ 10000000000000000000ull }), "10000000000000000000");

    testConvertToString<2>("100000000000000000000", { 7766279631452241920ull, 5ull });
    testConvertToString<2>("1000000000000000000000", { 3875820019684212736ull, 54ull });
    testConvertToString<2>("10000000000000000000000", { 1864712049423024128ull, 542ull });
    testConvertToString<2>("100000000000000000000000", { 200376420520689664ull, 5421ull });
    testConvertToString<2>("1000000000000000000000000", { 2003764205206896640ull, 54210ull });
    testConvertToString<2>("10000000000000000000000000", { 1590897978359414784ull, 542101ull });
    testConvertToString<2>("100000000000000000000000000", { 15908979783594147840ull, 5421010ull });
    testConvertToString<2>("1000000000000000000000000000", { 11515845246265065472ull, 54210108ull });
    testConvertToString<2>("10000000000000000000000000000", { 4477988020393345024ull, 542101086ull });
    testConvertToString<2>("100000000000000000000000000000", { 7886392056514347008ull, 5421010862ull });
    testConvertToString<2>("1000000000000000000000000000000", { 5076944270305263616ull, 54210108624ull });
    testConvertToString<2>("10000000000000000000000000000000", { 13875954555633532928ull, 542101086242ull });
    testConvertToString<2>("100000000000000000000000000000000", { 9632337040368467968ull, 5421010862427ull });
    testConvertToString<2>("1000000000000000000000000000000000", { 4089650035136921600ull, 54210108624275ull });
    testConvertToString<2>("10000000000000000000000000000000000", { 4003012203950112768ull, 542101086242752ull });
    testConvertToString<2>("100000000000000000000000000000000000", { 3136633892082024448ull, 5421010862427522ull });
    testConvertToString<2>("1000000000000000000000000000000000000", { 12919594847110692864ull, 54210108624275221ull });
    testConvertToString<2>("10000000000000000000000000000000000000", { 68739955140067328ull, 542101086242752217ull });
    testConvertToString<2>("100000000000000000000000000000000000000", { 687399551400673280ull, 5421010862427522170ull });
    testConvertToString<3>("1000000000000000000000000000000000000000", { 6873995514006732800ull, 17316620476856118468ull, 2ull });
    testConvertToString<3>("10000000000000000000000000000000000000000", { 13399722918938673152ull, 7145508105175220139ull, 29ull });
    testConvertToString<3>("100000000000000000000000000000000000000000", { 4870020673419870208ull, 16114848830623546549ull, 293ull });

    ASSERT_EQ(BigUnsigned{ 0 }.toString(), "0");
    ASSERT_EQ(BigUnsigned{ 1 }.toString(), "1");
    ASSERT_EQ(BigUnsigned{ 10 }.toString(), "10");
    ASSERT_EQ(BigUnsigned{ 100 }.toString(), "100");
    ASSERT_EQ(BigUnsigned{ 1000 }.toString(), "1000");
    ASSERT_EQ(BigUnsigned{ 10000 }.toString(), "10000");
    ASSERT_EQ(BigUnsigned{ 100000 }.toString(), "100000");
    ASSERT_EQ(BigUnsigned{ 1000000 }.toString(), "1000000");
    ASSERT_EQ(BigUnsigned{ 10000000 }.toString(), "10000000");
    ASSERT_EQ(BigUnsigned{ 100000000 }.toString(), "100000000");
    ASSERT_EQ(BigUnsigned{ 1000000000 }.toString(), "1000000000");
    ASSERT_EQ(BigUnsigned{ 10000000000 }.toString(), "10000000000");
    ASSERT_EQ(BigUnsigned{ 100000000000 }.toString(), "100000000000");
    ASSERT_EQ(BigUnsigned{ 1000000000000 }.toString(), "1000000000000");
    ASSERT_EQ(BigUnsigned{ 10000000000000 }.toString(), "10000000000000");
    ASSERT_EQ(BigUnsigned{ 100000000000000 }.toString(), "100000000000000");
    ASSERT_EQ(BigUnsigned{ 1000000000000000 }.toString(), "1000000000000000");
    ASSERT_EQ(BigUnsigned{ 10000000000000000 }.toString(), "10000000000000000");
    ASSERT_EQ(BigUnsigned{ 100000000000000000 }.toString(), "100000000000000000");
    ASSERT_EQ(BigUnsigned{ 1000000000000000000 }.toString(), "1000000000000000000");
    ASSERT_EQ(BigUnsigned{ 10000000000000000000ull }.toString(), "10000000000000000000");

    testConvertToStringMemberFunc<2>("100000000000000000000", { 7766279631452241920ull, 5ull });
    testConvertToStringMemberFunc<2>("1000000000000000000000", { 3875820019684212736ull, 54ull });
    testConvertToStringMemberFunc<2>("10000000000000000000000", { 1864712049423024128ull, 542ull });
    testConvertToStringMemberFunc<2>("100000000000000000000000", { 200376420520689664ull, 5421ull });
    testConvertToStringMemberFunc<2>("1000000000000000000000000", { 2003764205206896640ull, 54210ull });
    testConvertToStringMemberFunc<2>("10000000000000000000000000", { 1590897978359414784ull, 542101ull });
    testConvertToStringMemberFunc<2>("100000000000000000000000000", { 15908979783594147840ull, 5421010ull });
    testConvertToStringMemberFunc<2>("1000000000000000000000000000", { 11515845246265065472ull, 54210108ull });
    testConvertToStringMemberFunc<2>("10000000000000000000000000000", { 4477988020393345024ull, 542101086ull });
    testConvertToStringMemberFunc<2>("100000000000000000000000000000", { 7886392056514347008ull, 5421010862ull });
    testConvertToStringMemberFunc<2>("1000000000000000000000000000000", { 5076944270305263616ull, 54210108624ull });
    testConvertToStringMemberFunc<2>("10000000000000000000000000000000", { 13875954555633532928ull, 542101086242ull });
    testConvertToStringMemberFunc<2>("100000000000000000000000000000000", { 9632337040368467968ull, 5421010862427ull });
    testConvertToStringMemberFunc<2>("1000000000000000000000000000000000", { 4089650035136921600ull, 54210108624275ull });
    testConvertToStringMemberFunc<2>("10000000000000000000000000000000000", { 4003012203950112768ull, 542101086242752ull });
    testConvertToStringMemberFunc<2>("100000000000000000000000000000000000", { 3136633892082024448ull, 5421010862427522ull });
    testConvertToStringMemberFunc<2>("1000000000000000000000000000000000000", { 12919594847110692864ull, 54210108624275221ull });
    testConvertToStringMemberFunc<2>("10000000000000000000000000000000000000", { 68739955140067328ull, 542101086242752217ull });
    testConvertToStringMemberFunc<2>("100000000000000000000000000000000000000", { 687399551400673280ull, 5421010862427522170ull });
    testConvertToStringMemberFunc<3>("1000000000000000000000000000000000000000", { 6873995514006732800ull, 17316620476856118468ull, 2ull });
    testConvertToStringMemberFunc<3>("10000000000000000000000000000000000000000", { 13399722918938673152ull, 7145508105175220139ull, 29ull });
    testConvertToStringMemberFunc<3>("100000000000000000000000000000000000000000", { 4870020673419870208ull, 16114848830623546549ull, 293ull });

    // clang-format on
}

TEST(BigUnsignedConvert, FromString)
{
    using namespace bigunsigned;

    testConvertFromStringPrimitive<unsigned long long>();
    testConvertFromStringPrimitive<unsigned long>();
    testConvertFromStringPrimitive<unsigned int>();
    testConvertFromStringPrimitive<unsigned short>();
    testConvertFromStringPrimitive<unsigned char>();

    testConvertFromStringPrimitive<long long>();
    testConvertFromStringPrimitive<long>();
    testConvertFromStringPrimitive<int>();
    testConvertFromStringPrimitive<short>();
    testConvertFromStringPrimitive<char>();

    /* Big integer which do not fit into primitive types */

    // clang-format off
    ASSERT_EQ(stringToBigUnsigned("100000000000000000000").toString(), "100000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("1000000000000000000000").toString(), "1000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("10000000000000000000000").toString(), "10000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("100000000000000000000000").toString(), "100000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("1000000000000000000000000").toString(), "1000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("10000000000000000000000000").toString(), "10000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("100000000000000000000000000").toString(), "100000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("1000000000000000000000000000").toString(), "1000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("10000000000000000000000000000").toString(), "10000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("100000000000000000000000000000").toString(), "100000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("1000000000000000000000000000000").toString(), "1000000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("10000000000000000000000000000000").toString(), "10000000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("100000000000000000000000000000000").toString(), "100000000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("1000000000000000000000000000000000").toString(), "1000000000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("10000000000000000000000000000000000").toString(), "10000000000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("100000000000000000000000000000000000").toString(), "100000000000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("1000000000000000000000000000000000000").toString(), "1000000000000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("10000000000000000000000000000000000000").toString(), "10000000000000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("100000000000000000000000000000000000000").toString(), "100000000000000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("1000000000000000000000000000000000000000").toString(), "1000000000000000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("10000000000000000000000000000000000000000").toString(), "10000000000000000000000000000000000000000");
    ASSERT_EQ(stringToBigUnsigned("100000000000000000000000000000000000000000").toString(), "100000000000000000000000000000000000000000");
    // clang-format on

    /* Very big number */

    // clang-format off
    ASSERT_EQ(stringToBigUnsigned("100000000000000000000000000000000000000000"
                                  "781231233333334778977777999636666666666666"
                                  "777891111111111111111111333343458566666666"
                                  "112377777777777777777777777777777777777777").toString(),
              "100000000000000000000000000000000000000000"
              "781231233333334778977777999636666666666666"
              "777891111111111111111111333343458566666666"
              "112377777777777777777777777777777777777777");
    // clang-format on
}

#pragma warning(pop)