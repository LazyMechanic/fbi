find_package(GTest CONFIG REQUIRED)

add_executable(test 
    "test.cc"
    "BigUnsignedTests.hh")

target_link_libraries(
    test
    fbi
    GTest::gtest
    GTest::gtest_main)

add_test(
    NAME fbiRunTest
    COMMAND test)