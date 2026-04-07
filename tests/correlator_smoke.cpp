#include "wspr/wspr_ref_correlator.hpp"

#include <iostream>
#include <string>

namespace
{
    bool run_case(
        const std::string &label,
        const std::string &extra,
        const std::string &expected_callsign)
    {
        wspr::WsprRefCorrelator correlator;

        wspr::WsprDecodedMessage type2;
        type2.valid = true;
        type2.type = wspr::WsprMessageType::Type2;
        type2.callsign = "<hashed>";
        type2.extra = extra;
        type2.power_dbm = 20;
        type2.is_partial = true;

        wspr::WsprDecodedMessage type3;
        type3.valid = true;
        type3.type = wspr::WsprMessageType::Type3;
        type3.callsign = "<hashed>";
        type3.callsign_hash = 11595;
        type3.has_hash = true;
        type3.locator = "EM18IG";
        type3.power_dbm = 20;
        type3.is_partial = true;

        correlator.add_message(type2);
        correlator.add_message(type3);

        wspr::WsprDecodedMessage resolved;
        if (!correlator.try_resolve_last(resolved))
        {
            std::cerr << label << ": Correlator failed to resolve pair.\n";
            return false;
        }

        const bool pass =
            resolved.valid &&
            resolved.callsign == expected_callsign &&
            resolved.extra == extra &&
            resolved.locator == "EM18IG" &&
            resolved.power_dbm == 20 &&
            resolved.callsign_hash == 11595 &&
            resolved.has_hash &&
            !resolved.is_partial;

        std::cout << label << ":\n";
        std::cout << "  Callsign: " << resolved.callsign << "\n";
        std::cout << "  Extra:    " << resolved.extra << "\n";
        std::cout << "  Locator:  " << resolved.locator << "\n";
        std::cout << "  Power:    " << resolved.power_dbm << "\n";
        std::cout << "  Hash:     " << resolved.callsign_hash << "\n";
        std::cout << "  Partial:  " << (resolved.is_partial ? "true" : "false") << "\n";
        std::cout << "  Result:   " << (pass ? "PASS" : "FAIL") << "\n\n";

        return pass;
    }
} // namespace

int main()
{
    bool all_pass = true;

    all_pass &= run_case(
        "Suffix case",
        "/12",
        "<callsign>/12");

    all_pass &= run_case(
        "Prefix case",
        "W0/",
        "W0/<callsign>");

    std::cout << "Summary: " << (all_pass ? "PASS" : "FAIL") << "\n";
    return all_pass ? 0 : 1;
}
