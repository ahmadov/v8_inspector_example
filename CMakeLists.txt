cmake_minimum_required(VERSION 3.5)

project(v8_inspector_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(V8_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/deps/include/" PATH "Path to the V8 include dir.")
set(V8_MONOLITH_LIB_PATH "${CMAKE_SOURCE_DIR}/deps/libv8_monolith.a" )
set(BOOST_BEAST_INCLUDE "${BOOST_ROOT}/include" PATH "Path to the Boost.Beast include dir." )

FILE(GLOB TARGET_H "${CMAKE_SOURCE_DIR}/src/*.h")
FILE(GLOB TARGET_CPP "${CMAKE_SOURCE_DIR}/src/*.cpp")
SET(SOURCE_FILES ${TARGET_CPP} ${TARGET_H})

add_executable(v8_inspector_example ${SOURCE_FILES})

include_directories(
        ${V8_INCLUDE_DIR}
        ${BOOST_BEAST_INCLUDE}
)

add_library(v8_monolith STATIC IMPORTED)
set_target_properties(
        v8_monolith
        PROPERTIES
        IMPORTED_LOCATION
        "${V8_MONOLITH_LIB_PATH}"
)

target_link_libraries(v8_inspector_example v8_monolith)
target_link_libraries(v8_inspector_example pthread)
target_link_libraries(v8_inspector_example stdc++fs)
target_link_libraries(v8_inspector_example dl)
