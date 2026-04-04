#include "wspr/wspr_ref_correlator.hpp"

#include <iostream>

int main()
{
    wspr::WsprRefCorrelator correlator;

    wspr::WsprDecodedMessage type2;
    type2.valid = true;
    type2.type = wspr::WsprMessageType::Type2;
    type2.callsign = "<hashed>";
    type2.extra = "/7";
    type2.power_dbm = 30;
    type2.is_partial = true;

    wspr::WsprDecodedMessage type3;
    type3.valid = true;
    type3.type = wspr::WsprMessageType::Type3;
    type3.callsign = "<hashed>";
    type3.callsign_hash = 12345;
    type3.has_hash = true;
    type3.locator = "FN20AB";
    type3.power_dbm = 30;
    type3.is_partial = true;

    correlator.add_message(type2);
    correlator.add_message(type3);

    wspr::WsprDecodedMessage resolved;
    if (!correlator.try_resolve_last(resolved))
    {
        std::cerr << "Correlator failed to resolve pair.\n";
        return 1;
    }

    std::cout << "Resolved message:\n";
    std::cout << "  Callsign: " << resolved.callsign << "\n";
    std::cout << "  Extra:    " << resolved.extra << "\n";
    std::cout << "  Locator:  " << resolved.locator << "\n";
    std::cout << "  Power:    " << resolved.power_dbm << "\n";
    std::cout << "  Hash:     " << resolved.callsign_hash << "\n";
    std::cout << "  Partial:  " << (resolved.is_partial ? "true" : "false") << "\n";

    return 0;
}
