# WSPR-Reference

A reference-oriented WSPR encoder, decoder, unpacker, and correlator in C++17.

## Overview

`WSPR-Reference` is a focused implementation of core WSPR message handling with
an emphasis on clarity, repeatability, and regression testing.

The project currently includes:

- A standalone WSPR reference encoder
- A Fano-based decoder pipeline for recovering payload bits from symbol streams
- Type 1 unpacking with full end-to-end validation
- Type 2 unpacking with validated prefix and suffix decoding
- Type 3 unpacking with validated locator and power recovery
- A correlator for combining Type 2 and Type 3 partial decodes into a more
  useful compound result
- Small command-line tools for encoding, decoding, and correlating messages
- Regression-style harnesses covering the major pipeline stages

This repository is intentionally structured as a reference implementation rather
than a highly optimized production decoder.

## Current capabilities

### Encoding

The encoder can generate 162-symbol WSPR streams for:

- Type 1 messages
- Type 2 compound callsign messages
- Type 3 hashed callsign plus 6-character locator messages

### Decoding

The decoder can recover payload bits from a 162-symbol WSPR stream using the
following stages:

- Symbol parsing and validation
- Sync-vector removal
- Deinterleaving
- Fano-style convolutional decoding
- Payload unpacking

### Unpacking

The unpacker currently supports:

- Type 1 full message unpacking
- Type 2 partial unpacking with decoded extension forms such as:
  - `/7`
  - `/12`
  - `W1/`
- Type 3 partial unpacking with:
  - hashed callsign placeholder
  - recovered 6-character locator
  - recovered power level

### Correlation

The correlator can combine a Type 2 partial decode and a Type 3 partial decode
into a more useful result, for example:

- `<hashed>/12`
- `W1/<hashed>`

along with locator, power, and hash metadata.

## Command-line tools

### `wspr-encode`

Encodes a WSPR message and prints the resulting symbol stream.

Example:

```bash
./wspr-encode K1ABC FN20 30
./wspr-encode K1ABC/12 FN20 30
./wspr-encode "<K1ABC>" FN20AB 30
```

### `wspr-decode`

Decodes a single 162-symbol WSPR stream and prints recovered payload bits plus
the best available unpacked result.

Example:

```bash
./wspr-decode 330020021022111022120121133220000232032322002012130033030021303020033032301010232030110201323012223220221021021112330211212021312202030320112222222330323322013222
```

### `wspr-correlate`

Decodes two symbol streams and attempts to correlate them as a Type 2 and Type 3
pair.

Example:

```bash
./wspr-correlate 330220021020113022100321133020200230032120022210130233030001301022033232321210012232130003103030203220001221023112330233230223312202030122132020202132103322031020 332002201020111022320121311022222212032302022230130033230021301202031232301032230032310221303032221222201023223312130031030203132200010100132000220330303120213202
```

## Build

The project uses CMake.

```bash
mkdir -p build
cd build
cmake ..
make
```

## Dependencies

The main encoder and decoder pipeline is self-contained and uses the C++
standard library.

The vector verification tooling depends on `nlohmann/json` via the repository's
local include tree:

```text
include/nlohmann/json.hpp
```

No external package manager integration is required if that header is present in
the repository.

## Project layout

```text
src/
  encode_main.cpp
  decode_main.cpp
  correlate_main.cpp
  wspr/
    wspr_constants.hpp
    wspr_ref_encoder.hpp
    wspr_ref_encoder.cpp
    wspr_ref_decoder.hpp
    wspr_ref_decoder.cpp
    wspr_ref_fano.hpp
    wspr_ref_fano.cpp
    wspr_ref_unpack.hpp
    wspr_ref_unpack.cpp
    wspr_ref_correlator.hpp
    wspr_ref_correlator.cpp

tests/
  verify_vectors.cpp
  fano_sanity.cpp
  fano_branch_ordering.cpp
  fano_bounded_decode.cpp
  fano_payload_roundtrip.cpp
  fano_limited_decode.cpp
  fano_compare_short.cpp
  fano_compare_24.cpp
  fano_full_payload_roundtrip.cpp
  payload_compare_roundtrip.cpp
  unpack_type1_roundtrip.cpp
  unpack_type2_smoke.cpp
  unpack_type2_roundtrip.cpp
  unpack_type2_matrix.cpp
  unpack_type2_overlap_observation.cpp
  unpack_type3_smoke.cpp
  unpack_type3_roundtrip.cpp
  correlator_smoke.cpp
```

## Validation and test coverage

The repository includes a growing set of focused regression harnesses.

### Encoder / vector validation

- Golden vector verification
- Payload round-trip comparison between encoder and decoder payload views

### Fano decoder validation

- Parity sanity checks
- Branch-order sanity checks
- Bounded-depth decode prototype checks
- Limited thresholded decode checks
- Short and 24-bit comparison harnesses
- Full 81-bit payload round-trip validation

### Unpacking validation

- Type 1 round-trip validation
- Type 2 smoke and round-trip validation
- Type 2 matrix coverage across multiple prefix and suffix forms
- Type 3 smoke and round-trip validation

### Correlator validation

- Smoke test for Type 2 / Type 3 pairing
- Human-readable combined output validation through the correlator CLI

## Type 2 ambiguity note

There is a known ambiguity region in Type 2 extension decoding.

The current test and observation work shows an overlap in the encoded extension
space where some values can plausibly be interpreted as either:

- one-character suffixes in the range `/Q` through `/Z`
- wrapped two-digit suffixes in the range `/00` through `/12`

The current implementation preserves the decode policy that is already validated
against the repository's known-good vectors, including:

- `/7`
- `/12`
- `W1/`

An observation harness is included to make this overlap explicit:

```bash
./unpack_type2_overlap_observation
```

At this stage, ambiguity handling is intentionally deferred until after broader
validation coverage. In other words, the project currently favors stability for
known-good vectors over claiming universal Type 2 disambiguation.

## Status summary

The repository currently has validated support for:

- Type 1 end-to-end decode
- Type 2 extension decode for known prefix and suffix forms
- Type 3 partial decode with correct locator and power recovery
- Type 2 / Type 3 correlation into human-readable combined output

## License

This project is licensed under the MIT License.

See `LICENSE.md` for details.
