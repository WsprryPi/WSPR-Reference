#include "wspr/wspr_ref_correlator.hpp"
#include "wspr/wspr_ref_decoder.hpp"
#include "wspr/wspr_ref_unpack.hpp"
#include "wspr/wspr_constants.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

namespace
{
    bool decode_one(
        const std::string &symbol_text,
        wspr::WsprRefDecoder &decoder,
        wspr::WsprRefUnpacker &unpacker,
        wspr::WsprDecodedMessage &message,
        std::string &error)
    {
        uint8_t payload_bits[wspr::WSPR_PAYLOAD_BIT_COUNT] = {};

        if (!decoder.decode_payload_bits_from_symbols(
                symbol_text,
                payload_bits,
                error))
        {
            return false;
        }

        if (unpacker.unpack_type1(
                payload_bits,
                wspr::WSPR_PAYLOAD_BIT_COUNT,
                message))
        {
            return true;
        }

        if (unpacker.unpack_type2(
                payload_bits,
                wspr::WSPR_PAYLOAD_BIT_COUNT,
                message))
        {
            return true;
        }

        if (unpacker.unpack_type3(
                payload_bits,
                wspr::WSPR_PAYLOAD_BIT_COUNT,
                message))
        {
            return true;
        }

        error = "No unpacker recognized the payload.";
        return false;
    }

    void print_message(const wspr::WsprDecodedMessage &message)
    {
        std::cout << "  Type:     ";

        switch (message.type)
        {
        case wspr::WsprMessageType::Type1:
            std::cout << "Type1\n";
            break;
        case wspr::WsprMessageType::Type2:
            std::cout << "Type2\n";
            break;
        case wspr::WsprMessageType::Type3:
            std::cout << "Type3\n";
            break;
        default:
            std::cout << "Unknown\n";
            break;
        }

        std::cout << "  Callsign: " << message.callsign << "\n";

        if (message.has_ambiguity)
            std::cout << "  Ambiguous with: "
                      << message.alternate_extra << "\n";

        if (message.has_hash)
            std::cout << "  Hash:     " << message.callsign_hash << "\n";

        if (!message.locator.empty())
            std::cout << "  Locator:  " << message.locator << "\n";

        std::cout << "  Power:    " << message.power_dbm << "\n";
        std::cout << "  Partial:  " << (message.is_partial ? "true" : "false") << "\n";
        std::cout << "\n";
    }

    void print_quiet_correlated(const wspr::WsprDecodedMessage &message)
    {
        std::cout << "CORRELATED ";

        switch (message.type)
        {
        case wspr::WsprMessageType::Type1:
            std::cout << "TYPE1 ";
            break;
        case wspr::WsprMessageType::Type2:
            std::cout << "TYPE2 ";
            break;
        case wspr::WsprMessageType::Type3:
            std::cout << "TYPE3 ";
            break;
        default:
            std::cout << "UNKNOWN ";
            break;
        }

        std::cout << message.callsign << " ";

        if (message.has_hash)
            std::cout << message.callsign_hash << " ";
        else
            std::cout << "- ";

        if (!message.locator.empty())
            std::cout << message.locator << " ";
        else
            std::cout << "- ";

        std::cout << message.power_dbm;

        if (message.has_ambiguity)
            std::cout << " ALT " << message.alternate_extra;

        std::cout << "\n";
    }

    void print_quiet_uncorrelated(
        const std::string &prefix,
        const wspr::WsprDecodedMessage &message)
    {
        std::cout << prefix << " ";

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
                << message.power_dbm << "\n";
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

    if ((argc - argi) != 2)
    {
        std::cerr << "Usage: wspr-correlate [--quiet] <symbols1> <symbols2>\n";
        return 1;
    }

    const std::string symbols1 = argv[argi];
    const std::string symbols2 = argv[argi + 1];

    wspr::WsprRefDecoder decoder;
    wspr::WsprRefUnpacker unpacker;
    wspr::WsprRefCorrelator correlator;

    wspr::WsprDecodedMessage msg1;
    wspr::WsprDecodedMessage msg2;
    std::string error;

    if (!decode_one(symbols1, decoder, unpacker, msg1, error))
    {
        std::cerr << "Failed to decode first message: " << error << "\n";
        return 1;
    }

    if (!decode_one(symbols2, decoder, unpacker, msg2, error))
    {
        std::cerr << "Failed to decode second message: " << error << "\n";
        return 1;
    }

    correlator.add_message(msg1);
    correlator.add_message(msg2);

    wspr::WsprDecodedMessage resolved;
    const bool correlated = correlator.try_resolve_last(resolved);

    if (quiet)
    {
        if (correlated)
        {
            print_quiet_correlated(resolved);
            return 0;
        }

        print_quiet_uncorrelated("UNCORRELATED1", msg1);
        print_quiet_uncorrelated("UNCORRELATED2", msg2);
        return 0;
    }

    std::cout << "Decoded input 1\n";
    std::cout << "===============\n";
    print_message(msg1);

    std::cout << "Decoded input 2\n";
    std::cout << "===============\n";
    print_message(msg2);

    if (correlated)
    {
        std::cout << "Correlated result\n";
        std::cout << "=================\n";
        print_message(resolved);
        return 0;
    }

    std::cout << "No correlated result available.\n";
    return 0;
}
