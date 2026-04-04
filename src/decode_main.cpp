#include "wspr/wspr_ref_decoder.hpp"
#include "wspr/wspr_ref_unpack.hpp"
#include "wspr/wspr_constants.hpp"

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
    wspr::WsprRefUnpacker unpacker;

    uint8_t g_bits[wspr::WSPR_BIT_COUNT] = {};
    uint8_t deinterleaved_bits[wspr::WSPR_BIT_COUNT] = {};
    uint8_t payload_bits[wspr::WSPR_PAYLOAD_BIT_COUNT] = {};

    std::string error;

    if (!decoder.symbols_to_bits(argv[1], g_bits, error))
    {
        std::cerr << "Decode error: " << error << "\n";
        return 1;
    }

    decoder.deinterleave_bits(g_bits, deinterleaved_bits);

    if (!decoder.decode_payload_bits_from_symbols(argv[1], payload_bits, error))
    {
        std::cerr << "Payload decode error: " << error << "\n";
        return 1;
    }

    std::cout << "Recovered g bits:\n";
    for (std::size_t i = 0; i < wspr::WSPR_BIT_COUNT; ++i)
        std::cout << static_cast<unsigned>(g_bits[i]);
    std::cout << "\n";

    std::cout << "Deinterleaved bits:\n";
    for (std::size_t i = 0; i < wspr::WSPR_BIT_COUNT; ++i)
        std::cout << static_cast<unsigned>(deinterleaved_bits[i]);
    std::cout << "\n";

    std::cout << "Recovered payload bits:\n";
    for (std::size_t i = 0; i < wspr::WSPR_PAYLOAD_BIT_COUNT; ++i)
        std::cout << static_cast<unsigned>(payload_bits[i]);
    std::cout << "\n";

    wspr::WsprDecodedMessage message;
    if (unpacker.unpack_type1(
            payload_bits,
            wspr::WSPR_PAYLOAD_BIT_COUNT,
            message))
    {
        std::cout << "\nDecoded Type 1 message:\n";
        std::cout << "  Callsign: " << message.callsign << "\n";
        std::cout << "  Locator:  " << message.locator << "\n";
        std::cout << "  Power:    " << message.power_dbm << "\n";
    }
    else
    {
        std::cout << "\nType 1 unpack failed: " << message.error << "\n";
    }

    return 0;
}
