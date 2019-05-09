//
// Created by bodand on 2019-02-06.
//
//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "OCDFAInspection"
#pragma once

#include <typeinfo>
#include <typeindex>
#include <string>
#include <map>
#include <memory>

#include "utils.hpp"
#include "_OptionHandler.hpp"

namespace InfoParse {
    class OptionsParser {
        std::map<std::type_index, std::pair<void*,
                std::function<std::string(void*, const std::string&)>>> optionHandlers;

    public:
        template<class T>
        ENABLE_IF true THEN
        (void) addOption(const std::string& longName, char shortName, T* exporter);
        template<class T>
        ENABLE_IF true THEN
        (void) addOption(const std::string& name, T* exporter);

        std::string parse(int argc, char** argv);
        std::string parse(const std::string& args);

    private:
        std::string explodeBundledFlags(const std::string& args);
        std::string equalizeWhitespace(const std::string& args);
    };

    template<class T>
    ENABLE_IF true THEN
    (void) OptionsParser::addOption(const std::string& longName, char shortName, T* exporter) {
        if (optionHandlers.find(typeid(T)) == optionHandlers.end()) {
            optionHandlers[typeid(T)].first = (void*) new _OptionHandler<T>();
            optionHandlers[typeid(T)].second = [](void* optionVoid, const std::string& args) {
                return ((_OptionHandler<T>*) optionVoid)->handle(args);
            };
        }
        ((_OptionHandler<T>*) optionHandlers[typeid(T)].first)
                ->addOption(longName, shortName, exporter);
    }

    template<class T>
    ENABLE_IF true THEN
    (void) OptionsParser::addOption(const std::string& name, T* exporter) {
        if (optionHandlers.find(typeid(T)) == optionHandlers.end()) {
            optionHandlers[typeid(T)].first = (void*) new _OptionHandler<T>();
            optionHandlers[typeid(T)].second = [](void* optionVoid, const std::string& args) {
                return ((_OptionHandler<T>*) optionVoid)->handle(args);
            };
        }
        ((_OptionHandler<T>*) optionHandlers[typeid(T)].first)->addOption(name, exporter);
    }
}

//#pragma clang diagnostic pop
