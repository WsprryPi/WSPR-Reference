#ifndef WSPR_REF_DECODER_HPP
#define WSPR_REF_DECODER_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace wspr
{
    class WsprRefDecoder
    {
    public:
        bool symbols_to_bits(
            const std::string &symbol_text,
            uint8_t *g_bits,
            std::string &error) const;

        void deinterleave_bits(
            const uint8_t *interleaved_bits,
            uint8_t *deinterleaved_bits) const;

        bool decode_payload_bits_from_symbols(
            const std::string &symbol_text,
            uint8_t *payload_bits,
            std::string &error) const;

    private:
        bool parse_symbols(
            const std::string &symbol_text,
            uint8_t *symbols,
            std::string &error) const;
    };
} // namespace wspr

#endif // WSPR_REF_DECODER_HPP
