#include <utility>

#pragma once

#include <utility>
#include <string>
#include <sstream>
#include <iostream>

#include "utils.hpp"

namespace InfoParse {
    namespace Internals {
        std::string __getOptionValueAsString(std::size_t startMatch, std::string& args,
                                             const std::string& name, const std::string& sequence);
        std::wstring __getOptionValueAsWString(std::size_t startMatch, std::string& args,
                                               const std::string& name, const std::string& sequence);
    }

    template<class T>
    class __Option {
        std::string longName;
        char shortName;
        T* exporter;

    public:
        __Option(std::string longName, char shortName, T* exporter);
        __Option(const std::string& name, T* exporter);

        virtual ~__Option();

        std::string match(const std::string& args) const;

    private:
        void handleParameterParsing(std::size_t startMatch, std::string& args,
                                    const std::string& name, const std::string& sequence) const;
    };

    template<>
    class __Option<bool> {
        std::string longName;
        char shortName;
        bool* exporter;

    public:
        __Option(std::string longName, char shortName, bool* exporter);
        __Option(const std::string& name, bool* exporter);

        virtual ~__Option();

        std::string match(const std::string& args) const;
    };

    template<class T>
    std::string __Option<T>::match(const std::string& args) const {
        std::string parsable(args);
        std::string shortNameString(std::string("") + shortName);
        std::size_t startMatch;
        const auto longSequence = std::string(" --") + longName + " ";
        const auto shortSequence = std::string(" -") + shortName + " ";

        unless ((startMatch = parsable.find(longSequence)) == -1) {
            handleParameterParsing(startMatch, /*&*/parsable, longName, longSequence);
        } else unless ((startMatch = parsable.find(shortSequence)) == -1) {
            handleParameterParsing(startMatch, /*&*/parsable, shortNameString, shortSequence);
        }

        return parsable;
    }

    template<class T>
    __Option<T>::__Option(std::string longName, char shortName, T* exporter):
            longName(std::move(longName)),
            shortName(shortName),
            exporter(exporter) {}

    template<class T>
    __Option<T>::__Option(const std::string& name, T* exporter):
            longName(name),
            shortName(name[0]),
            exporter(exporter) {}

    template<class T>
    void __Option<T>::handleParameterParsing(std::size_t startMatch, std::string& args,
                                             const std::string& name, const std::string& sequence) const {
        if constexpr (!std::is_same_v<decltype(exporter), std::wstring*> &&
                      !std::is_same_v<decltype(exporter), wchar_t*>) {
            std::stringstream exportStream(Internals::__getOptionValueAsString(startMatch, args,
                                                                               name, sequence));
            exportStream >> *exporter;
        } else {
            std::wstringstream exportStream(Internals::__getOptionValueAsWString(startMatch, args,
                                                                                 name, sequence));
            exportStream >> *exporter;
        }
    }

    template<class T>
    __Option<T>::~__Option() = default;
}

