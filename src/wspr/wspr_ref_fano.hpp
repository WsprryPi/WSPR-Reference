#ifndef WSPR_REF_FANO_HPP
#define WSPR_REF_FANO_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace wspr
{
    class WsprRefFanoDecoder
    {
    public:
        bool decode_hard_bits(
            const uint8_t *coded_bits,
            std::size_t coded_bit_count,
            uint8_t *decoded_bits,
            std::size_t decoded_bit_count,
            std::string &error) const;

        /*
         * Bounded-depth prototype used to validate path exploration before the
         * full Fano sequential search is implemented.
         *
         * This explores all paths up to input_bit_limit and returns the best path
         * by cumulative hard branch metric. It is intentionally only for short
         * depths such as 8 or 16 bits.
         */
        bool decode_hard_bits_bounded(
            const uint8_t *coded_bits,
            std::size_t coded_bit_count,
            uint8_t *decoded_bits,
            std::size_t decoded_bit_count,
            std::size_t input_bit_limit,
            std::string &error) const;

        bool decode_hard_bits_fano_limited(
            const uint8_t *coded_bits,
            std::size_t coded_bit_count,
            uint8_t *decoded_bits,
            std::size_t decoded_bit_count,
            std::size_t input_bit_limit,
            std::string &error) const;

        void debug_expected_parity(
            uint32_t shift_register,
            uint8_t input_bit,
            uint8_t &p0,
            uint8_t &p1) const;

        int debug_branch_metric_hard(
            uint8_t expected_p0,
            uint8_t expected_p1,
            uint8_t observed_p0,
            uint8_t observed_p1) const;

    private:
        struct FanoNode
        {
            uint32_t shift_register = 0;
            int cumulative_metric = 0;
            uint8_t chosen_bit = 0;
            uint8_t next_branch_to_try = 0;
        };

        void expected_parity(
            uint32_t shift_register,
            uint8_t input_bit,
            uint8_t &p0,
            uint8_t &p1) const;

        int branch_metric_hard(
            uint8_t expected_p0,
            uint8_t expected_p1,
            uint8_t observed_p0,
            uint8_t observed_p1) const;
    };
} // namespace wspr

#endif
