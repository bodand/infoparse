/*
 * Copyright (c) 2019, András Bodor
 * Licensed under BSD 3-Clause
 * For more information see the supplied
 * LICENSE file
 */

//
// Created by bodand on 2019-02-05.
//


#include "Option_.hpp"

InfoParse::Option_<bool>::Option_(std::string longName, char shortName, bool* exporter) :
        longName(std::move(longName)),
        shortName(shortName),
        exporter(exporter) {}

InfoParse::Option_<bool>::Option_(const std::string& name, bool* exporter) :
        longName(name),
        shortName(name[0]),
        exporter(exporter) {}


std::string InfoParse::Option_<bool>::match(const std::string& args) const {
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

bool InfoParse::Option_<bool>::operator==(const Option_& rhs) const {
    return longName == rhs.longName;
}

bool InfoParse::Option_<bool>::operator!=(const Option_& rhs) const {
    return !(rhs == *this);
}

bool InfoParse::Option_<bool>::operator==(const std::string& name) const {
    return longName == name;
}

bool InfoParse::Option_<bool>::operator!=(const std::string& name) const {
    return !(longName == name);
}

bool InfoParse::Option_<bool>::operator==(const char* cname) const {
    return *this == std::string(cname);
}

bool InfoParse::Option_<bool>::operator!=(const char* cname) const {
    return *this != std::string(cname);
}
