//
// Created by bodand on 2019-02-05.
//

#include <iostream>
#include <codecvt>
#include <locale>

#include "__Option.hpp"

InfoParse::__Option<bool>::__Option(std::string longName, char shortName, bool* exporter) :
        longName(std::move(longName)),
        shortName(shortName),
        exporter(exporter) {}

InfoParse::__Option<bool>::__Option(const std::string& name, bool* exporter) :
        longName(name),
        shortName(name[0]),
        exporter(exporter) {}

InfoParse::__Option<bool>::~__Option() = default;

std::string InfoParse::Internals::__getOptionValueAsString(std::size_t startMatch,
                                                           std::string& args,
                                                           const std::string& name,
                                                           const std::string& sequence) {
    std::size_t matchEnd = args.find(' ', startMatch + sequence.length() + 1);
    std::size_t matchLength = matchEnd - startMatch;
    std::string match = args.substr(startMatch, matchLength);
    args.erase(startMatch, matchLength);

    return match.substr(match.find_last_of(' ') + 1);
}

std::wstring InfoParse::Internals::__getOptionValueAsWString(std::size_t startMatch,
                                                             std::string& args,
                                                             const std::string& name,
                                                             const std::string& sequence) {
    std::size_t matchEnd = args.find(' ', startMatch + sequence.length() + 1);
    std::size_t matchLength = matchEnd - startMatch;
    std::string match = args.substr(startMatch, matchLength);
    args.erase(startMatch, matchLength);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    return converter.from_bytes(match.substr(match.find_last_of(' ') + 1));
}

std::string InfoParse::__Option<bool>::match(const std::string& args) const {
    std::string parsable(args);
    std::string shortNameString(std::string("") + shortName);
    std::size_t startMatch;
    const auto longSequence = std::string(" --") + longName + " ";
    const auto shortSequence = std::string(" -") + shortName + " ";

    unless ((startMatch = parsable.find(longSequence)) == -1) {
        parsable.erase(startMatch, longSequence.length() - 1);
        *exporter = true;
    } else unless ((startMatch = parsable.find(shortSequence)) == -1) {
        parsable.erase(startMatch, shortSequence.length() - 1);
        *exporter = true;
    }

    return parsable;
}
