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
    std::string expected_primary_extra;
    bool expect_ambiguity;
    std::string expected_alternate_extra;
};

int main()
{
    const std::vector<Type2Case> cases = {
        // One-character suffix
        {"K1ABC/0", "FN20", 30, "/0", false, ""},
        {"K1ABC/7", "FN20", 30, "/7", false, ""},
        {"K1ABC/P", "FN20", 30, "/P", false, ""},
        {"K1ABC/Z", "FN20", 30, "/09", true, "/Z"},

        // Two-digit numeric suffix
        {"K1ABC/00", "FN20", 30, "/00", true, "/Q"},
        {"K1ABC/12", "FN20", 30, "/12", false, ""},
        {"K1ABC/59", "FN20", 30, "/59", false, ""},
        {"K1ABC/99", "FN20", 30, "/99", false, ""},

        // Prefix
        {"W1/K1ABC", "FN20", 30, "W1/", false, ""},
        {"DL/K1ABC", "FN20", 30, "DL/", false, ""},
        {"F4/K1ABC", "FN20", 30, "F4/", false, ""},
        {"AB1/K1ABC", "FN20", 30, "AB1/", false, ""}};

    wspr::WsprRefEncoder encoder;
    wspr::WsprRefDecoder decoder;
    wspr::WsprRefUnpacker unpacker;

    bool all_pass = true;

    for (const auto& tc : cases)
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

        const bool base_pass =
            msg.valid &&
            msg.type == wspr::WsprMessageType::Type2 &&
            msg.callsign == "<hashed>" &&
            msg.extra == tc.expected_primary_extra &&
            msg.power_dbm == tc.power_dbm &&
            msg.is_partial;

        const bool ambiguity_pass =
            !tc.expect_ambiguity ||
            (msg.has_ambiguity &&
             msg.alternate_extra == tc.expected_alternate_extra);

        const bool unambiguous_pass =
            tc.expect_ambiguity ||
            (!msg.has_ambiguity &&
             msg.alternate_extra.empty());

        const bool pass = base_pass && ambiguity_pass && unambiguous_pass;

        std::string result_label;
        if (pass && tc.expect_ambiguity)
            result_label = "AMBIGUOUS (expected overlap)";
        else if (pass)
            result_label = "PASS";
        else
            result_label = "FAIL";

        std::cout << "Case: " << tc.callsign << "\n";
        std::cout << "  Decoded callsign: " << msg.callsign << "\n";
        std::cout << "  Decoded extra:    " << msg.extra << "\n";

        if (msg.has_ambiguity)
            std::cout << "  Alternate extra:  " << msg.alternate_extra << "\n";

        std::cout << "  Decoded power:    " << msg.power_dbm << "\n";
        std::cout << "  Result:           " << result_label << "\n\n";

        if (!pass)
            all_pass = false;
    }

    std::cout << "Summary: " << (all_pass ? "PASS" : "FAIL") << "\n";
    return all_pass ? 0 : 1;
}
