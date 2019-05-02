//
// Created by bodand on 2019-02-05.
//

#pragma once

#include <vector>
#include <utility>
#include <functional>

#include "_Option.hpp"

namespace InfoParse {
    template<class T>
    class _OptionHandler {
        std::vector<_Option<T>> options;

    public:
        _OptionHandler();
        virtual ~_OptionHandler();

        void addOption(_Option<T> option);

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

    template<class T>
    _OptionHandler<T>::~_OptionHandler() = default;

    template<class T>
    _OptionHandler<T>::_OptionHandler() = default;
}


