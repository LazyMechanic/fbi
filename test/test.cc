#pragma warning(push)
// Disable gtest warnings
#pragma warning(disable : 26495)
#pragma warning(disable : 26812)

#include <gtest/gtest.h>

#include "BigUnsignedTests.hh"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#pragma warning(pop)