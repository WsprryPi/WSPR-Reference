#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
BUILD_DIR="${REPO_ROOT}/build"

TYPE1_SYMBOLS="330020021022111022120121133220000232032322002012130033030021303020033032301010232030110201323012223220221021021112330211212021312202030320112222222330323322013222"
TYPE2_SUFFIX_SYMBOLS="330220021020113022100321133020200230032120022210130233030001301022033232321210012232130003103030203220001221023112330233230223312202030122132020202132103322031020"
TYPE2_PREFIX_SYMBOLS="330220221022113022100123133020000030010122022010130233230021323022033232301212012032132003303210223022021223001110330213210021112200012120132000202330303320031022"
TYPE3_SYMBOLS="332002201020111022320121311022222212032302022230130033230021301202031232301032230032310221303032221222201023223312130031030203132200010100132000220330303120213202"

TYPE1_QUIET_EXPECTED="TYPE1 K1ABC FN20 30"
TYPE2_SUFFIX_QUIET_EXPECTED="TYPE2 <hashed> /12 30"
TYPE2_PREFIX_QUIET_EXPECTED="TYPE2 <hashed> W1/ 30"
TYPE3_QUIET_EXPECTED="TYPE3 <hashed> 6521 FN20AB 30"
CORRELATED_SUFFIX_QUIET_EXPECTED="CORRELATED TYPE2 <hashed>/12 6521 FN20AB 30"
CORRELATED_PREFIX_QUIET_EXPECTED="CORRELATED TYPE2 W1/<hashed> 6521 FN20AB 30"

assert_contains() {
    local haystack="$1"
    local needle="$2"
    local label="$3"

    if [[ "${haystack}" != *"${needle}"* ]]; then
        printf "ASSERTION FAILED: %s\n" "${label}" >&2
        printf "Expected to find: %s\n" "${needle}" >&2
        exit 1
    fi
}

run_and_assert() {
    local label="$1"
    local cmd="$2"
    shift 2
    local output

    printf "== %s ==\n" "${label}"
    output="$(eval "${cmd}")"
    printf "%s\n\n" "${output}"

    while (( "$#" > 0 )); do
        assert_contains "${output}" "$1" "${label}"
        shift
    done
}

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

run_and_assert \
    "CLI encode sanity" \
    "./wspr-encode K1ABC FN20 30" \
    "Type: TYPE1" \
    "Callsign: K1ABC" \
    "Locator: FN20" \
    "Power: 30 dBm" \
    "Symbols:"

run_and_assert \
    "CLI decode Type 1" \
    "./wspr-decode ${TYPE1_SYMBOLS}" \
    "Decoded Type 1 message:" \
    "Callsign: K1ABC" \
    "Locator:  FN20" \
    "Power:    30"

run_and_assert \
    "CLI decode Type 2 suffix" \
    "./wspr-decode ${TYPE2_SUFFIX_SYMBOLS}" \
    "Decoded Type 2 partial message:" \
    "Callsign: <hashed>" \
    "Extra:    /12" \
    "Power:    30"

run_and_assert \
    "CLI decode Type 2 prefix" \
    "./wspr-decode ${TYPE2_PREFIX_SYMBOLS}" \
    "Decoded Type 2 partial message:" \
    "Callsign: <hashed>" \
    "Extra:    W1/" \
    "Power:    30"

run_and_assert \
    "CLI decode Type 3" \
    "./wspr-decode ${TYPE3_SYMBOLS}" \
    "Decoded Type 3 partial message:" \
    "Callsign: <hashed>" \
    "Hash:     6521" \
    "Locator:  FN20AB" \
    "Power:    30"

run_and_assert \
    "CLI correlate suffix" \
    "./wspr-correlate ${TYPE2_SUFFIX_SYMBOLS} ${TYPE3_SYMBOLS}" \
    "Correlated result" \
    "Callsign: <hashed>/12" \
    "Locator:  FN20AB" \
    "Power:    30"

run_and_assert \
    "CLI correlate prefix" \
    "./wspr-correlate ${TYPE2_PREFIX_SYMBOLS} ${TYPE3_SYMBOLS}" \
    "Correlated result" \
    "Callsign: W1/<hashed>" \
    "Locator:  FN20AB" \
    "Power:    30"

run_and_assert \
    "CLI encode quiet" \
    "./wspr-encode --quiet K1ABC FN20 30" \
    "330020021022111022120121133220000232032322002012130033030021303020033032301010232030110201323012223220221021021112330211212021312202030320112222222330323322013222"

run_and_assert \
    "CLI encode symbols-only" \
    "./wspr-encode --symbols-only K1ABC FN20 30" \
    "330020021022111022120121133220000232032322002012130033030021303020033032301010232030110201323012223220221021021112330211212021312202030320112222222330323322013222"

run_and_assert \
    "CLI decode Type 1 quiet" \
    "./wspr-decode --quiet ${TYPE1_SYMBOLS}" \
    "${TYPE1_QUIET_EXPECTED}"

run_and_assert \
    "CLI decode Type 2 suffix quiet" \
    "./wspr-decode --quiet ${TYPE2_SUFFIX_SYMBOLS}" \
    "${TYPE2_SUFFIX_QUIET_EXPECTED}"

run_and_assert \
    "CLI decode Type 2 prefix quiet" \
    "./wspr-decode --quiet ${TYPE2_PREFIX_SYMBOLS}" \
    "${TYPE2_PREFIX_QUIET_EXPECTED}"

run_and_assert \
    "CLI decode Type 3 quiet" \
    "./wspr-decode --quiet ${TYPE3_SYMBOLS}" \
    "${TYPE3_QUIET_EXPECTED}"

run_and_assert \
    "CLI correlate suffix quiet" \
    "./wspr-correlate --quiet ${TYPE2_SUFFIX_SYMBOLS} ${TYPE3_SYMBOLS}" \
    "${CORRELATED_SUFFIX_QUIET_EXPECTED}"

run_and_assert \
    "CLI correlate prefix quiet" \
    "./wspr-correlate --quiet ${TYPE2_PREFIX_SYMBOLS} ${TYPE3_SYMBOLS}" \
    "${CORRELATED_PREFIX_QUIET_EXPECTED}"

printf "All major regressions completed successfully.\n"
