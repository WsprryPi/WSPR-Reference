cmake_minimum_required(VERSION 3.16)

project(wspr_ref LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(wspr_ref_lib
    src/wspr/wspr_ref_api.cpp
    src/wspr/wspr_ref_correlator.cpp
    src/wspr/wspr_ref_decoder.cpp
    src/wspr/wspr_ref_encoder.cpp
    src/wspr/wspr_ref_fano.cpp
    src/wspr/wspr_ref_unpack.cpp
)

target_include_directories(wspr_ref_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(wspr_ref_lib
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

function(wspr_ref_target target)
    target_include_directories(${target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_compile_options(${target}
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endfunction()

add_executable(wspr-ref
    src/main.cpp
)
target_link_libraries(wspr-ref PRIVATE wspr_ref_lib)
wspr_ref_target(wspr-ref)

add_executable(verify_vectors
    tests/verify_vectors.cpp
)
target_link_libraries(verify_vectors PRIVATE wspr_ref_lib)
wspr_ref_target(verify_vectors)

add_executable(wspr-decode
    src/decode_main.cpp
)
target_link_libraries(wspr-decode PRIVATE wspr_ref_lib)
wspr_ref_target(wspr-decode)

add_executable(fano_sanity
    tests/fano_sanity.cpp
    src/wspr/wspr_ref_fano.cpp
)
wspr_ref_target(fano_sanity)

add_executable(fano_branch_ordering
    tests/fano_branch_ordering.cpp
    src/wspr/wspr_ref_fano.cpp
)
wspr_ref_target(fano_branch_ordering)

add_executable(fano_bounded_decode
    tests/fano_bounded_decode.cpp
    src/wspr/wspr_ref_fano.cpp
)
wspr_ref_target(fano_bounded_decode)

add_executable(fano_payload_roundtrip
    tests/fano_payload_roundtrip.cpp
    src/wspr/wspr_ref_fano.cpp
)
wspr_ref_target(fano_payload_roundtrip)

add_executable(fano_limited_decode
    tests/fano_limited_decode.cpp
    src/wspr/wspr_ref_fano.cpp
)
wspr_ref_target(fano_limited_decode)

add_executable(fano_compare_short
    tests/fano_compare_short.cpp
    src/wspr/wspr_ref_fano.cpp
)
wspr_ref_target(fano_compare_short)

add_executable(fano_compare_24
    tests/fano_compare_24.cpp
    src/wspr/wspr_ref_fano.cpp
)
wspr_ref_target(fano_compare_24)

add_executable(fano_full_payload_roundtrip
    tests/fano_full_payload_roundtrip.cpp
    src/wspr/wspr_ref_fano.cpp
)
wspr_ref_target(fano_full_payload_roundtrip)

add_executable(payload_compare_roundtrip
    tests/payload_compare_roundtrip.cpp
)
target_link_libraries(payload_compare_roundtrip PRIVATE wspr_ref_lib)
wspr_ref_target(payload_compare_roundtrip)

add_executable(unpack_type1_roundtrip
    tests/unpack_type1_roundtrip.cpp
)
target_link_libraries(unpack_type1_roundtrip PRIVATE wspr_ref_lib)
wspr_ref_target(unpack_type1_roundtrip)

add_executable(unpack_type2_smoke
    tests/unpack_type2_smoke.cpp
    src/wspr/wspr_ref_unpack.cpp
)
wspr_ref_target(unpack_type2_smoke)

add_executable(unpack_type3_smoke
    tests/unpack_type3_smoke.cpp
    src/wspr/wspr_ref_unpack.cpp
)
wspr_ref_target(unpack_type3_smoke)

add_executable(correlator_smoke
    tests/correlator_smoke.cpp
)
target_link_libraries(correlator_smoke PRIVATE wspr_ref_lib)
wspr_ref_target(correlator_smoke)

add_executable(unpack_type2_roundtrip
    tests/unpack_type2_roundtrip.cpp
)
target_link_libraries(unpack_type2_roundtrip PRIVATE wspr_ref_lib)
wspr_ref_target(unpack_type2_roundtrip)

add_executable(wspr-encode
    src/encode_main.cpp
)
target_link_libraries(wspr-encode PRIVATE wspr_ref_lib)
wspr_ref_target(wspr-encode)

add_executable(unpack_type3_roundtrip
    tests/unpack_type3_roundtrip.cpp
)
target_link_libraries(unpack_type3_roundtrip PRIVATE wspr_ref_lib)
wspr_ref_target(unpack_type3_roundtrip)

add_executable(wspr-correlate
    src/correlate_main.cpp
)
target_link_libraries(wspr-correlate PRIVATE wspr_ref_lib)
wspr_ref_target(wspr-correlate)

add_executable(unpack_type2_matrix
    tests/unpack_type2_matrix.cpp
)
target_link_libraries(unpack_type2_matrix PRIVATE wspr_ref_lib)
wspr_ref_target(unpack_type2_matrix)

add_executable(unpack_type2_overlap_observation
    tests/unpack_type2_overlap_observation.cpp
)
wspr_ref_target(unpack_type2_overlap_observation)

add_executable(unpack_type2_ambiguity
    tests/unpack_type2_ambiguity.cpp
)
target_link_libraries(unpack_type2_ambiguity PRIVATE wspr_ref_lib)
wspr_ref_target(unpack_type2_ambiguity)

add_executable(api_example
    examples/api_example.cpp
)
target_link_libraries(api_example PRIVATE wspr_ref_lib)
wspr_ref_target(api_example)
