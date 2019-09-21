#ifndef RVARAGO_ABSENT_FOREACH_H
#define RVARAGO_ABSENT_FOREACH_H

#include "absent/combinators/bind.h"

namespace rvarago::absent {

    /***
     * Given a nullable type N<A> (i.e. optional like object), and an unary function f: A -> void:
     * - When empty: it should do nothing.
     * - When *not* empty: it should apply the unary function to the input nullable's wrapped only for its side-effect.
     *
     * @param input any optional like object that can be checked against empty and provide access to its wrapped value.
     * @param fn an unary function A -> void.
     */
    template <template <typename...> typename Nullable, typename Effect, typename A, typename... Rest>
    constexpr auto foreach(Nullable<A, Rest...> const& input, Effect fn) noexcept -> void {
        if (!nullable::syntax::empty<Nullable, A, Rest...>::_(input)) {
            fn(nullable::syntax::value<Nullable, A, Rest...>::_(input));
        }
    }

}

#endif //RVARAGO_ABSENT_FOREACH_H
