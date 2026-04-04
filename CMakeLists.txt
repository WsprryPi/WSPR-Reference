cmake_minimum_required(VERSION 3.16)

project(wspr_ref LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(wspr-ref
    src/main.cpp
    src/wspr/wspr_ref_encoder.cpp
)

target_include_directories(wspr-ref
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(wspr-ref
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(verify_vectors
    tests/verify_vectors.cpp
    src/wspr/wspr_ref_encoder.cpp
)

target_include_directories(verify_vectors
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(verify_vectors
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
