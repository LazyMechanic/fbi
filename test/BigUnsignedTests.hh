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
        EXPECT_EQ(BigUnsigned{ primitive }, primitive);

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
        EXPECT_EQ(converter(bigInt), primitive);

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
        EXPECT_EQ(BigUnsigned{ str }, primitive);

        if (primitive == std::numeric_limits<T>::max()) {
            break;
        }
    }
}

template <std::size_t N>
void testConvertFromString(const std::string& str, const std::array<unsigned long long, N>& blocks)
{
    BigUnsigned bigInt{ str };
    EXPECT_EQ(bigInt.getLength(), blocks.size());
    for (std::size_t i = 0; i < blocks.size(); ++i) {
        EXPECT_EQ(bigInt.getBlock(i), blocks[i]);
    }
}

template <std::size_t N>
void testConvertToString(const std::string& str, std::array<unsigned long long, N> blocks)
{
    BigUnsigned bigInt{ blocks.data(), blocks.size() };
    EXPECT_EQ(bigUnsignedToString(bigInt), str);
}

template <std::size_t N>
void testConvertToStringMemberFunc(const std::string& str, std::array<unsigned long long, N> blocks)
{
    BigUnsigned bigInt{ blocks.data(), blocks.size() };
    EXPECT_EQ(bigInt.toString(), str);
}

inline void testOperatorAddition(const std::string& left, const std::string& right, const std::string& answer)
{
    BigUnsigned bigInt{};
    bigInt.add(BigUnsigned{ left }, BigUnsigned{ right });
    EXPECT_EQ(bigInt.toString(), answer);
}

inline void testOperatorSubtraction(const std::string& left, const std::string& right, const std::string& answer)
{
    BigUnsigned bigInt{};
    bigInt.subtract(BigUnsigned{ left }, BigUnsigned{ right });
    EXPECT_EQ(bigInt.toString(), answer);
}
} // namespace bigunsigned

TEST(BigUnsignedConstruct, Default)
{
    /* Default constructors */
    EXPECT_EQ(BigUnsigned{}, 0);
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
        EXPECT_EQ(bigInt, 0);
        EXPECT_EQ(bigInt.bitLength(), 0);
        EXPECT_EQ(bigInt.getLength(), 0);
        EXPECT_EQ(bigInt.toString(), "0");
    }
    {
        std::array<BigUnsigned::Blk, 3> blocks{ 0, 0, 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        EXPECT_EQ(bigInt, 0);
        EXPECT_EQ(bigInt.bitLength(), 0);
        EXPECT_EQ(bigInt.getLength(), 0);
        EXPECT_EQ(bigInt.toString(), "0");
    }
    {
        std::array<BigUnsigned::Blk, 10> blocks{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        EXPECT_EQ(bigInt, 0);
        EXPECT_EQ(bigInt.bitLength(), 0);
        EXPECT_EQ(bigInt.getLength(), 0);
        EXPECT_EQ(bigInt.toString(), "0");
    }

    {
        std::array<BigUnsigned::Blk, 2> blocks{ 7777777, 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        EXPECT_EQ(bigInt, 7777777);
        EXPECT_EQ(bigInt.bitLength(), 23);
        EXPECT_EQ(bigInt.getLength(), 1);
        EXPECT_EQ(bigInt.getBlock(0), 7777777);
        EXPECT_EQ(bigInt.toString(), "7777777");
    }
    {
        std::array<BigUnsigned::Blk, 3> blocks{ 7777777, 0, 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        EXPECT_EQ(bigInt, 7777777);
        EXPECT_EQ(bigInt.bitLength(), 23);
        EXPECT_EQ(bigInt.getLength(), 1);
        EXPECT_EQ(bigInt.getBlock(0), 7777777);
        EXPECT_EQ(bigInt.toString(), "7777777");
    }
    {
        std::array<BigUnsigned::Blk, 10> blocks{ 7777777, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        EXPECT_EQ(bigInt, 7777777);
        EXPECT_EQ(bigInt.bitLength(), 23);
        EXPECT_EQ(bigInt.getLength(), 1);
        EXPECT_EQ(bigInt.getBlock(0), 7777777);
        EXPECT_EQ(bigInt.toString(), "7777777");
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
        EXPECT_EQ(bigInt.bitLength(), 67);
        EXPECT_EQ(bigInt.getLength(), 2);
        EXPECT_EQ(bigInt.getBlock(0), 3);
        EXPECT_EQ(bigInt.getBlock(1), 4);
        EXPECT_EQ(bigInt.toString(), "73786976294838206467");
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
        EXPECT_EQ(bigInt.bitLength(), 266);
        EXPECT_EQ(bigInt.getLength(), 5);
        EXPECT_EQ(bigInt.getBlock(0), 222354);
        EXPECT_EQ(bigInt.getBlock(1), 3);
        EXPECT_EQ(bigInt.getBlock(2), 4);
        EXPECT_EQ(bigInt.getBlock(3), 11111111);
        EXPECT_EQ(bigInt.getBlock(4), 789);
        EXPECT_EQ(bigInt.toString(),
                  "91359958408242547934771647345892647840473432917015689066444647515128388106019986");
    }
    {
        std::array<BigUnsigned::Blk, 7> blocks{ 7777777898791111, 111111344444, 4000000, 777, 123, 88888000006, 6023 };
        BigUnsigned bigInt{ blocks.data(), blocks.size() };
        EXPECT_EQ(bigInt.bitLength(), 397);
        EXPECT_EQ(bigInt.getLength(), 7);
        EXPECT_EQ(bigInt.getBlock(0), 7777777898791111);
        EXPECT_EQ(bigInt.getBlock(1), 111111344444);
        EXPECT_EQ(bigInt.getBlock(2), 4000000);
        EXPECT_EQ(bigInt.getBlock(3), 777);
        EXPECT_EQ(bigInt.getBlock(4), 123);
        EXPECT_EQ(bigInt.getBlock(5), 88888000006);
        EXPECT_EQ(bigInt.getBlock(6), 6023);
        // clang-format off
        EXPECT_EQ(bigInt.toString(),
                  "237318283321073811911218412300784440097921881331589397685365402091525638845764065313359710598415169688867963437285417159");
        // clang-format on
    }
}

TEST(BigUnsignedConstruct, FromString)
{
    // clang-format off

    EXPECT_EQ(BigUnsigned{ "1" }.toString(), "1");
    EXPECT_EQ(BigUnsigned{ "10" }.toString(), "10");
    EXPECT_EQ(BigUnsigned{ "100" }.toString(), "100");
    EXPECT_EQ(BigUnsigned{ "1000" }.toString(), "1000");
    EXPECT_EQ(BigUnsigned{ "10000" }.toString(), "10000");
    EXPECT_EQ(BigUnsigned{ "100000" }.toString(), "100000");
    EXPECT_EQ(BigUnsigned{ "1000000" }.toString(), "1000000");
    EXPECT_EQ(BigUnsigned{ "10000000" }.toString(), "10000000");
    EXPECT_EQ(BigUnsigned{ "100000000" }.toString(), "100000000");
    EXPECT_EQ(BigUnsigned{ "1000000000" }.toString(), "1000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000" }.toString(), "10000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000" }.toString(), "100000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000" }.toString(), "1000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000" }.toString(), "10000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000" }.toString(), "100000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000" }.toString(), "1000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000" }.toString(), "10000000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000000" }.toString(), "100000000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000000" }.toString(), "1000000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000000" }.toString(), "10000000000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000000000" }.toString(), "100000000000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000000000" }.toString(), "1000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000000000" }.toString(), "10000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000000000000" }.toString(), "100000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000000000000" }.toString(), "1000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000000000000" }.toString(), "10000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000000000000000" }.toString(), "100000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000000000000000" }.toString(), "1000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000000000000000" }.toString(), "10000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000000000000000000" }.toString(), "100000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000000000000000000" }.toString(), "1000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000000000000000000" }.toString(), "10000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000000000000000000000" }.toString(), "100000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000000000000000000000000" }.toString(), "100000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000000000000000000000000000" }.toString(), "100000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000000000000000000000000000000" }.toString(), "100000000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000000000000000000000000000000000" }.toString(), "100000000000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "100000000000000000000000000000000000000000000000" }.toString(), "100000000000000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "1000000000000000000000000000000000000000000000000" }.toString(), "1000000000000000000000000000000000000000000000000");
    EXPECT_EQ(BigUnsigned{ "10000000000000000000000000000000000000000000000000" }.toString(), "10000000000000000000000000000000000000000000000000");

    // clang-format on

    /* Very big number */
    EXPECT_EQ(BigUnsigned{ "2373182833210738119112184123007844400979218813315893976853654020915256388457640653133597105"
                           "98415169688867963437285417159" }
                  .toString(),
              "23731828332107381191121841230078444009792188133158939768536540209152563884576406531335971059841516968886"
              "7963437285417159");
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
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 0 }), "0");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 1 }), "1");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 10 }), "10");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 100 }), "100");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 1000 }), "1000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 10000 }), "10000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 100000 }), "100000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 1000000 }), "1000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 10000000 }), "10000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 100000000 }), "100000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 1000000000 }), "1000000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 10000000000 }), "10000000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 100000000000 }), "100000000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 1000000000000 }), "1000000000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 10000000000000 }), "10000000000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 100000000000000 }), "100000000000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 1000000000000000 }), "1000000000000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 10000000000000000 }), "10000000000000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 100000000000000000 }), "100000000000000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 1000000000000000000 }), "1000000000000000000");
    EXPECT_EQ(bigUnsignedToString(BigUnsigned{ 10000000000000000000ull }), "10000000000000000000");

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
    
    /* Very big number */
    testConvertToString<7>(
        "2373182833210738119112184123007844400979218813315893976853654020915256388457640653133597105"
        "98415169688867963437285417159",
        { 7777777898791111, 111111344444, 4000000, 777, 123, 88888000006, 6023 });

    EXPECT_EQ(BigUnsigned{ 0 }.toString(), "0");
    EXPECT_EQ(BigUnsigned{ 1 }.toString(), "1");
    EXPECT_EQ(BigUnsigned{ 10 }.toString(), "10");
    EXPECT_EQ(BigUnsigned{ 100 }.toString(), "100");
    EXPECT_EQ(BigUnsigned{ 1000 }.toString(), "1000");
    EXPECT_EQ(BigUnsigned{ 10000 }.toString(), "10000");
    EXPECT_EQ(BigUnsigned{ 100000 }.toString(), "100000");
    EXPECT_EQ(BigUnsigned{ 1000000 }.toString(), "1000000");
    EXPECT_EQ(BigUnsigned{ 10000000 }.toString(), "10000000");
    EXPECT_EQ(BigUnsigned{ 100000000 }.toString(), "100000000");
    EXPECT_EQ(BigUnsigned{ 1000000000 }.toString(), "1000000000");
    EXPECT_EQ(BigUnsigned{ 10000000000 }.toString(), "10000000000");
    EXPECT_EQ(BigUnsigned{ 100000000000 }.toString(), "100000000000");
    EXPECT_EQ(BigUnsigned{ 1000000000000 }.toString(), "1000000000000");
    EXPECT_EQ(BigUnsigned{ 10000000000000 }.toString(), "10000000000000");
    EXPECT_EQ(BigUnsigned{ 100000000000000 }.toString(), "100000000000000");
    EXPECT_EQ(BigUnsigned{ 1000000000000000 }.toString(), "1000000000000000");
    EXPECT_EQ(BigUnsigned{ 10000000000000000 }.toString(), "10000000000000000");
    EXPECT_EQ(BigUnsigned{ 100000000000000000 }.toString(), "100000000000000000");
    EXPECT_EQ(BigUnsigned{ 1000000000000000000 }.toString(), "1000000000000000000");
    EXPECT_EQ(BigUnsigned{ 10000000000000000000ull }.toString(), "10000000000000000000");

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

    /* Very big number */
    testConvertToStringMemberFunc<7>(
        "2373182833210738119112184123007844400979218813315893976853654020915256388457640653133597105"
        "98415169688867963437285417159",
        { 7777777898791111, 111111344444, 4000000, 777, 123, 88888000006, 6023 });

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
    EXPECT_EQ(stringToBigUnsigned("100000000000000000000").toString(), "100000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("1000000000000000000000").toString(), "1000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("10000000000000000000000").toString(), "10000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("100000000000000000000000").toString(), "100000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("1000000000000000000000000").toString(), "1000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("10000000000000000000000000").toString(), "10000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("100000000000000000000000000").toString(), "100000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("1000000000000000000000000000").toString(), "1000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("10000000000000000000000000000").toString(), "10000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("100000000000000000000000000000").toString(), "100000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("1000000000000000000000000000000").toString(), "1000000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("10000000000000000000000000000000").toString(), "10000000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("100000000000000000000000000000000").toString(), "100000000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("1000000000000000000000000000000000").toString(), "1000000000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("10000000000000000000000000000000000").toString(), "10000000000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("100000000000000000000000000000000000").toString(), "100000000000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("1000000000000000000000000000000000000").toString(), "1000000000000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("10000000000000000000000000000000000000").toString(), "10000000000000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("100000000000000000000000000000000000000").toString(), "100000000000000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("1000000000000000000000000000000000000000").toString(), "1000000000000000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("10000000000000000000000000000000000000000").toString(), "10000000000000000000000000000000000000000");
    EXPECT_EQ(stringToBigUnsigned("100000000000000000000000000000000000000000").toString(), "100000000000000000000000000000000000000000");
    // clang-format on

    /* Very big number */

    // clang-format off
    EXPECT_EQ(stringToBigUnsigned("100000000000000000000000000000000000000000"
                                  "781231233333334778977777999636666666666666"
                                  "777891111111111111111111333343458566666666"
                                  "112377777777777777777777777777777777777777").toString(),
              "100000000000000000000000000000000000000000"
              "781231233333334778977777999636666666666666"
              "777891111111111111111111333343458566666666"
              "112377777777777777777777777777777777777777");
    // clang-format on
}

TEST(BigUnsignedOperators, Assignment)
{
    EXPECT_EQ(BigUnsigned{} = BigUnsigned{ 0 }, BigUnsigned{ 0 });
    EXPECT_EQ(BigUnsigned{} = BigUnsigned{ 7777777777777ull }, BigUnsigned{ 7777777777777ull });
    EXPECT_EQ(BigUnsigned{} = BigUnsigned{ "100000000000000000000000000000000000000000" },
              BigUnsigned{ "100000000000000000000000000000000000000000" });
    EXPECT_EQ(BigUnsigned{} = BigUnsigned{ "100000000000000000000000000000000000000000"
                                           "781231233333334778977777999636666666666666"
                                           "777891111111111111111111333343458566666666"
                                           "112377777777777777777777777777777777777777" },
              BigUnsigned{ "100000000000000000000000000000000000000000"
                           "781231233333334778977777999636666666666666"
                           "777891111111111111111111333343458566666666"
                           "112377777777777777777777777777777777777777" });
}

TEST(BigUnsignedOperators, Addition)
{
    using namespace bigunsigned;

    testOperatorAddition("1", "1", "2");
    testOperatorAddition("1", "13", "14");
    testOperatorAddition("1", "169", "170");
    testOperatorAddition("1", "2197", "2198");
    testOperatorAddition("1", "28561", "28562");
    testOperatorAddition("1", "371293", "371294");
    testOperatorAddition("1", "4826809", "4826810");
    testOperatorAddition("1", "62748517", "62748518");
    testOperatorAddition("1", "815730721", "815730722");
    testOperatorAddition("1", "10604499373", "10604499374");
    testOperatorAddition("13", "137858491849", "137858491862");
    testOperatorAddition("13", "1792160394037", "1792160394050");
    testOperatorAddition("13", "23298085122481", "23298085122494");
    testOperatorAddition("13", "302875106592253", "302875106592266");
    testOperatorAddition("13", "3937376385699289", "3937376385699302");
    testOperatorAddition("13", "51185893014090757", "51185893014090770");
    testOperatorAddition("13", "665416609183179841", "665416609183179854");
    testOperatorAddition("13", "8650415919381337933", "8650415919381337946");
    testOperatorAddition("13", "112455406951957393129", "112455406951957393142");
    testOperatorAddition("13", "1461920290375446110677", "1461920290375446110690");
    testOperatorAddition("169", "19004963774880799438801", "19004963774880799438970");
    testOperatorAddition("169", "247064529073450392704413", "247064529073450392704582");
    testOperatorAddition("169", "3211838877954855105157369", "3211838877954855105157538");
    testOperatorAddition("169", "41753905413413116367045797", "41753905413413116367045966");
    testOperatorAddition("169", "542800770374370512771595361", "542800770374370512771595530");
    testOperatorAddition("169", "7056410014866816666030739693", "7056410014866816666030739862");
    testOperatorAddition("169", "91733330193268616658399616009", "91733330193268616658399616178");
    testOperatorAddition("169", "1192533292512492016559195008117", "1192533292512492016559195008286");
    testOperatorAddition("169", "15502932802662396215269535105521", "15502932802662396215269535105690");
    testOperatorAddition("169", "201538126434611150798503956371773", "201538126434611150798503956371942");
    testOperatorAddition("2197", "2619995643649944960380551432833049", "2619995643649944960380551432835246");
    testOperatorAddition("2197", "34059943367449284484947168626829637", "34059943367449284484947168626831834");
    testOperatorAddition("2197", "442779263776840698304313192148785281", "442779263776840698304313192148787478");
    testOperatorAddition("2197", "5756130429098929077956071497934208653", "5756130429098929077956071497934210850");
    testOperatorAddition("2197", "74829695578286078013428929473144712489", "74829695578286078013428929473144714686");
    testOperatorAddition("2197", "972786042517719014174576083150881262357", "972786042517719014174576083150881264554");
    testOperatorAddition(
        "2197", "12646218552730347184269489080961456410641", "12646218552730347184269489080961456412838");
    testOperatorAddition(
        "2197", "164400841185494513395503358052498933338333", "164400841185494513395503358052498933340530");
    testOperatorAddition(
        "2197", "2137210935411428674141543654682486133398329", "2137210935411428674141543654682486133400526");
    testOperatorAddition(
        "2197", "27783742160348572763840067510872319734178277", "27783742160348572763840067510872319734180474");
    testOperatorAddition(
        "28561", "361188648084531445929920877641340156544317601", "361188648084531445929920877641340156544346162");
    testOperatorAddition(
        "28561", "4695452425098908797088971409337422035076128813", "4695452425098908797088971409337422035076157374");
    testOperatorAddition(
        "28561", "61040881526285814362156628321386486455989674569", "61040881526285814362156628321386486455989703130");
    testOperatorAddition("28561",
                         "793531459841715586708036168178024323927865769397",
                         "793531459841715586708036168178024323927865797958");
    testOperatorAddition("28561",
                         "10315908977942302627204470186314316211062255002161",
                         "10315908977942302627204470186314316211062255030722");
    testOperatorAddition("28561",
                         "134106816713249934153658112422086110743809315028093",
                         "134106816713249934153658112422086110743809315056654");
    testOperatorAddition("28561",
                         "1743388617272249143997555461487119439669521095365209",
                         "1743388617272249143997555461487119439669521095393770");
    testOperatorAddition("28561",
                         "22664052024539238871968220999332552715703774239747717",
                         "22664052024539238871968220999332552715703774239776278");
    testOperatorAddition("28561",
                         "294632676319010105335586872991323185304149065116720321",
                         "294632676319010105335586872991323185304149065116748882");
    testOperatorAddition("28561",
                         "3830224792147131369362629348887201408953937846517364173",
                         "3830224792147131369362629348887201408953937846517392734");
    testOperatorAddition("371293",
                         "49792922297912707801714181535533618316401192004725734249",
                         "49792922297912707801714181535533618316401192004726105542");
    testOperatorAddition("371293",
                         "647307989872865201422284359961937038113215496061434545237",
                         "647307989872865201422284359961937038113215496061434916530");
    testOperatorAddition("371293",
                         "8415003868347247618489696679505181495471801448798649088081",
                         "8415003868347247618489696679505181495471801448798649459374");
    testOperatorAddition("371293",
                         "109395050288514219040366056833567359441133418834382438145053",
                         "109395050288514219040366056833567359441133418834382438516346");
    testOperatorAddition("371293",
                         "1422135653750684847524758738836375672734734444846971695885689",
                         "1422135653750684847524758738836375672734734444846971696256982");
    testOperatorAddition("371293",
                         "18487763498758903017821863604872883745551547783010632046513957",
                         "18487763498758903017821863604872883745551547783010632046885250");
    testOperatorAddition("371293",
                         "240340925483865739231684226863347488692170121179138216604681441",
                         "240340925483865739231684226863347488692170121179138216605052734");
    testOperatorAddition("371293",
                         "3124432031290254610011894949223517352998211575328796815860858733",
                         "3124432031290254610011894949223517352998211575328796815861230026");
    testOperatorAddition("371293",
                         "40617616406773309930154634339905725588976750479274358606191163529",
                         "40617616406773309930154634339905725588976750479274358606191534822");
    testOperatorAddition("371293",
                         "528029013288053029092010246418774432656697756230566661880485125877",
                         "528029013288053029092010246418774432656697756230566661880485497170");
    testOperatorAddition("4826809",
                         "6864377172744689378196133203444067624537070830997366604446306636401",
                         "6864377172744689378196133203444067624537070830997366604446311463210");
    testOperatorAddition("4826809",
                         "89236903245680961916549731644772879118981920802965765857801986273213",
                         "89236903245680961916549731644772879118981920802965765857801991100022");
    testOperatorAddition("4826809",
                         "1160079742193852504915146511382047428546764970438554956151425821551769",
                         "1160079742193852504915146511382047428546764970438554956151425826378578");
    testOperatorAddition("4826809",
                         "15081036648520082563896904647966616571107944615701214429968535680172997",
                         "15081036648520082563896904647966616571107944615701214429968535684999806");
    testOperatorAddition("4826809",
                         "196053476430761073330659760423566015424403280004115787589590963842248961",
                         "196053476430761073330659760423566015424403280004115787589590963847075770");
    testOperatorAddition("4826809",
                         "2548695193599893953298576885506358200517242640053505238664682529949236493",
                         "2548695193599893953298576885506358200517242640053505238664682529954063302");
    testOperatorAddition("4826809",
                         "33133037516798621392881499511582656606724154320695568102640872889340074409",
                         "33133037516798621392881499511582656606724154320695568102640872889344901218");
    testOperatorAddition("4826809",
                         "430729487718382078107459493650574535887414006169042385334331347561420967317",
                         "430729487718382078107459493650574535887414006169042385334331347561425794126");
    testOperatorAddition("4826809",
                         "5599483340338967015396973417457468966536382080197551009346307518298472575121",
                         "5599483340338967015396973417457468966536382080197551009346307518298477401930");
    testOperatorAddition("4826809",
                         "72793283424406571200160654426947096564972967042568163121501997737880143476573",
                         "72793283424406571200160654426947096564972967042568163121501997737880148303382");
    testOperatorAddition("62748517",
                         "946312684517285425602088507550312255344648571553386120579525970592441865195449",
                         "946312684517285425602088507550312255344648571553386120579525970592441927943966");
    testOperatorAddition("62748517",
                         "12302064898724710532827150598154059319480431430194019567533837617701744247540837",
                         "12302064898724710532827150598154059319480431430194019567533837617701744310289354");
    testOperatorAddition("62748517",
                         "159926843683421236926752957776002771153245608592522254377939889030122675218030881",
                         "159926843683421236926752957776002771153245608592522254377939889030122675280779398");
    testOperatorAddition("62748517",
                         "2079048967884476080047788451088036024992192911702789306913218557391594777834401453",
                         "2079048967884476080047788451088036024992192911702789306913218557391594777897149970");
    testOperatorAddition("62748517",
                         "27027636582498189040621249864144468324898507852136260989871841246090732111847218889",
                         "27027636582498189040621249864144468324898507852136260989871841246090732111909967406");
    testOperatorAddition("62748517",
                         "351359275572476457528076248233878088223680602077771392868333936199179517454013845557",
                         "351359275572476457528076248233878088223680602077771392868333936199179517454076594074");
    testOperatorAddition("62748517",
                         "4567670582442193947864991227040415146907847827011028107288341170589333726902179992241",
                         "4567670582442193947864991227040415146907847827011028107288341170589333726902242740758");
    testOperatorAddition("62748517",
                         "59379717571748521322244885951525396909802021751143365394748435217661338449728339899133",
                         "59379717571748521322244885951525396909802021751143365394748435217661338449728402647650");
    testOperatorAddition("62748517",
                         "771936328432730777189183517369830159827426282764863750131729657829597399846468418688729",
                         "771936328432730777189183517369830159827426282764863750131729657829597399846468481437246");
    testOperatorAddition("62748517",
                         "10035172269625500103459385725807792077756541675943228751712485551784766198004089442953477",
                         "10035172269625500103459385725807792077756541675943228751712485551784766198004089505701994");
    testOperatorAddition("815730721",
                         "130457239505131501344972014435501297010835041787261973772262312173201960574053162758395201",
                         "130457239505131501344972014435501297010835041787261973772262312173201960574053163574125922");
    testOperatorAddition("815730721",
                         "1695944113566709517484636187661516861140855543234405659039410058251625487462691115859137613",
                         "1695944113566709517484636187661516861140855543234405659039410058251625487462691116674868334");
    testOperatorAddition(
        "815730721",
        "22047273476367223727300270439599719194831122062047273567512330757271131337014984506168788969",
        "22047273476367223727300270439599719194831122062047273567512330757271131337014984506984519690");
    testOperatorAddition(
        "815730721",
        "286614555192773908454903515714796349532804586806614556377660299844524707381194798580194256597",
        "286614555192773908454903515714796349532804586806614556377660299844524707381194798581009987318");
    testOperatorAddition(
        "815730721",
        "3725989217506060809913745704292352543926459628485989232909583897978821195955532381542525335761",
        "3725989217506060809913745704292352543926459628485989232909583897978821195955532381543341066482");
    testOperatorAddition(
        "815730721",
        "48437859827578790528878694155800583071043975170317860027824590673724675547421920960052829364893",
        "48437859827578790528878694155800583071043975170317860027824590673724675547421920960053645095614");
    testOperatorAddition(
        "815730721",
        "629692177758524276875423024025407579923571677214132180361719678758420782116484972480686781743609",
        "629692177758524276875423024025407579923571677214132180361719678758420782116484972480687597474330");
    testOperatorAddition(
        "815730721",
        "8185998310860815599380499312330298539006431803783718344702355823859470167514304642248928162666917",
        "8185998310860815599380499312330298539006431803783718344702355823859470167514304642248928978397638");
    testOperatorAddition(
        "815730721",
        "106417978041190602791946491060293881007083613449188338481130625710173112177685960349236066114669921",
        "106417978041190602791946491060293881007083613449188338481130625710173112177685960349236066930400642");
    testOperatorAddition(
        "815730721",
        "1383433714535477836295304383783820453092086974839448400254698134232250458309917484540068859490708973",
        "1383433714535477836295304383783820453092086974839448400254698134232250458309917484540068860306439694");
    testOperatorAddition(
        "10604499373",
        "17984638288961211871838956989189665890197130672912829203311075745019255958028927299020895173379216649",
        "17984638288961211871838956989189665890197130672912829203311075745019255958028927299020895183983716022");
    testOperatorAddition(
        "10604499373",
        "233800297756495754333906440859465656572562698747866779643043984685250327454376054887271637253929816437",
        "233800297756495754333906440859465656572562698747866779643043984685250327454376054887271637264534315810");
    testOperatorAddition(
        "10604499373",
        "3039403870834444806340783731173053535443315083722268135359571800908254256906888713534531284301087613681",
        "3039403870834444806340783731173053535443315083722268135359571800908254256906888713534531284311692113054");
    testOperatorAddition(
        "10604499373",
        "39512250320847782482430188505249695960763096088389485759674433411807305339789553275948906695914138977853",
        "39512250320847782482430188505249695960763096088389485759674433411807305339789553275948906695924743477226");
    testOperatorAddition(
        "10604499373",
        "513659254171021172271592450568246047489920249149063314875767634353494969417264192587335787046883806712089",
        "513659254171021172271592450568246047489920249149063314875767634353494969417264192587335787046894411211462");
    testOperatorAddition(
        "10604499373",
        "6677570304223275239530701857387198617368963238937823093384979246595434602424434503635365231609489487257157",
        "6677570304223275239530701857387198617368963238937823093384979246595434602424434503635365231609500091756530");
    testOperatorAddition(
        "10604499373",
        "86808413954902578113899124146033582025796522106191700214004730205740649831517648547259748010923363334343041",
        "86808413954902578113899124146033582025796522106191700214004730205740649831517648547259748010923373938842414");
    testOperatorAddition(
        "10604499373",
        "1128509381413733515480688613898436566335354787380492102782061492674628447809729431114376724142003723346459533",
        "112850938141373351548068861389843656633535478738049210278206149267462844780972943111437672414200373395095890"
        "6");
    testOperatorAddition("10604499373",
                         "146706219583785357012489519806796753623596122359463973361667994047701698215264826044868974138"
                         "46048403503973929",
                         "146706219583785357012489519806796753623596122359463973361667994047701698215264826044868974138"
                         "46048414108473302");
    testOperatorAddition("10604499373",
                         "190718085458920964116236375748835779710674959067303165370168392262012207679844273858329666379"
                         "998629245551661077",
                         "190718085458920964116236375748835779710674959067303165370168392262012207679844273858329666379"
                         "998629256156160450");
}

TEST(BigUnsignedOperators, Subtraction)
{
    using namespace bigunsigned;

    testOperatorSubtraction("1", "1", "0");
    testOperatorSubtraction("15", "13", "2");
    testOperatorSubtraction("225", "169", "56");
    testOperatorSubtraction("3375", "2197", "1178");
    testOperatorSubtraction("50625", "28561", "22064");
    testOperatorSubtraction("759375", "371293", "388082");
    testOperatorSubtraction("11390625", "4826809", "6563816");
    testOperatorSubtraction("170859375", "62748517", "108110858");
    testOperatorSubtraction("2562890625", "815730721", "1747159904");
    testOperatorSubtraction("38443359375", "10604499373", "27838860002");
    testOperatorSubtraction("576650390625", "137858491849", "438791898776");
    testOperatorSubtraction("8649755859375", "1792160394037", "6857595465338");
    testOperatorSubtraction("129746337890625", "23298085122481", "106448252768144");
    testOperatorSubtraction("1946195068359375", "302875106592253", "1643319961767122");
    testOperatorSubtraction("29192926025390625", "3937376385699289", "25255549639691336");
    testOperatorSubtraction("437893890380859375", "51185893014090757", "386707997366768618");
    testOperatorSubtraction("6568408355712890625", "665416609183179841", "5902991746529710784");
    testOperatorSubtraction("98526125335693359375", "8650415919381337933", "89875709416312021442");
    testOperatorSubtraction("1477891880035400390625", "112455406951957393129", "1365436473083442997496");
    testOperatorSubtraction("22168378200531005859375", "1461920290375446110677", "20706457910155559748698");
    testOperatorSubtraction("332525673007965087890625", "19004963774880799438801", "313520709233084288451824");
    testOperatorSubtraction("4987885095119476318359375", "247064529073450392704413", "4740820566046025925654962");
    testOperatorSubtraction("74818276426792144775390625", "3211838877954855105157369", "71606437548837289670233256");
    testOperatorSubtraction(
        "1122274146401882171630859375", "41753905413413116367045797", "1080520240988469055263813578");
    testOperatorSubtraction(
        "16834112196028232574462890625", "542800770374370512771595361", "16291311425653862061691295264");
    testOperatorSubtraction(
        "252511682940423488616943359375", "7056410014866816666030739693", "245455272925556671950912619682");
    testOperatorSubtraction(
        "3787675244106352329254150390625", "91733330193268616658399616009", "3695941913913083712595750774616");
    testOperatorSubtraction(
        "56815128661595284938812255859375", "1192533292512492016559195008117", "55622595369082792922253060851258");
    testOperatorSubtraction(
        "852226929923929274082183837890625", "15502932802662396215269535105521", "836723997121266877866914302785104");
    testOperatorSubtraction("12783403948858939111232757568359375",
                            "201538126434611150798503956371773",
                            "12581865822424327960434253611987602");
    testOperatorSubtraction("191751059232884086668491363525390625",
                            "2619995643649944960380551432833049",
                            "189131063589234141708110812092557576");
    testOperatorSubtraction("2876265888493261300027370452880859375",
                            "34059943367449284484947168626829637",
                            "2842205945125812015542423284254029738");
    testOperatorSubtraction("43143988327398919500410556793212890625",
                            "442779263776840698304313192148785281",
                            "42701209063622078802106243601064105344");
    testOperatorSubtraction("647159824910983792506158351898193359375",
                            "5756130429098929077956071497934208653",
                            "641403694481884863428202280400259150722");
    testOperatorSubtraction("9707397373664756887592375278472900390625",
                            "74829695578286078013428929473144712489",
                            "9632567678086470809578946348999755678136");
    testOperatorSubtraction("145610960604971353313885629177093505859375",
                            "972786042517719014174576083150881262357",
                            "144638174562453634299711053093942624597018");
    testOperatorSubtraction("2184164409074570299708284437656402587890625",
                            "12646218552730347184269489080961456410641",
                            "2171518190521839952524014948575441131479984");
    testOperatorSubtraction("32762466136118554495624266564846038818359375",
                            "164400841185494513395503358052498933338333",
                            "32598065294933059982228763206793539885021042");
    testOperatorSubtraction("491436992041778317434363998472690582275390625",
                            "2137210935411428674141543654682486133398329",
                            "489299781106366888760222454818008096141992296");
    testOperatorSubtraction("7371554880626674761515459977090358734130859375",
                            "27783742160348572763840067510872319734178277",
                            "7343771138466326188751619909579486414396681098");
    testOperatorSubtraction("110573323209400121422731899656355381011962890625",
                            "361188648084531445929920877641340156544317601",
                            "110212134561315589976801978778714040855418573024");
    testOperatorSubtraction("1658599848141001821340978494845330715179443359375",
                            "4695452425098908797088971409337422035076128813",
                            "1653904395715902912543889523435993293144367230562");
    testOperatorSubtraction("24878997722115027320114677422679960727691650390625",
                            "61040881526285814362156628321386486455989674569",
                            "24817956840588741505752520794358574241235660716056");
    testOperatorSubtraction("373184965831725409801720161340199410915374755859375",
                            "793531459841715586708036168178024323927865769397",
                            "372391434371883694215012125172021386591446890089978");
    testOperatorSubtraction("5597774487475881147025802420102991163730621337890625",
                            "10315908977942302627204470186314316211062255002161",
                            "5587458578497938844398597949916676847519559082888464");
    testOperatorSubtraction("83966617312138217205387036301544867455959320068359375",
                            "134106816713249934153658112422086110743809315028093",
                            "83832510495424967271233378189122781345215510753331282");
    testOperatorSubtraction("1259499259682073258080805544523173011839389801025390625",
                            "1743388617272249143997555461487119439669521095365209",
                            "1257755871064801008936807989061685892399720279930025416");
    testOperatorSubtraction("18892488895231098871212083167847595177590847015380859375",
                            "22664052024539238871968220999332552715703774239747717",
                            "18869824843206559632340114946848262624875143241141111658");
    testOperatorSubtraction("283387333428466483068181247517713927663862705230712890625",
                            "294632676319010105335586872991323185304149065116720321",
                            "283092700752147472962845660644722604478558556165596170304");
    testOperatorSubtraction("4250810001426997246022718712765708914957940578460693359375",
                            "3830224792147131369362629348887201408953937846517364173",
                            "4246979776634850114653356083416821713548986640614175995202");
    testOperatorSubtraction("63762150021404958690340780691485633724369108676910400390625",
                            "49792922297912707801714181535533618316401192004725734249",
                            "63712357099107045982539066509950100106052707484905674656376");
    testOperatorSubtraction("956432250321074380355111710372284505865536630153656005859375",
                            "647307989872865201422284359961937038113215496061434545237",
                            "955784942331201515153689426012322568827423414657594571314138");
    testOperatorSubtraction("14346483754816115705326675655584267587983049452304840087890625",
                            "8415003868347247618489696679505181495471801448798649088081",
                            "14338068750947768457708185958904762406487577650856041438802544");
    testOperatorSubtraction("215197256322241735579900134833764013819745741784572601318359375",
                            "109395050288514219040366056833567359441133418834382438145053",
                            "215087861271953221360859768776930446460304608365738218880214322");
    testOperatorSubtraction("3227958844833626033698502022506460207296186126768589019775390625",
                            "1422135653750684847524758738836375672734734444846971695885689",
                            "3226536709179875348850977263767623831623451392323742048079504936");
    testOperatorSubtraction("48419382672504390505477530337596903109442791901528835296630859375",
                            "18487763498758903017821863604872883745551547783010632046513957",
                            "48400894909005631602459708473992030225697240353745824664584345418");
    testOperatorSubtraction("726290740087565857582162955063953546641641878522932529449462890625",
                            "240340925483865739231684226863347488692170121179138216604681441",
                            "726050399162081991842931270837090199152949708401753391232858209184");
    testOperatorSubtraction("10894361101313487863732444325959303199624628177843987941741943359375",
                            "3124432031290254610011894949223517352998211575328796815860858733",
                            "10891236669282197609122432431010079682271629966268659144926082500642");
    testOperatorSubtraction("163415416519702317955986664889389547994369422667659819126129150390625",
                            "40617616406773309930154634339905725588976750479274358606191163529",
                            "163374798903295544646056510255049642268780445917180544767522959227096");
    testOperatorSubtraction("2451231247795534769339799973340843219915541340014897286891937255859375",
                            "528029013288053029092010246418774432656697756230566661880485125877",
                            "2450703218782246716310707963094424445482884642258666720230056770733498");
    testOperatorSubtraction("36768468716933021540096999600112648298733120100223459303379058837890625",
                            "6864377172744689378196133203444067624537070830997366604446306636401",
                            "36761604339760276850718803466909204231108583029392461936774612531254224");
    testOperatorSubtraction("551527030753995323101454994001689724480996801503351889550685882568359375",
                            "89236903245680961916549731644772879118981920802965765857801986273213",
                            "551437793850749642139538444270044951601877819582548923784828080582086162");
    testOperatorSubtraction("8272905461309929846521824910025345867214952022550278343260288238525390625",
                            "1160079742193852504915146511382047428546764970438554956151425821551769",
                            "8271745381567735994016909763513963819786405257579839788304136812703838856");
    testOperatorSubtraction("124093581919648947697827373650380188008224280338254175148904323577880859375",
                            "15081036648520082563896904647966616571107944615701214429968535680172997",
                            "124078500883000427615263476745732221391653172393638473934474355042200686378");
    testOperatorSubtraction("1861403728794734215467410604755702820123364205073812627233564853668212890625",
                            "196053476430761073330659760423566015424403280004115787589590963842248961",
                            "1861207675318303454394079944995279254107939801793808511445975262704370641664");
    testOperatorSubtraction("27921055931921013232011159071335542301850463076107189408503472805023193359375",
                            "2548695193599893953298576885506358200517242640053505238664682529949236493",
                            "27918507236727413338057860494450035943649945833467135903264808122493244122882");
    testOperatorSubtraction("418815838978815198480167386070033134527756946141607841127552092075347900390625",
                            "33133037516798621392881499511582656606724154320695568102640872889340074409",
                            "418782705941298399858774504570521551871150221987287145559449451202458560316216");
    testOperatorSubtraction("6282237584682227977202510791050497017916354192124117616913281381130218505859375",
                            "430729487718382078107459493650574535887414006169042385334331347561420967317",
                            "6281806855194509595124403331556846443380466778117948574527947049782657084892058");
    testOperatorSubtraction("94233563770233419658037661865757455268745312881861764253699220716953277587890625",
                            "5599483340338967015396973417457468966536382080197551009346307518298472575121",
                            "94227964286893080691022264892339997799778776499781566702689874409434979115315504");
    testOperatorSubtraction("1413503456553501294870564927986361829031179693227926463805488310754299163818359375",
                            "72793283424406571200160654426947096564972967042568163121501997737880143476573",
                            "1413430663270076888299364767331934881934614720260883895642366808756561283674882802");
    testOperatorSubtraction("21202551848302519423058473919795427435467695398418896957082324661314487457275390625",
                            "946312684517285425602088507550312255344648571553386120579525970592441865195449",
                            "21201605535618002137632871831287877123212350749847343570961745135343895015410195176");
    testOperatorSubtraction("318038277724537791345877108796931411532015430976283454356234869919717311859130859375",
                            "12302064898724710532827150598154059319480431430194019567533837617701744247540837",
                            "318025975659639066635344281646333257472695950544853260336667336082099610114883318538");
    testOperatorSubtraction("4770574165868066870188156631953971172980231464644251815343523048795759677886962890625",
                            "159926843683421236926752957776002771153245608592522254377939889030122675218030881",
                            "4770414239024383448951229878996195170209078219035659293089145108906729555211744859744");
    testOperatorSubtraction("71558612488021003052822349479309567594703471969663777230152845731936395168304443359375",
                            "2079048967884476080047788451088036024992192911702789306913218557391594777834401453",
                            "71556533439053118576742301690858479558678479776752074440845932513379003573526608957922");
    testOperatorSubtraction("1073379187320315045792335242189643513920552079544956658452292685979045927524566650390625",
                            "27027636582498189040621249864144468324898507852136260989871841246090732111847218889",
                            "1073352159683732547603294620939779369452227181037104522191302814137799836792454803171736");
    testOperatorSubtraction(
        "16100687809804725686885028632844652708808281193174349876784390289685688912868499755859375",
        "351359275572476457528076248233878088223680602077771392868333936199179517454013845557",
        "16100336450529153210427500556596418830720057512572272105391521955749489733351045742013818");
    testOperatorSubtraction(
        "241510317147070885303275429492669790632124217897615248151765854345285333693027496337890625",
        "4567670582442193947864991227040415146907847827011028107288341170589333726902179992241",
        "241505749476488443109327564501442750216977310049788237123658566004114744359300594157898384");
    testOperatorSubtraction(
        "3622654757206063279549131442390046859481863268464228722276487815179280005395412445068359375",
        "59379717571748521322244885951525396909802021751143365394748435217661338449728339899133",
        "3622595377488491531027809197504095334084953466442477578911093066744062344056962716728460242");
    testOperatorSubtraction(
        "54339821358090949193236971635850702892227949026963430834147317227689200080931186676025390625",
        "771936328432730777189183517369830159827426282764863750131729657829597399846468418688729",
        "54339049421762516462459782452333333062068121600680665970397185498031370483531340207606701896");
    testOperatorSubtraction(
        "815097320371364237898554574537760543383419235404451462512209758415338001213967800140380859375",
        "10035172269625500103459385725807792077756541675943228751712485551784766198004089442953477",
        "815087285199094612398451115152034735591341478862775519283458045929786216447769796050937905898");
    testOperatorSubtraction(
        "12226459805570463568478318618066408150751288531066771937683146376230070018209517002105712890625",
        "130457239505131501344972014435501297010835041787261973772262312173201960574053162758395201",
        "12226329348330958436976973646051972649454277696024984675709374113917896816248942948942954495424");
    testOperatorSubtraction(
        "183396897083556953527174779270996122261269327966001579065247195643451050273142755031585693359375",
        "1695944113566709517484636187661516861140855543234405659039410058251625487462691115859137613",
        "183395201139443386817657294634808460744408187110458344659588156233392798647655292340469834221762");
    testOperatorSubtraction(
        "2750953456253354302907621689064941833919039919490023685978707934651765754097141325473785400390625",
        "22047273476367223727300270439599719194831122062047273567512330757271131337014984506168788969",
        "2750931408979877935683894388794502234199845088367961638705140422321008482965804310489279231601656");
    testOperatorSubtraction(
        "41264301843800314543614325335974127508785598792350355289680619019776486311457119882106781005859375",
        "286614555192773908454903515714796349532804586806614556377660299844524707381194798580194256597",
        "41264015229245121769705870432458412712436065987763548675124241359476641786749738687308200811602778");
    testOperatorSubtraction(
        "618964527657004718154214880039611912631783981885255329345209285296647294671856798231601715087890625",
        "3725989217506060809913745704292352543926459628485989232909583897978821195955532381542525335761",
        "618960801667787212093404966293907620279240055425626843355976375712749315850660842699220172562554864");
    testOperatorSubtraction(
        "9284467914855070772313223200594178689476759728278829940178139279449709420077851973474025726318359375",
        "48437859827578790528878694155800583071043975170317860027824590673724675547421920960052829364893",
        "9284419476995243193522694321900022888893688684303659622318111454859035695402304551553065673488994482");
    testOperatorSubtraction(
        "139267018722826061584698348008912680342151395924182449102672089191745641301167779602110385894775390625",
        "629692177758524276875423024025407579923571677214132180361719678758420782116484972480686781743609",
        "139266389030648303060421472585888654934571472352505234970491727472066882880385663117137905207993647016");
    testOperatorSubtraction(
        "2089005280842390923770475220133690205132270938862736736540081337876184619517516694031655788421630859375",
        "8185998310860815599380499312330298539006431803783718344702355823859470167514304642248928162666917",
        "2088997094844080062954875839634377874833731932430932952821736635520360760047349179727013539493468192458");
    testOperatorSubtraction(
        "31335079212635863856557128302005353076984064082941051048101220068142769292762750410474836826324462890625",
        "106417978041190602791946491060293881007083613449188338481130625710173112177685960349236066114669921",
        "31334972794657822665954336355514292783103056999327601859762738937517059119650572724514487590258348220704");
    testOperatorSubtraction(
        "470026188189537957848356924530080296154760961244115765721518301022141539391441256157122552394866943359375",
        "1383433714535477836295304383783820453092086974839448400254698134232250458309917484540068859490708973",
        "470024804755823422370520629225696512334307869157140926273118046324007307140982946239638012326007452650402");
    testOperatorSubtraction(
        "7050392822843069367725353867951204442321414418661736485822774515332123090871618842356838285923004150390625",
        "17984638288961211871838956989189665890197130672912829203311075745019255958028927299020895173379216649",
        "7050374838204780406513482028994215252655524221531063572993571204256378071615660813429539265027830771173976");
    testOperatorSubtraction(
        "105755892342646040515880308019268066634821216279926047287341617729981846363074282635352574288845062255859375",
        "233800297756495754333906440859465656572562698747866779643043984685250327454376054887271637253929816437",
        "105755658542348284020125974112827207169164643717227299420561974685997161112746828259297687017207808326042938");
    testOperatorSubtraction(
        "1586338385139690607738204620289020999522318244198890709310124265949727695446114239530288614332675933837890625",
        "3039403870834444806340783731173053535443315083722268135359571800908254256906888713534531284301087613681",
        "158633534573581977329339827950528982646878280088380698704198890637792678719185733264157507980139163275027694"
        "4");
    testOperatorSubtraction(
        "2379507577709535911607306930433531499283477366298336063965186398924591543169171359295432921499013900756835937"
        "5",
        "39512250320847782482430188505249695960763096088389485759674433411807305339789553275948906695914138977853",
        "2379503626484503826829058687414680974313881289988727225016610431481250362438637380340105326608344309342938152"
        "2");
    testOperatorSubtraction(
        "35692613665643038674109603956502972489252160494475040959477795983868873147537570389431493822485208511352539062"
        "5",
        "513659254171021172271592450568246047489920249149063314875767634353494969417264192587335787046883806712089",
        "35692562299717621571992376797257915664647411502450126053146308407105437798040628663012235088906503822971867853"
        "6");
    testOperatorSubtraction(
        "53538920498464558011164405934754458733878240741712561439216693975803309721306355584147240733727812767028808593"
        "75",
        "6677570304223275239530701857387198617368963238937823093384979246595434602424434503635365231609489487257157",
        "53538853722761515778412010627735884861892067052080172060985760126010843766960331339802204380075496672133936022"
        "18");
    testOperatorSubtraction(
        "80308380747696837016746608902131688100817361112568842158825040963704964581959533376220861100591719150543212890"
        "625",
        "86808413954902578113899124146033582025796522106191700214004730205740649831517648547259748010923363334343041",
        "80308293939282882114168495003007542067235335316046735967124826958974758841309701858572313840843708227179878547"
        "584");
    testOperatorSubtraction(
        "12046257112154525552511991335319753215122604166885326323823756144555744687293930006433129165088757872581481933"
        "59375",
        "1128509381413733515480688613898436566335354787380492102782061492674628447809729431114376724142003723346459533",
        "12046245827060711415176836528433614230756940813337452518902728323940817941009451909138818021321516452544248468"
        "99842");
    testOperatorSubtraction("180693856682317883287679870029796298226839062503279894857356342168336170309408950096496937"
                            "47633136808872222900390625",
                            "146706219583785357012489519806796753623596122359463973361667994047701698215264826044868974"
                            "13846048403503973929",
                            "180693709976098299502322857540276491430085438907157535393382980500342122607710734831670892"
                            "60735722962823819396416696");
    testOperatorSubtraction("271040785023476824931519805044694447340258593754919842286034513252504255464113425144745406"
                            "214497052133083343505859375",
                            "190718085458920964116236375748835779710674959067303165370168392262012207679844273858329666"
                            "379998629245551661077",
                            "271040594305391366010555688808318698504478883079960774982869143084111993451905745300471547"
                            "884830672134454097954198298");


}

#pragma warning(pop)