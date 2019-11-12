find_package(GTest CONFIG REQUIRED)

add_executable(fbiTests 
    "test.cc"
    "BigUnsignedTests.hh")

target_link_libraries(
    fbiTests
    fbi
    GTest::gtest
    GTest::gtest_main)

set_target_properties(
    fbiTests PROPERTIES 
    CXX_STANDARD          17
    CXX_EXTENSIONS        OFF
    CXX_STANDARD_REQUIRED YES)

add_test(
    NAME fbiTests
    COMMAND fbiTests)