#include "wspr_ref_fano.hpp"
#include "wspr_constants.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace wspr
{
    void WsprRefFanoDecoder::expected_parity(
        uint32_t shift_register,
        uint8_t input_bit,
        uint8_t &p0,
        uint8_t &p1) const
    {
        uint32_t reg0 = (shift_register << 1) | static_cast<uint32_t>(input_bit);
        uint32_t reg1 = reg0;

        uint32_t t0 = reg0 & WSPR_POLY_0;
        uint32_t t1 = reg1 & WSPR_POLY_1;

        p0 = 0;
        p1 = 0;

        for (uint8_t k = 0; k < 32; ++k)
        {
            p0 ^= static_cast<uint8_t>(t0 & 0x01U);
            p1 ^= static_cast<uint8_t>(t1 & 0x01U);
            t0 >>= 1;
            t1 >>= 1;
        }
    }

    int WsprRefFanoDecoder::branch_metric_hard(
        uint8_t expected_p0,
        uint8_t expected_p1,
        uint8_t observed_p0,
        uint8_t observed_p1) const
    {
        int metric = 0;
        metric += (expected_p0 == observed_p0) ? 1 : -1;
        metric += (expected_p1 == observed_p1) ? 1 : -1;
        return metric;
    }

    bool WsprRefFanoDecoder::decode_hard_bits(
        const uint8_t *coded_bits,
        std::size_t coded_bit_count,
        uint8_t *decoded_bits,
        std::size_t decoded_bit_count,
        std::string &error) const
    {
        error.clear();

        if (coded_bits == nullptr || decoded_bits == nullptr)
        {
            error = "Null input to decode_hard_bits().";
            return false;
        }

        if (coded_bit_count != WSPR_BIT_COUNT)
        {
            error = "coded_bit_count must equal WSPR_BIT_COUNT.";
            return false;
        }

        if (decoded_bit_count != WSPR_PAYLOAD_BIT_COUNT)
        {
            error = "decoded_bit_count must equal WSPR_PAYLOAD_BIT_COUNT.";
            return false;
        }

        /*
         * This is a clean-room Stage 3 starter, not a finished Fano decoder.
         *
         * The helpers below are correct building blocks:
         * - expected_parity()
         * - branch_metric_hard()
         *
         * The full Fano search is intentionally not faked here. Returning a
         * plausible-looking decoded bitstream would be misleading.
         */

        for (std::size_t i = 0; i < decoded_bit_count; ++i)
            decoded_bits[i] = 0;

        /*
         * Minimal sanity exercise of the branch metric path so the implementation
         * stays compile-tested while the real search is added.
         */
        uint32_t test_shift_register = 0;
        uint8_t expected_p0 = 0;
        uint8_t expected_p1 = 0;

        expected_parity(test_shift_register, 0, expected_p0, expected_p1);

        const uint8_t observed_p0 = coded_bits[0];
        const uint8_t observed_p1 = coded_bits[1];
        const int metric = branch_metric_hard(
            expected_p0,
            expected_p1,
            observed_p0,
            observed_p1);

        (void)metric;

        error =
            "Fano sequential decoder search is not implemented yet. "
            "Stage 3 scaffolding is in place, but this function currently stops "
            "before attempting path exploration.";
        return false;
    }
} // namespace wspr
