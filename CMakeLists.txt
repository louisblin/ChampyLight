cmake_minimum_required (VERSION 3.0.2)
project (champylight)

# configure building parameters
set (CMAKE_C_FLAGS "-g -Wall -Werror -pedantic -std=c99 -Wno-deprecated-declarations")
add_definitions (-D_XOPEN_SOURCE -D_BSD_SOURCE)

add_subdirectory (deamon)
add_subdirectory (controller)

# dummy temporary tests
add_custom_target (cxxtest cxxtestgen --error-printer -o runner.cpp `find ../*/test -name "*.h" `
    COMMAND g++ -Wall -o runner -I../controller/src -I../deamon/src runner.cpp
    COMMAND ./runner
    DEPENDS dmxd controller
    COMMENT "Testing...")
