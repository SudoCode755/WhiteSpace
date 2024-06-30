include(GoogleTest)

add_executable(WhiteSpace-Test 
whiteSpace.test.cpp
stack_manip.test.cpp
arithmetic.test.cpp
heap_access.test.cpp
io_ops.test.cpp
flow_control.test.cpp
)

find_package(GTest)


target_link_libraries(WhiteSpace-Test PRIVATE
    GTest::gtest_main    
    WhiteSpace-Core
)

gtest_discover_tests(WhiteSpace-Test)
