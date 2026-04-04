#include "wspr/wspr_ref_encoder.hpp"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

namespace
{
    const char *infer_wspr_type(const std::string &callsign)
    {
        if (!callsign.empty() && callsign.front() == '<')
            return "TYPE3";

        if (callsign.find('/') != std::string::npos)
            return "TYPE2";

        return "TYPE1";
    }

    bool validate_symbol_stream(const uint8_t *symbols, std::size_t count)
    {
        for (std::size_t i = 0; i < count; ++i)
        {
            if (symbols[i] > 3)
                return false;
        }

        return true;
    }
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr
            << "Usage: jtencode-wspr <callsign> <locator> <power_dbm>\n"
            << "\n"
            << "Examples:\n"
            << "  jtencode-wspr K1ABC FN20 30\n"
            << "  jtencode-wspr K1ABC/7 FN20 30\n"
            << "  jtencode-wspr K1ABC/12 FN20 30\n"
            << "  jtencode-wspr W1/K1ABC FN20 30\n"
            << "  jtencode-wspr \"<K1ABC>\" FN20AB 30\n";
        return 1;
    }

    const std::string callsign = argv[1];
    const std::string locator = argv[2];
    const int power = std::stoi(argv[3]);

    uint8_t symbols[wspr::WSPR_SYMBOL_COUNT];
    std::memset(symbols, 0, sizeof(symbols));

    wspr::WsprRefEncoder encoder;
    encoder.wspr_encode(callsign.c_str(), locator.c_str(), power, symbols);

    if (!validate_symbol_stream(symbols, wspr::WSPR_SYMBOL_COUNT))
    {
        std::cerr << "Error: encoder returned invalid WSPR symbols.\n";
        return 1;
    }

    std::cout << "Type: " << infer_wspr_type(callsign) << "\n";
    std::cout << "Callsign: " << callsign << "\n";
    std::cout << "Locator: " << locator << "\n";
    std::cout << "Power: " << power << " dBm\n";
    std::cout << "Symbols: ";

    for (std::size_t i = 0; i < wspr::WSPR_SYMBOL_COUNT; ++i)
        std::cout << static_cast<unsigned>(symbols[i]);

    std::cout << "\n";

    return 0;
}
