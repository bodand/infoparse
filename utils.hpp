//
// Created by bodand on 2019-01-20.
//

#pragma once

#include <vector>
#include <string>

#define unless(x) if (!(x))
#define until(x) while (!(x))

#define ENABLE_IF typename std::enable_if<
#define THEN(T) ,T>::type

namespace InfoParse {
    std::string makeMonolithArgs(int argc, char** argv);

    std::vector<std::string> splitByWhitespace(const std::string& str);

    namespace Internals {

        /**
         * Checks if two supplied type's inheritance relationship
         * is that of a Base and Inheritor
         *
         * @tparam B (Base) The class from which the inheritor shall inherit
         * @tparam I (Inheritor) The class shall inherit from the base
         * @return Whether the inheritance relationship is in check
         */
        template<class B, class I>
        constexpr bool extends() {
            return std::is_base_of<
                    typename std::remove_reference<I>::type,
                    typename std::remove_reference<B>::type
                                  >::value;
        }

        /**
         * Checks if the supplied Factory can create a Product using Args
         * as arguments
         *
         * @tparam F (Factory) The factory class to construct the product
         * @tparam P (Product) The class that is to be created by the factory
         * @tparam Args Parameters for the Factory manufacture() method call
         * @return Whether the Factory can produce the Product with Args
         */
        template<class F, class P,
                 class... Args>
        constexpr bool can_construct_with() {
            return std::is_same<
                    decltype(std::declval<F>().manufacture(std::declval<Args>()...)),
                    P
                               >::value;
        }

        /**
         * Checks if the supplied Factory can create a Product
         * without arguments
         *
         * @tparam F (Factory) The factory class to construct the product
         * @tparam P (Product) The class that is to be created by the factory
         * @return Whether the Factory can produce the Product
         *
         * @see can_construct_with<F, P, Args...>()
         */
        template<class F, class P>
        constexpr bool can_construct() {
            return can_construct_with<F, P>();
        }
    }
}
