find_package(GTest CONFIG REQUIRED)

add_executable(fbiTests 
    "test.cc"
    "BigUnsignedTests.hh")

target_link_libraries(
    fbiTests
    fbi
    GTest::gtest
    GTest::gtest_main)

add_test(
    NAME fbiRunTest
    COMMAND fbiTests)