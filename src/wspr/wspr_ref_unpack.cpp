#include "wspr_ref_unpack.hpp"
#include "wspr_constants.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

namespace wspr
{
    namespace
    {
        char decode_wspr_base37(uint32_t value)
        {
            if (value <= 9)
                return static_cast<char>('0' + value);

            if (value >= 10 && value <= 35)
                return static_cast<char>('A' + (value - 10));

            return ' ';
        }
    } // namespace

    uint32_t WsprRefUnpacker::extract_n(
        const uint8_t *payload_bits,
        std::size_t payload_bit_count) const
    {
        (void)payload_bit_count;

        uint32_t n = 0;

        for (std::size_t i = 0; i < 28; ++i)
        {
            n = (n << 1) | static_cast<uint32_t>(payload_bits[i]);
        }

        return n;
    }

    uint32_t WsprRefUnpacker::extract_m(
        const uint8_t *payload_bits,
        std::size_t payload_bit_count) const
    {
        (void)payload_bit_count;

        uint32_t m = 0;

        for (std::size_t i = 28; i < 50; ++i)
        {
            m = (m << 1) | static_cast<uint32_t>(payload_bits[i]);
        }

        return m;
    }

    void WsprRefUnpacker::unpack_callsign_type1(
        uint32_t n,
        std::string &callsign) const
    {
        char out[7] = {};

        out[5] = static_cast<char>((n % 27U) + 10U + 'A' - 10U);
        n /= 27U;

        out[4] = static_cast<char>((n % 27U) + 10U + 'A' - 10U);
        n /= 27U;

        out[3] = static_cast<char>((n % 27U) + 10U + 'A' - 10U);
        n /= 27U;

        out[2] = decode_wspr_base37(n % 10U);
        n /= 10U;

        out[1] = decode_wspr_base37(n % 36U);
        n /= 36U;

        out[0] = decode_wspr_base37(n);

        for (int i = 3; i <= 5; ++i)
        {
            if (out[i] == '[')
                out[i] = ' ';
        }

        callsign.assign(out, 6);

        while (!callsign.empty() && callsign.front() == ' ')
            callsign.erase(callsign.begin());

        while (!callsign.empty() && callsign.back() == ' ')
            callsign.pop_back();
    }

    bool WsprRefUnpacker::unpack_locator_power_type1(
        uint32_t m,
        std::string &locator,
        int &power_dbm) const
    {
        const uint32_t power_field = (m - 64U) % 128U;
        const uint32_t locator_field = (m - 64U) / 128U;

        power_dbm = static_cast<int>(power_field);

        if (locator_field >= (180U * 180U))
            return false;

        const uint32_t a = locator_field / 180U;
        const uint32_t b = locator_field % 180U;

        const uint32_t d0 = 179U - a;
        const uint32_t d1 = b;

        const uint32_t l0 = d0 / 10U;
        const uint32_t l2 = d0 % 10U;

        const uint32_t l1 = d1 / 10U;
        const uint32_t l3 = d1 % 10U;

        if (l0 > 17U || l1 > 17U || l2 > 9U || l3 > 9U)
            return false;

        locator.clear();
        locator.push_back(static_cast<char>('A' + l0));
        locator.push_back(static_cast<char>('A' + l1));
        locator.push_back(static_cast<char>('0' + l2));
        locator.push_back(static_cast<char>('0' + l3));

        return true;
    }

    bool WsprRefUnpacker::unpack_type1(
        const uint8_t *payload_bits,
        std::size_t payload_bit_count,
        WsprDecodedMessage &message) const
    {
        message = WsprDecodedMessage{};

        if (payload_bits == nullptr)
        {
            message.error = "Null payload_bits.";
            return false;
        }

        if (payload_bit_count != WSPR_PAYLOAD_BIT_COUNT)
        {
            message.error = "payload_bit_count must equal WSPR_PAYLOAD_BIT_COUNT.";
            return false;
        }

        const uint32_t n = extract_n(payload_bits, payload_bit_count);
        const uint32_t m = extract_m(payload_bits, payload_bit_count);

        unpack_callsign_type1(n, message.callsign);

        if (!unpack_locator_power_type1(m, message.locator, message.power_dbm))
        {
            message.error = "Failed to unpack locator/power for Type 1.";
            return false;
        }

        message.valid = true;
        message.type = WsprMessageType::Type1;
        return true;
    }
} // namespace wspr
