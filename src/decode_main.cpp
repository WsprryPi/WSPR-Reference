#include "wspr/wspr_ref_decoder.hpp"
#include "wspr/wspr_ref_encoder.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: wspr-decode <162-symbol-string>\n";
        return 1;
    }

    wspr::WsprRefDecoder decoder;
    uint8_t g_bits[wspr::WSPR_BIT_COUNT] = {};
    uint8_t deinterleaved_bits[wspr::WSPR_BIT_COUNT] = {};
    std::string error;

    if (!decoder.symbols_to_bits(argv[1], g_bits, error))
    {
        std::cerr << "Decode error: " << error << "\n";
        return 1;
    }

    decoder.deinterleave_bits(g_bits, deinterleaved_bits);

    std::cout << "Recovered g bits:\n";
    for (std::size_t i = 0; i < wspr::WSPR_BIT_COUNT; ++i)
        std::cout << static_cast<unsigned>(g_bits[i]);
    std::cout << "\n";

    std::cout << "Deinterleaved bits:\n";
    for (std::size_t i = 0; i < wspr::WSPR_BIT_COUNT; ++i)
        std::cout << static_cast<unsigned>(deinterleaved_bits[i]);
    std::cout << "\n";

    return 0;
}
