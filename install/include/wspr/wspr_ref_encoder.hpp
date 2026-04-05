#ifndef WSPR_REF_ENCODER_HPP
#define WSPR_REF_ENCODER_HPP

#include <cstddef>
#include <cstdint>

#include "wspr_constants.hpp"

namespace wspr
{

class WsprRefEncoder
{
public:
    void wspr_encode(const char *call, const char *loc, int8_t dbm, uint8_t *symbols);

    bool debug_get_payload_bits(
        const char* call,
        const char* loc,
        int8_t dbm,
        uint8_t* payload_bits) const;

private:
    uint8_t wspr_code(char c) const;
    void wspr_message_prep(char *call, char *loc, int8_t dbm);
    void wspr_bit_packing(uint8_t *c);
    void wspr_interleave(uint8_t *s) const;
    void wspr_merge_sync_vector(const uint8_t *g, uint8_t *symbols) const;
    void convolve(const uint8_t *c, uint8_t *s, uint8_t message_size, uint8_t bit_size) const;
    void pad_callsign(char *call) const;

    char callsign_[13];
    char locator_[7];
    int8_t power_;
};
} // namespace wspr

#endif
