#include "wspr/wspr_ref_encoder.hpp"
#include "wspr/wspr_constants.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>
#include <string>

namespace
{
    bool validate_symbol_stream(const uint8_t *symbols, std::size_t count)
    {
        if (symbols == nullptr)
            return false;

        for (std::size_t i = 0; i < count; ++i)
        {
            if (symbols[i] > 3U)
                return false;
        }

        return true;
    }

    std::string detect_type(const std::string &callsign, const std::string &locator)
    {
        if (!callsign.empty() && callsign.front() == '<' && callsign.back() == '>')
            return "TYPE3";

        if (callsign.find('/') != std::string::npos)
            return "TYPE2";

        if (locator.size() == 6)
            return "TYPE3";

        return "TYPE1";
    }

    std::string symbols_to_string(const uint8_t *symbols, std::size_t count)
    {
        std::string out;
        out.reserve(count);

        for (std::size_t i = 0; i < count; ++i)
            out.push_back(static_cast<char>('0' + symbols[i]));

        return out;
    }
} // namespace

int main(int argc, char **argv)
{
    bool quiet = false;
    int argi = 1;

    while (argi < argc)
    {
        const std::string arg = argv[argi];

        if (arg == "--quiet" || arg == "--symbols-only")
        {
            quiet = true;
            ++argi;
            continue;
        }

        break;
    }

    if ((argc - argi) != 3)
    {
        std::cerr
            << "Usage: wspr-encode [--quiet|--symbols-only] "
            << "<callsign> <locator> <power_dbm>\n";
        return 1;
    }

    const std::string callsign = argv[argi];
    const std::string locator = argv[argi + 1];

    int power_dbm = 0;
    try
    {
        power_dbm = std::stoi(argv[argi + 2]);
    }
    catch (const std::exception &)
    {
        std::cerr << "Invalid power value.\n";
        return 1;
    }

    wspr::WsprRefEncoder encoder;

    uint8_t symbols[wspr::WSPR_SYMBOL_COUNT] = {};
    std::memset(symbols, 0, sizeof(symbols));

    encoder.wspr_encode(
        callsign.c_str(),
        locator.c_str(),
        static_cast<int8_t>(power_dbm),
        symbols);

    if (!validate_symbol_stream(symbols, wspr::WSPR_SYMBOL_COUNT))
    {
        std::cerr << "Generated symbol stream is invalid.\n";
        return 1;
    }

    const std::string symbol_text =
        symbols_to_string(symbols, wspr::WSPR_SYMBOL_COUNT);

    if (quiet)
    {
        std::cout << symbol_text << "\n";
        return 0;
    }

    std::cout << "Type: " << detect_type(callsign, locator) << "\n";
    std::cout << "Callsign: " << callsign << "\n";
    std::cout << "Locator: " << locator << "\n";
    std::cout << "Power: " << power_dbm << " dBm\n";
    std::cout << "Symbols: " << symbol_text << "\n";

    return 0;
}
