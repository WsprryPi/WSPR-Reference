#include "wspr/wspr_ref_encoder.hpp"
#include "wspr/wspr_ref_decoder.hpp"
#include "wspr/wspr_ref_unpack.hpp"
#include "wspr/wspr_constants.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

struct Type2Case
{
    std::string callsign;
    std::string locator;
    int8_t power_dbm;
    std::string expected_extra;
};

int main()
{
    const std::vector<Type2Case> cases = {
        // One-character suffix
        {"K1ABC/0", "FN20", 30, "/0"},
        {"K1ABC/7", "FN20", 30, "/7"},
        {"K1ABC/P", "FN20", 30, "/P"},
        {"K1ABC/Z", "FN20", 30, "/Z"},

        // Two-digit numeric suffix
        {"K1ABC/00", "FN20", 30, "/00"},
        {"K1ABC/12", "FN20", 30, "/12"},
        {"K1ABC/59", "FN20", 30, "/59"},
        {"K1ABC/99", "FN20", 30, "/99"},

        // Prefix
        {"W1/K1ABC", "FN20", 30, "W1/"},
        {"DL/K1ABC", "FN20", 30, "DL/"},
        {"F4/K1ABC", "FN20", 30, "F4/"},
        {"AB1/K1ABC", "FN20", 30, "AB1/"}};

    wspr::WsprRefEncoder encoder;
    wspr::WsprRefDecoder decoder;
    wspr::WsprRefUnpacker unpacker;

    bool all_pass = true;

    for (const auto &tc : cases)
    {
        uint8_t symbols[wspr::WSPR_SYMBOL_COUNT] = {};
        uint8_t payload_bits[wspr::WSPR_PAYLOAD_BIT_COUNT] = {};

        encoder.wspr_encode(
            tc.callsign.c_str(),
            tc.locator.c_str(),
            tc.power_dbm,
            symbols);

        std::string symbol_text;
        symbol_text.reserve(wspr::WSPR_SYMBOL_COUNT);

        for (std::size_t i = 0; i < wspr::WSPR_SYMBOL_COUNT; ++i)
            symbol_text.push_back(static_cast<char>('0' + symbols[i]));

        std::string error;
        if (!decoder.decode_payload_bits_from_symbols(
                symbol_text,
                payload_bits,
                error))
        {
            std::cerr << "Payload decode failed for " << tc.callsign
                      << ": " << error << "\n";
            return 1;
        }

        wspr::WsprDecodedMessage msg;
        if (!unpacker.unpack_type2(
                payload_bits,
                wspr::WSPR_PAYLOAD_BIT_COUNT,
                msg))
        {
            std::cerr << "Type 2 unpack failed for " << tc.callsign
                      << ": " << msg.error << "\n";
            return 1;
        }

        const bool pass =
            msg.valid &&
            msg.type == wspr::WsprMessageType::Type2 &&
            msg.callsign == "<hashed>" &&
            msg.extra == tc.expected_extra &&
            msg.power_dbm == tc.power_dbm &&
            msg.is_partial;

        std::cout << "Case: " << tc.callsign << "\n";
        std::cout << "  Decoded callsign: " << msg.callsign << "\n";
        std::cout << "  Decoded extra:    " << msg.extra << "\n";
        std::cout << "  Decoded power:    " << msg.power_dbm << "\n";
        std::cout << "  Result:           " << (pass ? "PASS" : "FAIL") << "\n\n";

        if (!pass)
            all_pass = false;
    }

    std::cout << "Summary: " << (all_pass ? "PASS" : "FAIL") << "\n";
    return all_pass ? 0 : 1;
}
