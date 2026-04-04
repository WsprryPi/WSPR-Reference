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

add_executable(wspr-decode
    src/decode_main.cpp
    src/wspr/wspr_ref_decoder.cpp
    src/wspr/wspr_ref_fano.cpp
)

target_include_directories(wspr-decode
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(wspr-decode
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(fano_sanity
    tests/fano_sanity.cpp
    src/wspr/wspr_ref_fano.cpp
)

target_include_directories(fano_sanity
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(fano_sanity
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(fano_branch_ordering
    tests/fano_branch_ordering.cpp
    src/wspr/wspr_ref_fano.cpp
)

target_include_directories(fano_branch_ordering
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(fano_branch_ordering
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(fano_bounded_decode
    tests/fano_bounded_decode.cpp
    src/wspr/wspr_ref_fano.cpp
)

target_include_directories(fano_bounded_decode
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(fano_bounded_decode
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(fano_payload_roundtrip
    tests/fano_payload_roundtrip.cpp
    src/wspr/wspr_ref_fano.cpp
)

target_include_directories(fano_payload_roundtrip
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(fano_payload_roundtrip
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(fano_limited_decode
    tests/fano_limited_decode.cpp
    src/wspr/wspr_ref_fano.cpp
)

target_include_directories(fano_limited_decode
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(fano_limited_decode
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(fano_compare_short
    tests/fano_compare_short.cpp
    src/wspr/wspr_ref_fano.cpp
)

target_include_directories(fano_compare_short
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(fano_compare_short
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(fano_compare_24
    tests/fano_compare_24.cpp
    src/wspr/wspr_ref_fano.cpp
)

target_include_directories(fano_compare_24
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(fano_compare_24
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(fano_full_payload_roundtrip
    tests/fano_full_payload_roundtrip.cpp
    src/wspr/wspr_ref_fano.cpp
)

target_include_directories(fano_full_payload_roundtrip
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(fano_full_payload_roundtrip
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(payload_compare_roundtrip
    tests/payload_compare_roundtrip.cpp
    src/wspr/wspr_ref_encoder.cpp
    src/wspr/wspr_ref_decoder.cpp
    src/wspr/wspr_ref_fano.cpp
)

target_include_directories(payload_compare_roundtrip
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(payload_compare_roundtrip
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(unpack_type1_roundtrip
    tests/unpack_type1_roundtrip.cpp
    src/wspr/wspr_ref_encoder.cpp
    src/wspr/wspr_ref_decoder.cpp
    src/wspr/wspr_ref_fano.cpp
    src/wspr/wspr_ref_unpack.cpp
)

target_include_directories(unpack_type1_roundtrip
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(unpack_type1_roundtrip
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
