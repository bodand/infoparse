//
// Created by bodand on 2019-02-06.
//

#include "OptionsParser.hpp"

#include <iterator>
#include <regex>

std::string InfoParse::OptionsParser::parse(int argc, char** argv) {
    return parse(InfoParse::makeMonolithArgs(argc, argv));
}

std::string InfoParse::OptionsParser::parse(const std::string& args) {
    std::string parsable(equalizeWhitespace(explodeBundledFlags(args)));
    for (const auto& handler : optionHandlers) {
        parsable = handler.second.second(handler.second.first, parsable);
    }
    return parsable;
}

std::string InfoParse::OptionsParser::explodeBundledFlags(const std::string& args) {
    std::string parsable(args);
    std::size_t bundleStart = 0;
    std::string bundleSequence(" -");
    do {
        bundleStart = parsable.find(bundleSequence, bundleStart);
        if (bundleStart == -1) break;
        if (parsable[bundleStart + 2] == '-' && ++bundleStart) continue;

        std::size_t bundleEnd = parsable.find(' ', bundleStart + 1);
        std::size_t bundleSize = bundleEnd - bundleStart - 1;
        if (bundleSize <= 1 && ++bundleStart) continue;

        std::string bundle = parsable.substr(bundleStart, bundleSize + 1);
        parsable.erase(bundleStart, bundleSize + 1);
        std::stringstream explodedBundleStream;
        for (const auto& ch : bundle) {
            unless (ch == ' ' || ch == '-') {
                explodedBundleStream << " -" << ch << ' ';
            }
        }
        std::string explodedBundle = explodedBundleStream.str();

        parsable.insert(bundleStart, explodedBundle);
        bundleStart++;
    } while (true);
    return parsable;
}

std::string InfoParse::OptionsParser::equalizeWhitespace(const std::string& args) {
    return std::regex_replace(args, std::regex("\\s+"), " ");
}
