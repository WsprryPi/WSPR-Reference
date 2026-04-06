#include "wspr/wspr_ref_api.hpp"

#include <iostream>
#include <string>

int main()
{
    const auto encoded = wspr::encode_message("AA0NT", "EM18", 20);
    if (!encoded.ok)
    {
        std::cerr << encoded.error << "\n";
        return 1;
    }

    std::cout << "Encoded symbols: " << encoded.symbols << "\n";

    wspr::WsprDecodedMessage decoded;
    std::string error;

    if (!wspr::decode_symbols(encoded.symbols, decoded, error))
    {
        std::cerr << error << "\n";
        return 1;
    }

    std::cout << "Decoded callsign: " << decoded.callsign << "\n";

    return 0;
}
