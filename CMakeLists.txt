cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB SRC_FILES 
    src/*.cpp
    src/GUI/*.cpp
    src/Core/*.cpp
    src/State/*.cpp
    src/SceneNode/*.cpp
    src/DataStructures/*.cpp
)

add_executable(main ${SRC_FILES} ${HEADER_FILES})

# add_executable(main src/main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

if (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" OR CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -fsanitize=undefined -fsanitize=address")
    target_link_options(main BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_definitions(-DSFML_NO_DEPRECATED_WARNINGS)
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")

message(STATUS "🔍 SRC_FILES found:")
foreach(file ${SRC_FILES})
    message(STATUS "  - ${file}")
endforeach()