cmake_minimum_required(VERSION 3.5)

project(v8_inspector_example LANGUAGES CXX)

set(V8_INCLUDE_DIR "" CACHE PATH "Path to the V8 include dir.")
set(V8_MONOLITH_LIB_PATH "" CACHE FILEPATH "Path to the compiled V8 monolith *.a library.")
set(WS_INCLUDE_DIR "" CACHE FILEPATH "Path to the WebSocket++ include dir.")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB TARGET_H "${CMAKE_SOURCE_DIR}/*.h")
FILE(GLOB TARGET_CPP "${CMAKE_SOURCE_DIR}/*.cpp")
SET(SOURCE_FILES ${TARGET_CPP} ${TARGET_H})

add_executable(v8_inspector_example ${SOURCE_FILES})

include_directories(
    ${WS_INCLUDE_DIR}
    ${V8_INCLUDE_DIR}
)
add_library(v8_monolith STATIC IMPORTED)

set_target_properties(
    v8_monolith
    PROPERTIES
    IMPORTED_LOCATION
    ${V8_MONOLITH_LIB_PATH}
)

target_link_libraries(v8_inspector_example v8_monolith)
target_link_libraries(v8_inspector_example pthread)
target_link_libraries(v8_inspector_example stdc++fs)
