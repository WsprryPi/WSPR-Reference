#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${REPO_ROOT}/build"

TYPE1_SYMBOLS="330020021022111022120121133220000232032322002012130033030021303020033032301010232030110201323012223220221021021112330211212021312202030320112222222330323322013222"
TYPE2_SUFFIX_SYMBOLS="330220021020113022100321133020200230032120022210130233030001301022033232321210012232130003103030203220001221023112330233230223312202030122132020202132103322031020"
TYPE2_PREFIX_SYMBOLS="330220221022113022100123133020000030010122022010130233230021323022033232301212012032132003303210223022021223001110330213210021112200012120132000202330303320031022"
TYPE3_SYMBOLS="332002201020111022320121311022222212032302022230130033230021301202031232301032230032310221303032221222201023223312130031030203132200010100132000220330303120213202"

printf "Repository root: %s\n" "${REPO_ROOT}"
printf "Build directory: %s\n\n" "${BUILD_DIR}"

cd "${BUILD_DIR}"

printf "== Building ==\n"
cmake ..
make
printf "\n"

printf "== Running core regression executables ==\n"
./verify_vectors
./payload_compare_roundtrip
./unpack_type1_roundtrip
./unpack_type2_roundtrip
./unpack_type3_roundtrip
./correlator_smoke
printf "\n"

printf "== Running Type 2 coverage observations (non-fatal) ==\n"
./unpack_type2_matrix || true
./unpack_type2_overlap_observation || true
printf "\n"

printf "== Running CLI encode/decode sanity checks ==\n"
./wspr-encode K1ABC FN20 30
printf "\n"

./wspr-decode "${TYPE1_SYMBOLS}"
printf "\n"

./wspr-decode "${TYPE2_SUFFIX_SYMBOLS}"
printf "\n"

./wspr-decode "${TYPE2_PREFIX_SYMBOLS}"
printf "\n"

./wspr-decode "${TYPE3_SYMBOLS}"
printf "\n"

printf "== Running correlator CLI sanity checks ==\n"
./wspr-correlate "${TYPE2_SUFFIX_SYMBOLS}" "${TYPE3_SYMBOLS}"
printf "\n"

./wspr-correlate "${TYPE2_PREFIX_SYMBOLS}" "${TYPE3_SYMBOLS}"
printf "\n"

printf "All major regressions completed successfully.\n"
