#ifndef WSPR_REF_UNPACK_HPP
#define WSPR_REF_UNPACK_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace wspr
{
    enum class WsprMessageType
    {
        Unknown = 0,
        Type1,
        Type2,
        Type3
    };

    struct WsprDecodedMessage
    {
        bool valid = false;
        WsprMessageType type = WsprMessageType::Unknown;

        std::string callsign;
        std::string locator;
        int power_dbm = 0;

        std::string error;
    };

    class WsprRefUnpacker
    {
    public:
        bool unpack_type1(
            const uint8_t *payload_bits,
            std::size_t payload_bit_count,
            WsprDecodedMessage &message) const;

    private:
        uint32_t extract_n(
            const uint8_t *payload_bits,
            std::size_t payload_bit_count) const;

        uint32_t extract_m(
            const uint8_t *payload_bits,
            std::size_t payload_bit_count) const;

        void unpack_callsign_type1(
            uint32_t n,
            std::string &callsign) const;

        bool unpack_locator_power_type1(
            uint32_t m,
            std::string &locator,
            int &power_dbm) const;
    };
} // namespace wspr

#endif  // WSPR_REF_UNPACK_HPP
