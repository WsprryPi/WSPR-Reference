#ifndef WSPR_REF_API_HPP
#define WSPR_REF_API_HPP

#include "wspr_ref_unpack.hpp"

#include <string>

namespace wspr
{
struct WsprEncodeResult
{
    bool ok = false;
    std::string type;
    std::string callsign;
    std::string locator;
    int power_dbm = 0;
    std::string symbols;
    std::string error;
};

struct WsprCorrelateResult
{
    bool ok = false;
    bool correlated = false;
    WsprDecodedMessage message1;
    WsprDecodedMessage message2;
    WsprDecodedMessage resolved;
    std::string error;
};

WsprEncodeResult encode_message(
    const std::string& callsign,
    const std::string& locator,
    int power_dbm);

bool decode_symbols(
    const std::string& symbols,
    WsprDecodedMessage& message,
    std::string& error);

bool correlate_messages(
    const WsprDecodedMessage& a,
    const WsprDecodedMessage& b,
    WsprDecodedMessage& resolved,
    std::string& error);

WsprCorrelateResult correlate_symbol_streams(
    const std::string& symbols1,
    const std::string& symbols2);
} // namespace wspr

#endif // WSPR_REF_API_HPP
