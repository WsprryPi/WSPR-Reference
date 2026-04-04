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

#endif // WSPR_REF_FANO_HPP
