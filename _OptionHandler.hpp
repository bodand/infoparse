//
// Created by bodand on 2019-02-05.
//

#pragma once

#include <vector>
#include <utility>
#include <functional>

#include "_Option.hpp"

namespace InfoParse {

    /**
     * Groups multiple _Option<T>-s together
     * for some type T
     * Can execute handling of argument string on all contained
     * _Option<T>-s.
     * Used internally, not really useful outside of
     * the internals of the project
     *
     * @tparam T The type for _Option<T>-s can be accepted into the group
     *
     * @see InfoParse::_Option<T>
     */
    template<class T>
    class _OptionHandler {
        std::vector<_Option<T>> options;

    public:
        /**
         * Adds an option to the internal collection
         * of _Option<T> type options
         *
         * @param option The _Option<T> to be added
         */
        void addOption(_Option<T> option);

        /**
         * Triggers the execution of argument processing on all
         * stored _Option<T>-s, each following the other in insertion
         * order.
         *
         * @param args The argument string to process
         * @return The remains of the processed string
         */
        std::string handle(const std::string& args) const;
    };

    template<class T>
    void _OptionHandler<T>::addOption(_Option<T> option) {
        options.push_back(option);
    }

    template<class T>
    std::string _OptionHandler<T>::handle(const std::string& args) const {
        std::string parsable(args);
        std::for_each(options.begin(), options.end(), [&](const _Option<T>& opt) {
            parsable = opt.match(parsable);
        });
        return parsable;
    }
}


