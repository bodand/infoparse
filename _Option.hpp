#include <utility>

#pragma once

#include <utility>
#include <string>
#include <sstream>
#include <iostream>
#include <codecvt>
#include <locale>

#include "utils.hpp"

namespace InfoParse {
    template<class T>
    class _Option {
        std::string longName;
        char shortName;
        T* exporter;

    public:
        _Option(std::string longName, char shortName, T* exporter);

        _Option(const std::string& name, T* exporter);

        virtual ~_Option();

        std::string match(const std::string& args) const;

    private:
        void handleParameterParsing(std::size_t startMatch, std::string& args,
                                    const std::string& name, const std::string& sequence) const;

        std::string _getOptionValueAsString(std::size_t startMatch, std::string& args,
                                            const std::string& name, const std::string& sequence) const;

        std::wstring _getOptionValueAsWString(std::size_t startMatch, std::string& args,
                                              const std::string& name, const std::string& sequence) const;
    };

    template<>
    class _Option<bool> {
        std::string longName;
        char shortName;
        bool* exporter;

    public:
        _Option(std::string longName, char shortName, bool* exporter);

        _Option(const std::string& name, bool* exporter);

        virtual ~_Option();

        std::string match(const std::string& args) const;
    };

    template<class T>
    std::string _Option<T>::match(const std::string& args) const {
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
    _Option<T>::_Option(std::string longName, char shortName, T* exporter):
            longName(std::move(longName)),
            shortName(shortName),
            exporter(exporter) {}

    template<class T>
    _Option<T>::_Option(const std::string& name, T* exporter):
            longName(name),
            shortName(name[0]),
            exporter(exporter) {}

    template<class T>
    void _Option<T>::handleParameterParsing(std::size_t startMatch, std::string& args,
                                            const std::string& name, const std::string& sequence) const {
        if constexpr (!std::is_same_v<std::remove_pointer_t<decltype(exporter)>, std::wstring> &&
                      !std::is_same_v<std::remove_pointer_t<decltype(exporter)>, wchar_t>) {
            std::stringstream exportStream(_getOptionValueAsString(startMatch, args,
                                                                   name, sequence));
            exportStream >> *exporter;
        } else {
            std::wstringstream exportStream(_getOptionValueAsWString(startMatch, args,
                                                                     name, sequence));
            exportStream >> *exporter;
        }
        if constexpr (std::is_same_v<std::remove_pointer_t<decltype(exporter)>, std::wstring> ||
                      std::is_same_v<std::remove_pointer_t<decltype(exporter)>, std::string>) {
            arcItrStr(*exporter);
        }
    }

    template<class T>
    std::string _Option<T>::_getOptionValueAsString(std::size_t startMatch,
                                                    std::string& args,
                                                    const std::string& name,
                                                    const std::string& sequence) const {
        std::size_t matchEnd = args.find(' ', startMatch + sequence.length() + 1);
        std::size_t matchLength = matchEnd - startMatch;
        std::string match = args.substr(startMatch, matchLength);
        args.erase(startMatch, matchLength);

        return match.substr(match.find_last_of(' ') + 1);
    }

    template<class T>
    std::wstring _Option<T>::_getOptionValueAsWString(std::size_t startMatch,
                                                      std::string& args,
                                                      const std::string& name,
                                                      const std::string& sequence) const {
        std::size_t matchEnd = args.find(' ', startMatch + sequence.length() + 1);
        std::size_t matchLength = matchEnd - startMatch;
        std::string match = args.substr(startMatch, matchLength);
        args.erase(startMatch, matchLength);

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

        return converter.from_bytes(match.substr(match.find_last_of(' ') + 1));
    }

    template<class T>
    _Option<T>::~_Option() = default;
}

