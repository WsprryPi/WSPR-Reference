#include "wspr/wspr_ref_api.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace
{
    struct DecodeCase
    {
        std::string callsign;
        std::string locator;
        int power_dbm = 0;
        std::string expected_callsign;
        std::string expected_extra;
    };
}

int main()
{
    const std::vector<DecodeCase> cases = {
        {"AA0NT/12", "EM18", 20, "<hashed>/12", "/12"},
        {"W0/AA0NT", "EM18", 20, "W0/<hashed>", "W0/"}};

    bool all_pass = true;

    for (const auto &tc : cases)
    {
        const wspr::WsprEncodeResult encoded =
            wspr::encode_message(tc.callsign, tc.locator, tc.power_dbm);

        if (!encoded.ok)
        {
            std::cerr << "Encode failed for " << tc.callsign
                      << ": " << encoded.error << "\n";
            return 1;
        }

        wspr::WsprDecodedMessage decoded;
        std::string error;
        if (!wspr::decode_symbols(encoded.symbols, decoded, error))
        {
            std::cerr << "Decode failed for " << tc.callsign
                      << ": " << error << "\n";
            return 1;
        }

        const bool pass =
            decoded.valid &&
            decoded.type == wspr::WsprMessageType::Type2 &&
            decoded.callsign == tc.expected_callsign &&
            decoded.extra == tc.expected_extra &&
            decoded.power_dbm == tc.power_dbm &&
            decoded.is_partial;

        std::cout << "Decode case: " << tc.callsign << "\n";
        std::cout << "  Callsign: " << decoded.callsign << "\n";
        std::cout << "  Extra:    " << decoded.extra << "\n";
        std::cout << "  Result:   " << (pass ? "PASS" : "FAIL") << "\n\n";

        if (!pass)
            all_pass = false;
    }

    const wspr::WsprEncodeResult type2 =
        wspr::encode_message("AA0NT/12", "EM18", 20);
    const wspr::WsprEncodeResult type3 =
        wspr::encode_message("<AA0NT>", "EM18IG", 20);

    if (!type2.ok || !type3.ok)
    {
        std::cerr << "Failed to encode correlation inputs.\n";
        return 1;
    }

    const wspr::WsprCorrelateResult correlated =
        wspr::correlate_symbol_streams(type2.symbols, type3.symbols);

    const bool correlate_pass =
        correlated.ok &&
        correlated.correlated &&
        correlated.message1.callsign == "<hashed>/12" &&
        correlated.message1.extra == "/12" &&
        correlated.resolved.callsign == "<callsign>/12" &&
        correlated.resolved.locator == "EM18IG" &&
        correlated.resolved.power_dbm == 20 &&
        correlated.resolved.has_hash &&
        !correlated.resolved.is_partial;

    std::cout << "Correlate case:\n";
    std::cout << "  Decoded Type 2 callsign: " << correlated.message1.callsign << "\n";
    std::cout << "  Resolved callsign:       " << correlated.resolved.callsign << "\n";
    std::cout << "  Result:                  " << (correlate_pass ? "PASS" : "FAIL") << "\n\n";

    all_pass = all_pass && correlate_pass;

    std::cout << "Summary: " << (all_pass ? "PASS" : "FAIL") << "\n";
    return all_pass ? 0 : 1;
}
