# WSPR Compound Callsign Ambiguity

## Overview

This document explains a subtle but important behavior in WSPR (Weak
Signal Propagation Reporter): ambiguity when decoding compound callsigns
that include a slash ("/"), especially with single-character suffixes.

------------------------------------------------------------------------

## What Is a Compound Callsign

A compound callsign includes a slash:

- K1ABC/P
- K1ABC/1
- PJ4/K1ABC

These cannot fit into a standard WSPR message.

------------------------------------------------------------------------

## Why WSPR Splits Messages

WSPR messages are extremely compact.

A standard message encodes:

- Callsign
- Grid locator
- Power

Compound callsigns exceed this capacity.

------------------------------------------------------------------------

## Message Flow (Conceptual Diagram)

    Time Slot 1 (Type 2)
    +---------------------------+
    | Callsign + Prefix/Suffix |
    | Power                    |
    +---------------------------+

    Time Slot 2 (Type 3)
    +---------------------------+
    | Locator (6 char)          |
    | Callsign Hash             |
    +---------------------------+

Both are required for full reconstruction.

------------------------------------------------------------------------

## The Ambiguity Problem

Example:

- K1ABC/P
- K1ABC/1

When suffix length is 1:

- Encoded as a compact numeric value
- No explicit type flag (letter vs digit)

Result:

Same encoded value may represent: - Letter - Digit

------------------------------------------------------------------------

## What the Receiver Sees

From Type 2 alone:

    Base: K1ABC
    Suffix: ?
    Candidates: [P, 1]

This is inherently ambiguous.

------------------------------------------------------------------------

## How WSPR Resolves It

Type 3 contains a hash of the full callsign.

Resolution process:

1. Generate candidates:

    - K1ABC/P
    - K1ABC/1

2. Compute hash for each

3. Compare to received hash

4. Select match

------------------------------------------------------------------------

## Decoder Flow Diagram

    Type 2 Decode
          |
          v
    Generate Candidates
          |
          v
    Wait for Type 3
          |
          v
    Hash Compare
          |
          v
    Resolved Callsign

------------------------------------------------------------------------

## Why Both Messages Matter

Without Type 3:

- Callsign remains ambiguous
- Incorrect reporting possible

Reliable decoding requires BOTH:

- Type 2 (structure)
- Type 3 (validation)

------------------------------------------------------------------------

## Valid Prefix and Suffix Rules

### Prefix

- Up to 3 characters
- Before callsign

Examples:

- F/K1ABC
- PJ4/K1ABC

------------------------------------------------------------------------

### Suffix

- One letter OR
- One or two digits

Examples:

- K1ABC/P
- K1ABC/1
- K1ABC/12

------------------------------------------------------------------------

### Limitation

Only one allowed:

- Prefix OR suffix

Invalid:

- PJ4/K1ABC/P

------------------------------------------------------------------------

## Edge Case: Single Character Ambiguity

Single-character suffix may be:

- Letter (P)
- Digit (1)

Cannot be resolved without Type 3.

------------------------------------------------------------------------

## Edge Case: Overlapping Encodings

Certain numeric encodings can overlap:

- Single digit vs compacted multi-digit space

This reinforces need for hash validation.

------------------------------------------------------------------------

## Decoder Implementation Guidance

### Step 1: Decode Type 2

Extract:

- Base callsign
- Encoded suffix/prefix

------------------------------------------------------------------------

### Step 2: Generate Candidates

If suffix length == 1:

- Interpret as letter
- Interpret as digit

------------------------------------------------------------------------

### Step 3: Defer Decision

Mark as ambiguous.

------------------------------------------------------------------------

### Step 4: Decode Type 3

Extract:

- Locator
- Hash

------------------------------------------------------------------------

### Step 5: Resolve

Match hash against candidates.

------------------------------------------------------------------------

## Recommended Debug Output

    Type2:
      base: K1ABC
      suffix candidates: [P, 1]
      status: ambiguous

    Type3:
      resolved: K1ABC/P

------------------------------------------------------------------------

## Bit-Level Note (Simplified)

WSPR uses fixed-width bit fields.

Compound callsigns reuse encoding space:

- No explicit discriminator bit
- Interpretation depends on context

This is a deliberate compression tradeoff.

------------------------------------------------------------------------

## Practical Implications

- Weak signals may lose one of the two messages
- Partial decodes are expected
- Robust decoders must tolerate ambiguity

------------------------------------------------------------------------

## Summary

- Slash callsigns require two transmissions
- Single-character suffix creates ambiguity
- Type 3 resolves ambiguity via hashing
- Both messages are required for correctness

This behavior is fundamental to WSPR design.
