# WSPR Reference Implementation

[![CI](https://github.com/WsprryPi/WSPR-Reference/actions/workflows/ci.yml/badge.svg)](https://github.com/WsprryPi/WSPR-Reference/actions)
[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)

A correctness-first WSPR reference encoder/decoder with explicit ambiguity handling.

## Quick Start

```bash
cmake -S . -B build
cmake --build build

./build/wspr-encode K1ABC FN20 30
./build/wspr-decode <symbols>
./build/wspr-correlate <type2_symbols> <type3_symbols>
```

## Example Output

```text
Encoded symbols: 3300200210221110...

Decoded callsign: K1ABC
Decoded locator:  FN20
Decoded power:    30

Correlated callsign: <hashed>/12
Correlated locator:  FN20AB
Correlated power:    30
```

## Overview

This project is a clean, testable WSPR reference implementation focused on
correctness, transparency, and reproducibility.

It supports:

- Type 1 messages
- Type 2 messages, including prefix and suffix forms
- Type 3 messages with hashed callsign and 6-character locator
- Type 2 and Type 3 correlation
- Human-readable, quiet, and JSON CLI output modes
- Regression coverage for known ambiguity in the Type 2 overlap region
- A reusable, installable C++ library API

## Components

- `wspr-encode`
- `wspr-decode`
- `wspr-correlate`
- `wspr_ref_lib`
- Regression suite
- API example

## CLI Tools

### Encode

```bash
./build/wspr-encode K1ABC FN20 30
./build/wspr-encode --quiet K1ABC FN20 30
./build/wspr-encode --json K1ABC FN20 30
```

### Decode

```bash
./build/wspr-decode <symbols>
./build/wspr-decode --quiet <symbols>
./build/wspr-decode --json <symbols>
```

### Correlate

```bash
./build/wspr-correlate --quiet <type2> <type3>
./build/wspr-correlate --json <type2> <type3>
```

## Type 2 Ambiguity Policy

Overlap region:

```text
/Z  ⇄  /09
/Q  ⇄  /00
```

Policy:

- Deterministic primary decode
- Alternate preserved
- No silent loss

## Regression Testing

```bash
./tests/run_major_regressions.sh
```

Overlap cases are observational, not failures.

## Reusable Library

```cpp
auto encoded = wspr::encode_message("K1ABC", "FN20", 30);
```

## Install

```bash
cmake -S . -B build
cmake --build build
cmake --install build --prefix ./install
```

## Using (CMake)

```cmake
find_package(wspr_ref CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE wspr::wspr_ref_lib)
```

## Status

- Core features complete
- Ambiguity handling complete
- Library export complete
- External consumption verified

## License

MIT License
