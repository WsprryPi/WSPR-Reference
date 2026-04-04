#include "wspr_ref_fano.hpp"
#include "wspr_constants.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <string>
#include <vector>

namespace wspr
{
void WsprRefFanoDecoder::expected_parity(
    uint32_t shift_register,
    uint8_t input_bit,
    uint8_t& p0,
    uint8_t& p1) const
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

void WsprRefFanoDecoder::debug_expected_parity(
    uint32_t shift_register,
    uint8_t input_bit,
    uint8_t& p0,
    uint8_t& p1) const
{
    expected_parity(shift_register, input_bit, p0, p1);
}

int WsprRefFanoDecoder::debug_branch_metric_hard(
    uint8_t expected_p0,
    uint8_t expected_p1,
    uint8_t observed_p0,
    uint8_t observed_p1) const
{
    return branch_metric_hard(expected_p0, expected_p1, observed_p0, observed_p1);
}

bool WsprRefFanoDecoder::decode_hard_bits(
    const uint8_t* coded_bits,
    std::size_t coded_bit_count,
    uint8_t* decoded_bits,
    std::size_t decoded_bit_count,
    std::string& error) const
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

    for (std::size_t i = 0; i < decoded_bit_count; ++i)
        decoded_bits[i] = 0;

    error =
        "Full Fano sequential decoder search is not implemented yet. "
        "Use decode_hard_bits_bounded() for short bounded-depth validation.";
    return false;
}

bool WsprRefFanoDecoder::decode_hard_bits_bounded(
    const uint8_t* coded_bits,
    std::size_t coded_bit_count,
    uint8_t* decoded_bits,
    std::size_t decoded_bit_count,
    std::size_t input_bit_limit,
    std::string& error) const
{
    error.clear();

    if (coded_bits == nullptr || decoded_bits == nullptr)
    {
        error = "Null input to decode_hard_bits_bounded().";
        return false;
    }

    if (input_bit_limit == 0)
    {
        error = "input_bit_limit must be greater than zero.";
        return false;
    }

    if (input_bit_limit > decoded_bit_count)
    {
        error = "input_bit_limit exceeds decoded_bit_count.";
        return false;
    }

    if ((input_bit_limit * 2U) > coded_bit_count)
    {
        error = "Not enough coded bits for requested bounded decode.";
        return false;
    }

    if (input_bit_limit > 24)
    {
        error =
            "input_bit_limit is too large for bounded exhaustive search. "
            "Keep it at 24 bits or less.";
        return false;
    }

    std::vector<uint8_t> current_path(input_bit_limit, 0);
    std::vector<uint8_t> best_path(input_bit_limit, 0);

    int best_metric = std::numeric_limits<int>::min();

    std::function<void(std::size_t, uint32_t, int)> dfs =
        [&](std::size_t depth, uint32_t shift_register, int cumulative_metric)
    {
        if (depth == input_bit_limit)
        {
            if (cumulative_metric > best_metric)
            {
                best_metric = cumulative_metric;
                best_path = current_path;
            }
            return;
        }

        const uint8_t observed_p0 = coded_bits[2 * depth];
        const uint8_t observed_p1 = coded_bits[(2 * depth) + 1];

        for (uint8_t candidate_bit = 0; candidate_bit <= 1; ++candidate_bit)
        {
            uint8_t expected_p0 = 0;
            uint8_t expected_p1 = 0;

            expected_parity(
                shift_register,
                candidate_bit,
                expected_p0,
                expected_p1);

            const int metric = branch_metric_hard(
                expected_p0,
                expected_p1,
                observed_p0,
                observed_p1);

            current_path[depth] = candidate_bit;

            const uint32_t next_shift_register =
                (shift_register << 1) | static_cast<uint32_t>(candidate_bit);

            dfs(
                depth + 1,
                next_shift_register,
                cumulative_metric + metric);
        }
    };

    dfs(0, 0, 0);

    if (best_metric == std::numeric_limits<int>::min())
    {
        error = "No path found in bounded search.";
        return false;
    }

    for (std::size_t i = 0; i < decoded_bit_count; ++i)
        decoded_bits[i] = 0;

    for (std::size_t i = 0; i < input_bit_limit; ++i)
        decoded_bits[i] = best_path[i];

    return true;
}
} // namespace wspr
