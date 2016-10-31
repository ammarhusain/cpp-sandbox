enable_testing()

add_library(compile-failures fail.cpp)
# Include the macro to build compile time failure tests.
include(static_fail.cmake)

add_fail_test(fail.cpp 2 compile-failures)
