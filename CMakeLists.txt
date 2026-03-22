cmake_minimum_required(VERSION 3.16)
project(main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SRC_FILES src/*.cpp)
add_executable(main ${SRC_FILES}
        include/Text.h
        src/Text.cpp)

target_include_directories(main PRIVATE include)

target_compile_options(main PRIVATE -F/Library/Frameworks)

target_link_options(main PRIVATE
        -F/Library/Frameworks
        -Wl,-rpath,/Library/Frameworks
)

target_link_libraries(main PRIVATE
        "-framework SDL2"
        "-framework SDL2_ttf"
        "-framework SDL2_mixer"
)

configure_file(src/level10.txt level10.txt COPYONLY)
configure_file(src/ButterflyPlanet.mp3 ButterflyPlanet.mp3 COPYONLY)
configure_file(src/godoMaum.ttf godoMaum.ttf COPYONLY)
