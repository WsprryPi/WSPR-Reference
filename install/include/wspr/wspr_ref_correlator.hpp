#ifndef WSPR_REF_CORRELATOR_HPP
#define WSPR_REF_CORRELATOR_HPP

#include "wspr_ref_unpack.hpp"

#include <vector>

namespace wspr
{
    class WsprRefCorrelator
    {
    public:
        void add_message(const WsprDecodedMessage &message);

        bool try_resolve_last(WsprDecodedMessage &resolved_message) const;

        void clear();

    private:
        bool can_pair(
            const WsprDecodedMessage &a,
            const WsprDecodedMessage &b) const;

        bool resolve_pair(
            const WsprDecodedMessage &a,
            const WsprDecodedMessage &b,
            WsprDecodedMessage &resolved_message) const;

        std::string combine_callsign_and_extra(
            const std::string &callsign,
            const std::string &extra) const;

        std::vector<WsprDecodedMessage> recent_messages_;
    };
} // namespace wspr

#endif // WSPR_REF_CORRELATOR_HPP
