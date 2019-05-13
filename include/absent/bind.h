#ifndef RVARAGO_ABSENT_BIND_H
#define RVARAGO_ABSENT_BIND_H

#include "member.h"

#include <functional>
#include <utility>

namespace rvarago::absent {

    /***
     * Given a nullable type N<A> (i.e. optional like object), and an unary function f: A -> N<B>:
     * - When empty: it should return a new empty nullable N<B>.
     * - When *not* empty: it should return a value generated by applying the unary mapping function to the input
     * nullable's which is already wrapped a nullable N<B>.
     *
     * @param input any optional like object that can be converted to bool and de-referenced.
     * @param fn an unary function A -> N<B>.
     * @return a new nullable generated by fn, possibly empty if input is also empty.
     */
    template <typename A, template <typename> typename Nullable, typename Mapper>
    constexpr auto bind(Nullable<A> const& input, Mapper fn) -> decltype(fn(std::declval<A>())) {
        if (!input) {
            return {};
        }
        return fn(*input);
    }

    /***
     * The same as bind but for a member function that has to be const and parameterless.
     */
    template <typename A, typename B, template <typename> typename Nullable>
    constexpr auto bind(Nullable<A> const& input, member::Mapper<const A, Nullable<B>> fn) -> Nullable<B> {
        return bind(input, [&fn](auto const& input_value){ return std::invoke(fn, input_value); });
    }

    /***
     * Infix version of bind.
     */
    template <typename A, template <typename> typename Nullable, typename Mapper>
    constexpr auto operator>>(Nullable<A> const& input, Mapper fn) -> decltype(bind(input, fn)) {
        return bind(input, fn);
    }

    /***
     * Infix version of bind for a member function.
     */
    template <typename A, typename B, template <typename> typename Nullable>
    constexpr auto operator>>(Nullable<A> const& input, member::Mapper<const A, Nullable<B>> fn) -> Nullable<B> {
        return bind(input, fn);
    }

}

#endif //RVARAGO_ABSENT_BIND_H
