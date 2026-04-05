#ifndef WSPR_CONSTANTS_HPP
#define WSPR_CONSTANTS_HPP

#include <cstddef>
#include <cstdint>

namespace wspr
{
    // -----------------------------------------------------------------------------
    // Core WSPR constants
    // -----------------------------------------------------------------------------

    constexpr std::size_t WSPR_SYMBOL_COUNT = 162;
    constexpr std::size_t WSPR_BIT_COUNT = 162;
    constexpr std::size_t WSPR_PAYLOAD_BIT_COUNT = 81;
    constexpr uint32_t WSPR_POLY_0 = 0xf2d05351U;
    constexpr uint32_t WSPR_POLY_1 = 0xe4613c47U;

    // -----------------------------------------------------------------------------
    // Sync vector (fixed for WSPR)
    // -----------------------------------------------------------------------------

    inline constexpr uint8_t SYNC_VECTOR[WSPR_SYMBOL_COUNT] = {
        1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0,
        0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1,
        0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0,
        1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
        0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1,
        1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0};

} // namespace wspr

#endif // WSPR_CONSTANTS_HPP
