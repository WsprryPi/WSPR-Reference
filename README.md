# WSPR Reference Implementation

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

The project now includes both reusable library APIs and thin CLI wrappers.

## Components

The repository currently provides:

- `wspr-encode` for symbol generation
- `wspr-decode` for symbol-to-message decoding
- `wspr-correlate` for pairing Type 2 and Type 3 messages
- `wspr_ref_lib` as the shared library target
- A focused regression suite and ambiguity observation harnesses

## CLI Tools

### Encode

Verbose output:

```text
./build/wspr-encode K1ABC FN20 30
```

Quiet output:

```text
./build/wspr-encode --quiet K1ABC FN20 30
./build/wspr-encode --symbols-only K1ABC FN20 30
```

JSON output:

```text
./build/wspr-encode --json K1ABC FN20 30
```

### Decode

Verbose output:

```text
./build/wspr-decode 330020021022111022120121133220000232032322002012130033030021303020033032301010232030110201323012223220221021021112330211212021312202030320112222222330323322013222
```

Quiet output:

```text
./build/wspr-decode --quiet 330020021022111022120121133220000232032322002012130033030021303020033032301010232030110201323012223220221021021112330211212021312202030320112222222330323322013222
```

JSON output:

```text
./build/wspr-decode --json 330020021022111022120121133220000232032322002012130033030021303020033032301010232030110201323012223220221021021112330211212021312202030320112222222330323322013222
```

### Correlate

Quiet output:

```text
./build/wspr-correlate --quiet \
330220021020113022100321133020200230032120022210130233030001301022033232321210012232130003103030203220001221023112330233230223312202030122132020202132103322031020 \
332002201020111022320121311022222212032302022230130033230021301202031232301032230032310221303032221222201023223312130031030203132200010100132000220330303120213202
```

JSON output:

```text
./build/wspr-correlate --json \
330220021020113022100321133020200230032120022210130233030001301022033232321210012232130003103030203220001221023112330233230223312202030122132020202132103322031020 \
332002201020111022320121311022222212032302022230130033230021301202031232301032230032310221303032221222201023223312130031030203132200010100132000220330303120213202
```

## JSON Output

All three CLIs support JSON output.

Examples:

```text
./build/wspr-encode --json K1ABC FN20 30
./build/wspr-decode --json <symbols>
./build/wspr-correlate --json <type2_symbols> <type3_symbols>
```

The JSON modes are intended for scripting, integration, and future library
consumers.

## Type 2 Ambiguity Policy

Type 2 extension decoding contains a known overlap region where some values can
be interpreted either as a one-character suffix or as a wrapped two-digit
suffix.

Example overlap:

```text
/Z  ⇄  /09
```

The decoder uses a deterministic primary interpretation while preserving the
alternate interpretation explicitly.

Quiet decode example:

```text
TYPE2 <hashed> /09 30 ALT /Z
```

Quiet correlate example:

```text
CORRELATED TYPE2 <hashed>/09 6521 FN20AB 30 ALT /Z
```

JSON example:

```json
{
  "type": "TYPE2",
  "callsign": "<hashed>",
  "extra": "/09",
  "power_dbm": 30,
  "is_partial": true,
  "has_hash": false,
  "has_ambiguity": true,
  "alternate_extra": "/Z"
}
```

This policy is intentional:

- keep the primary decode deterministic
- expose overlap honestly
- avoid silently discarding valid alternate interpretations
- keep CLI and JSON output stable for automation

## Regression Testing

Run the main regression script with:

```text
./tests/run_major_regressions.sh
```

The script covers:

- encode and decode round-trips
- Type 1, Type 2, and Type 3 decode paths
- quiet CLI output
- JSON-related behavior through dedicated spot checks
- Type 2 and Type 3 correlation
- ambiguity surfacing in overlap cases

### Note on overlap observations

The Type 2 matrix and overlap observation checks include known ambiguous
extension values. In that overlap region, the script is intended to report the
decoder's primary interpretation and alternate interpretation rather than treat
the case as an unexpected regression failure.

In other words, the overlap harness is observational coverage, not a strict
conformance failure for known ambiguous values.

## Reusable Library API

The codebase now exposes a small reusable API in:

```text
src/wspr/wspr_ref_api.hpp
src/wspr/wspr_ref_api.cpp
```

Current API surface:

- `wspr::encode_message(...)`
- `wspr::decode_symbols(...)`
- `wspr::correlate_messages(...)`
- `wspr::correlate_symbol_streams(...)`

Key result types:

- `wspr::WsprEncodeResult`
- `wspr::WsprDecodedMessage`
- `wspr::WsprCorrelateResult`

The CLI tools are thin wrappers over this API.

## Tiny API Usage Example

A minimal example using the reusable API:

```cpp
#include "wspr/wspr_ref_api.hpp"

#include <iostream>
#include <string>

int main()
{
    const auto encoded = wspr::encode_message("K1ABC", "FN20", 30);
    if (!encoded.ok)
    {
        std::cerr << encoded.error << "\n";
        return 1;
    }

    std::cout << "Encoded symbols: " << encoded.symbols << "\n";

    wspr::WsprDecodedMessage decoded;
    std::string error;
    if (!wspr::decode_symbols(encoded.symbols, decoded, error))
    {
        std::cerr << error << "\n";
        return 1;
    }

    std::cout << "Decoded callsign: " << decoded.callsign << "\n";

    const auto type2 = wspr::encode_message("K1ABC/12", "FN20", 30);
    const auto type3 = wspr::encode_message("<K1ABC>", "FN20AB", 30);

    const auto correlated =
        wspr::correlate_symbol_streams(type2.symbols, type3.symbols);

    if (correlated.ok && correlated.correlated)
        std::cout << "Correlated callsign: "
                  << correlated.resolved.callsign << "\n";

    return 0;
}
```

## Build

Typical build flow:

```text
cmake -S . -B build
cmake --build build
```

## Project Status

Current status:

- Type 1 support: complete
- Type 2 support: complete, including ambiguity surfacing
- Type 3 support: complete
- Correlation: complete
- JSON CLI support: complete
- Reusable library API: introduced
- Regression coverage: strong

## License

MIT License
