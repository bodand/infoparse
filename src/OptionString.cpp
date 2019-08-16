//
// Created by bodand on 2019-07-24.
//

#include "OptionString.hpp"
#include "utils.hpp"

InfoParse::Internals::OptionString::OptionString(const std::string& str)
        : names(OptionString::prependDashes(InfoParse::split(str, '|'))) {
    for (auto&& name : names) {
        kmpSearch.emplace_back([](/*const */std::string& name) {
          return std::make_shared<FinderEins<char>>(name.begin(), name.end());
        });
        bmSearch.emplace_back([](/*const */std::string& name) {
          return std::make_shared<FinderZwei<char>>(name.begin(), name.end());
        });
    }
}

const std::vector<std::string>& InfoParse::Internals::OptionString::getNames() const {
    return names;
}

std::vector<std::string> InfoParse::Internals::OptionString::prependDashes(std::vector<std::string>&& rawNames) {
    std::vector<std::string> retVal(rawNames.size());
    std::transform(rawNames.begin(), rawNames.end(), retVal.begin(), [](const std::string& str) {
      if (str == "<>")
          return str;
      return "-" + str;
    });
    return retVal;
}

std::vector<InfoParse::Internals::searchableOf<char>>
InfoParse::Internals::OptionString::get() const {
    typedef std::vector<InfoParse::Internals::searchableOf<char>> Vec;
    Vec retVal;
    for (Vec::size_type i = 0; i < names.size(); ++i) {
        retVal.emplace_back(std::make_tuple(names[i], kmpSearch[i], bmSearch[i]));
    }
    return retVal;
}

InfoParse::Internals::OptionString::OptionString(const char* name)
        : OptionString(std::string(name)) {}

const std::string& InfoParse::Internals::OptionString::operator[](std::vector<std::string>::size_type i) const {
    return names.at(i);
}

std::vector<InfoParse::Internals::searchableOf<char>>
InfoParse::Internals::OptionString::operator*() const {
    return get();
}

bool InfoParse::Internals::OptionString::operator==(const InfoParse::Internals::OptionString& rhs) const {
    return names == rhs.names;
}

bool InfoParse::Internals::OptionString::operator!=(const InfoParse::Internals::OptionString& rhs) const {
    return !(rhs == *this);
}

bool InfoParse::Internals::OptionString::hasShort() const {
    for (auto&& name : names) {
        if (name.size() == 2)
            return true;
    }
    return false;
}
