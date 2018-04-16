include_directories(src)
file (GLOB SOURCES src/*.c src/*.h)

add_executable (controller ${SOURCES})
target_link_libraries (controller LINK_PUBLIC curl m)
