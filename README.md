# WSPR Reference Implementation

## Overview

This project provides a clean, testable reference implementation of WSPR
(Weak Signal Propagation Reporter) encoding, decoding, and correlation.

It supports:

- Type 1 (standard messages)
- Type 2 (extended callsigns with prefix/suffix)
- Type 3 (hashed callsign with 6-character locator)

The goal is correctness, transparency, and reproducibility — not just
successful decodes, but understanding why they work.

---

## Features

- Deterministic encoder (wspr-encode)
- Robust decoder (wspr-decode)
- Type 2 ↔ Type 3 correlator (wspr-correlate)
- Full regression suite with CLI assertions
- Explicit handling of Type 2 ambiguity

---

## CLI Tools

### Encode

```
./wspr-encode K1ABC FN20 30
./wspr-encode --quiet K1ABC FN20 30
./wspr-encode --symbols-only K1ABC FN20 30
```

---

### Decode

```
./wspr-decode <symbols>
./wspr-decode --quiet <symbols>
```

Example (quiet):

```
TYPE1 K1ABC FN20 30
TYPE2 <hashed> /12 30
TYPE3 <hashed> 6521 FN20AB 30
```

---

### Correlate

```
./wspr-correlate <type2_symbols> <type3_symbols>
./wspr-correlate --quiet <type2_symbols> <type3_symbols>
```

Example:

```
CORRELATED TYPE2 <hashed>/12 6521 FN20AB 30
```

---

## Type 2 Ambiguity (Important)

WSPR Type 2 messages contain an overlap in encoding space where:

- A one-character suffix (e.g. /Z)
- Can collide with a two-digit suffix (e.g. /09)

### Example overlap

```
/Z  ⇄  /09
```

### Decoder Behavior

The decoder uses a deterministic primary interpretation:

```
TYPE2 <hashed> /09 30
```

But also exposes the alternate interpretation:

```
TYPE2 <hashed> /09 30 ALT /Z
```

### Correlator Behavior

When correlating with a Type 3 message:

```
CORRELATED TYPE2 <hashed>/09 6521 FN20AB 30 ALT /Z
```

### Design Philosophy

- Do not guess silently
- Do not discard valid interpretations
- Surface ambiguity explicitly
- Keep output stable for automation

---

## Regression Testing

Run the full suite:

```
./tests/run_major_regressions.sh
```

Covers:

- All message types
- Encode/decode roundtrips
- CLI output assertions
- Type 2 ambiguity cases
- Correlation correctness

---

## Example Ambiguous Case

```
./wspr-encode --quiet K1ABC/Z FN20 30
./wspr-decode --quiet <symbols>
```

Output:

```
TYPE2 <hashed> /09 30 ALT /Z
```

---

## Status

- Type 1: Complete
- Type 2: Complete (including ambiguity handling)
- Type 3: Complete
- Correlation: Complete
- Regression coverage: Strong

---

## License

MIT License
