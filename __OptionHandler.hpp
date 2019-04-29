//
// Created by bodand on 2019-02-05.
//

#pragma once

#include <vector>
#include <utility>
#include <functional>

#include "__Option.hpp"

namespace InfoParse {
    template<class T>
    class __OptionHandler {
        std::vector<__Option<T>> options;

    public:
        __OptionHandler();
        virtual ~__OptionHandler();

        void addOption(__Option<T> option);

        std::string handle(const std::string& args) const;
    };

    template<class T>
    void __OptionHandler<T>::addOption(__Option<T> option) {
        options.push_back(option);
    }

    template<class T>
    std::string __OptionHandler<T>::handle(const std::string& args) const {
        std::string parsable(args);
        std::for_each(options.begin(), options.end(), [&](const __Option<T>& opt) {
            parsable = opt.match(parsable);
        });
        return parsable;
    }

    template<class T>
    __OptionHandler<T>::~__OptionHandler() = default;

    template<class T>
    __OptionHandler<T>::__OptionHandler() = default;
}


