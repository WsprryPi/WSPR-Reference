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

        std::string extra;

        uint32_t callsign_hash = 0;
        bool has_hash = false;

        bool is_partial = false;
        std::string error;
    };

    class WsprRefUnpacker
    {
    public:
        bool unpack_type1(
            const uint8_t *payload_bits,
            std::size_t payload_bit_count,
            WsprDecodedMessage &message) const;

        bool unpack_type2(
            const uint8_t *payload_bits,
            std::size_t payload_bit_count,
            WsprDecodedMessage &message) const;

        bool unpack_type3(
            const uint8_t *payload_bits,
            std::size_t payload_bit_count,
            WsprDecodedMessage &message) const;

    private:
        bool decode_type2_power_and_offset(
            uint32_t low_bits,
            int &power_dbm,
            uint32_t &offset) const;

        std::string decode_type2_suffix_one_char(
            uint32_t value) const;

        std::string decode_type2_suffix_two_digit(
            uint32_t value) const;

        std::string decode_type2_prefix(
            uint32_t value) const;

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

        uint32_t extract_type3_locator_value(
            const uint8_t *payload_bits,
            std::size_t payload_bit_count) const;

        uint32_t extract_type3_hash_value(
            const uint8_t *payload_bits,
            std::size_t payload_bit_count) const;

        bool unpack_locator_type3(
            uint32_t locator_value,
            std::string &locator) const;
    };
} // namespace wspr

#endif // WSPR_REF_UNPACK_HPP
