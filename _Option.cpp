//
// Created by bodand on 2019-02-05.
//


#include "_Option.hpp"

InfoParse::_Option<bool>::_Option(std::string longName, char shortName, bool* exporter) :
        longName(std::move(longName)),
        shortName(shortName),
        exporter(exporter) {}

InfoParse::_Option<bool>::_Option(const std::string& name, bool* exporter) :
        longName(name),
        shortName(name[0]),
        exporter(exporter) {}


std::string InfoParse::_Option<bool>::match(const std::string& args) const {
    std::string parsable(args);
    std::string shortNameString(1, shortName);
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

bool InfoParse::_Option<bool>::operator==(const _Option& rhs) const {
    return longName == rhs.longName;
}

bool InfoParse::_Option<bool>::operator!=(const _Option& rhs) const {
    return !(rhs == *this);
}

bool InfoParse::_Option<bool>::operator==(const std::string& name) const {
    return longName == name;
}

bool InfoParse::_Option<bool>::operator!=(const std::string& name) const {
    return !(longName == name);
}

bool InfoParse::_Option<bool>::operator==(const char* cname) const {
    return *this == std::string(cname);
}

bool InfoParse::_Option<bool>::operator!=(const char* cname) const {
    return *this != std::string(cname);
}
