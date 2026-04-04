# WSPR Reference (wspr-ref)

A standalone, dependency-free (except for JSON parsing) reference implementation of WSPR (Weak Signal Propagation Reporter) encoding.

## Overview

This project provides a clean, portable C++ implementation of WSPR encoding modeled after JTEncode and provided as a minimal, self-contained codebase.

It supports:

- Type 1: Standard callsign + 4-character locator + power  
- Type 2: Compound callsigns (e.g. K1ABC/7, W1/K1ABC)  
- Type 3: Extended callsigns and 6-character locators  

The encoder produces the full 162-symbol sequence (values 0–3) used for WSPR transmission.

---

## Features

- No Arduino or embedded dependencies  
- No vendor library dependency  
- Deterministic, reproducible output  
- Verified against JTEncode  
- Golden vector regression testing  

---

## Build

```bash
mkdir build
cd build
cmake ..
make
```

---

## Usage

### Encode a message

```bash
./wspr-ref K1ABC FN20 30
```

Example output:

```text
Type: TYPE1
Callsign: K1ABC
Locator: FN20
Power: 30 dBm
Symbols: 3300200210...
```

---

## Verification (Golden Vectors)

This project includes a verification tool to ensure the encoder remains correct.

Run:

```bash
./verify_vectors
```

Example output:

```text
PASS: TYPE1 K1ABC FN20 30
PASS: TYPE2 K1ABC/7 FN20 30
...
Summary: PASS=6 FAIL=0
```

---

## Test Vectors

Golden reference vectors are stored in:

```bash
test_vectors/wspr_golden_vectors.json
```

IMPORTANT: Do not modify these casually — they are the ground truth used to validate correctness.

---

## Dependencies

This project uses a single header-only dependency:

```bash
nlohmann/json
```

The header is included locally in:

```c++
include/nlohmann/json.hpp
```

No external installation is required.

---

## Project Structure

```text
wspr-ref/
├── CMakeLists.txt
├── LICENSE.md
├── README.md
├── include/
│   └── nlohmann/json.hpp
├── test_vectors/
│   ├── README.md
│   └── wspr_golden_vectors.json
├── tests/
│   └── verify_vectors.cpp
└── src/
    ├── main.cpp
    └── wspr/
        ├── wspr_ref_encoder.hpp
        └── wspr_ref_encoder.cpp
```

---

## Design Goals

- Minimal and readable reference implementation  
- No hidden dependencies  
- Suitable for experimentation and validation  
- Foundation for decoder development  

---

## Future Work

- Symbol-string decoder  
- Round-trip encode/decode validation  
- Additional test vectors  
- CI-based regression testing  

---

## License

This project is licensed under the MIT License.

See LICENSE.md for details.
