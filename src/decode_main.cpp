#include "wspr/wspr_ref_decoder.hpp"
#include "wspr/wspr_ref_unpack.hpp"
#include "wspr/wspr_constants.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

namespace
{
    void print_payload_bits(const uint8_t *payload_bits)
    {
        std::cout << "Recovered payload bits:\n";
        for (std::size_t i = 0; i < wspr::WSPR_PAYLOAD_BIT_COUNT; ++i)
            std::cout << static_cast<unsigned>(payload_bits[i]);
        std::cout << "\n";
    }

    void print_type1_message(const wspr::WsprDecodedMessage &message)
    {
        std::cout << "\nDecoded Type 1 message:\n";
        std::cout << "  Callsign: " << message.callsign << "\n";
        std::cout << "  Locator:  " << message.locator << "\n";
        std::cout << "  Power:    " << message.power_dbm << "\n";
    }

    void print_type2_message(const wspr::WsprDecodedMessage &message)
    {
        std::cout << "\nDecoded Type 2 partial message:\n";
        std::cout << "  Callsign: " << message.callsign << "\n";
        std::cout << "  Extra:    " << message.extra << "\n";
        std::cout << "  Power:    " << message.power_dbm << "\n";
        std::cout << "  Partial:  "
                  << (message.is_partial ? "true" : "false") << "\n";

        if (message.has_ambiguity)
        {
            std::cout << "  Ambiguous with: "
                      << message.alternate_extra << "\n";
        }
    }

    void print_type3_message(const wspr::WsprDecodedMessage &message)
    {
        std::cout << "\nDecoded Type 3 partial message:\n";
        std::cout << "  Callsign: " << message.callsign << "\n";
        std::cout << "  Hash:     " << message.callsign_hash << "\n";
        std::cout << "  Locator:  " << message.locator << "\n";
        std::cout << "  Power:    " << message.power_dbm << "\n";
        std::cout << "  Partial:  "
                  << (message.is_partial ? "true" : "false") << "\n";
    }

    void print_quiet_message(const wspr::WsprDecodedMessage &message)
    {
        switch (message.type)
        {
        case wspr::WsprMessageType::Type1:
            std::cout
                << "TYPE1 "
                << message.callsign << " "
                << message.locator << " "
                << message.power_dbm << "\n";
            break;

        case wspr::WsprMessageType::Type2:
            std::cout
                << "TYPE2 "
                << message.callsign << " "
                << message.extra << " "
                << message.power_dbm;

            if (message.has_ambiguity)
                std::cout << " ALT " << message.alternate_extra;

            std::cout << "\n";
            break;

            if (message.has_ambiguity)
                std::cout << " ALT " << message.alternate_extra;

            std::cout << "\n";
            break;

        case wspr::WsprMessageType::Type3:
            std::cout
                << "TYPE3 "
                << message.callsign << " "
                << message.callsign_hash << " "
                << message.locator << " "
                << message.power_dbm << "\n";
            break;

        default:
            std::cout << "UNKNOWN\n";
            break;
        }
    }
} // namespace

int main(int argc, char **argv)
{
    bool quiet = false;
    int argi = 1;

    while (argi < argc)
    {
        const std::string arg = argv[argi];

        if (arg == "--quiet")
        {
            quiet = true;
            ++argi;
            continue;
        }

        break;
    }

    if ((argc - argi) != 1)
    {
        std::cerr << "Usage: wspr-decode [--quiet] <162-symbol-string>\n";
        return 1;
    }

    wspr::WsprRefDecoder decoder;
    wspr::WsprRefUnpacker unpacker;

    uint8_t g_bits[wspr::WSPR_BIT_COUNT] = {};
    uint8_t deinterleaved_bits[wspr::WSPR_BIT_COUNT] = {};
    uint8_t payload_bits[wspr::WSPR_PAYLOAD_BIT_COUNT] = {};

    std::string error;

    if (!decoder.symbols_to_bits(argv[argi], g_bits, error))
    {
        std::cerr << "Decode error: " << error << "\n";
        return 1;
    }

    decoder.deinterleave_bits(g_bits, deinterleaved_bits);

    if (!decoder.decode_payload_bits_from_symbols(argv[argi], payload_bits, error))
    {
        std::cerr << "Payload decode error: " << error << "\n";
        return 1;
    }

    wspr::WsprDecodedMessage message;

    if (unpacker.unpack_type1(
            payload_bits,
            wspr::WSPR_PAYLOAD_BIT_COUNT,
            message))
    {
        if (quiet)
        {
            print_quiet_message(message);
            return 0;
        }

        std::cout << "Recovered g bits:\n";
        for (std::size_t i = 0; i < wspr::WSPR_BIT_COUNT; ++i)
            std::cout << static_cast<unsigned>(g_bits[i]);
        std::cout << "\n";

        std::cout << "Deinterleaved bits:\n";
        for (std::size_t i = 0; i < wspr::WSPR_BIT_COUNT; ++i)
            std::cout << static_cast<unsigned>(deinterleaved_bits[i]);
        std::cout << "\n";

        print_payload_bits(payload_bits);
        print_type1_message(message);
        return 0;
    }

    if (unpacker.unpack_type2(
            payload_bits,
            wspr::WSPR_PAYLOAD_BIT_COUNT,
            message))
    {
        if (quiet)
        {
            print_quiet_message(message);
            return 0;
        }

        std::cout << "Recovered g bits:\n";
        for (std::size_t i = 0; i < wspr::WSPR_BIT_COUNT; ++i)
            std::cout << static_cast<unsigned>(g_bits[i]);
        std::cout << "\n";

        std::cout << "Deinterleaved bits:\n";
        for (std::size_t i = 0; i < wspr::WSPR_BIT_COUNT; ++i)
            std::cout << static_cast<unsigned>(deinterleaved_bits[i]);
        std::cout << "\n";

        print_payload_bits(payload_bits);
        print_type2_message(message);
        return 0;
    }

    if (unpacker.unpack_type3(
            payload_bits,
            wspr::WSPR_PAYLOAD_BIT_COUNT,
            message))
    {
        if (quiet)
        {
            print_quiet_message(message);
            return 0;
        }

        std::cout << "Recovered g bits:\n";
        for (std::size_t i = 0; i < wspr::WSPR_BIT_COUNT; ++i)
            std::cout << static_cast<unsigned>(g_bits[i]);
        std::cout << "\n";

        std::cout << "Deinterleaved bits:\n";
        for (std::size_t i = 0; i < wspr::WSPR_BIT_COUNT; ++i)
            std::cout << static_cast<unsigned>(deinterleaved_bits[i]);
        std::cout << "\n";

        print_payload_bits(payload_bits);
        print_type3_message(message);
        return 0;
    }

    if (!quiet)
        std::cout << "\nNo message type unpack succeeded.\n";

    return 0;
}
