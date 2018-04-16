include_directories(src)
file (GLOB SOURCES src/*.c src/*.h)

add_executable (dmxd ${SOURCES})
target_link_libraries (dmxd LINK_PUBLIC usb m)
