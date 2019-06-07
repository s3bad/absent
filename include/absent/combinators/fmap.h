#ifndef RVARAGO_ABSENT_FMAP_H
#define RVARAGO_ABSENT_FMAP_H

#include "absent/combinators/bind.h"
#include "absent/syntax/member.h"

#include <functional>
#include <utility>

namespace rvarago::absent {

    /***
     * Given a nullable type N<A> (i.e. optional like object), and an unary function f: A -> B:
     * - When empty: it should return a new empty nullable N<B>.
     * - When *not* empty: it should return a value generated by applying the unary mapping function to the input
     * nullable's wrapped in a brand new nullable N<B>.
     *
     * @param input any optional like object that can be checked against empty and provide access to its wrapped value.
     * @param fn an unary function A -> B.
     * @return a new nullable containing the mapped value of type B, possibly empty if input is also empty.
     */
    template <template <typename> typename Nullable, typename Mapper, typename A>
    constexpr auto fmap(Nullable<A> const& input, Mapper fn) -> Nullable<decltype(fn(std::declval<A>()))> {
        return bind(input, [&fn](auto const& value){ return Nullable<decltype(fn(std::declval<A>()))>{fn(value)}; });
    }

    /***
     * The same as fmap but for a member function that has to be const and parameterless.
     */
    template <template <typename> typename Nullable, typename A, typename B>
    constexpr auto fmap(Nullable<A> const& input, syntax::member::Mapper<const A, B> fn) -> Nullable<B> {
        return fmap(input, [&fn](auto const& input_value){ return std::invoke(fn, input_value); });
    }

    /***
     * Infix version of fmap.
     */
    template <template <typename> typename Nullable, typename Mapper, typename A>
    constexpr auto operator|(Nullable<A> const& input, Mapper fn) -> decltype(fmap(input, fn)) {
        return fmap(input, fn);
    }

    /**
     * Infix version of fmap for a member function.
     */
    template <template <typename> typename Nullable, typename A, typename B>
    constexpr auto operator|(Nullable<A> const& input, syntax::member::Mapper<const A, B> fn) -> Nullable<B> {
        return fmap(input, fn);
    }

}

#endif //RVARAGO_ABSENT_FMAP_H
