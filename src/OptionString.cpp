//
// Created by bodand on 2019-07-24.
//

#include "include.hpp"
#include INFO_PARSE_INCLUDE(OptionString.hpp)
#include INFO_PARSE_INCLUDE(utils.hpp)

info::parse::detail::OptionString::OptionString(const std::string& str)
        : _names(OptionString::prependDashes(info::parse::split(str, '|'))) {
    for (auto&& name : _names) {
        _kmpSearch.emplace_back([](/*const */std::string& name) {
          return std::make_shared<FinderEins<char>>(name.begin(), name.end());
        });
        _bmSearch.emplace_back([](/*const */std::string& name) {
          return std::make_shared<FinderZwei<char>>(name.begin(), name.end());
        });
    }
}

const std::vector<std::string>& info::parse::detail::OptionString::getNames() const {
    return _names;
}

std::vector<std::string> info::parse::detail::OptionString::prependDashes(std::vector<std::string>&& rawNames) {
    std::vector<std::string> retVal(rawNames.size());
    std::transform(rawNames.begin(), rawNames.end(), retVal.begin(), [](const std::string& str) {
      if (str == "<>")
          return str;
      return "-" + str;
    });
    return retVal;
}

std::vector<info::parse::detail::searchableOf<char>>
info::parse::detail::OptionString::get() const {
    typedef std::vector<info::parse::detail::searchableOf<char>> Vec;
    Vec retVal;
    for (Vec::size_type i = 0; i < _names.size(); ++i) {
        retVal.emplace_back(std::make_tuple(_names[i], _kmpSearch[i], _bmSearch[i]));
    }
    return retVal;
}

info::parse::detail::OptionString::OptionString(const char* name)
        : OptionString(std::string(name)) {}

const std::string& info::parse::detail::OptionString::operator[](std::vector<std::string>::size_type i) const {
    return _names.at(i);
}

std::vector<info::parse::detail::searchableOf<char>>
info::parse::detail::OptionString::operator*() const {
    return get();
}

bool info::parse::detail::OptionString::operator==(const info::parse::detail::OptionString& rhs) const {
    return _names == rhs._names;
}

bool info::parse::detail::OptionString::operator!=(const info::parse::detail::OptionString& rhs) const {
    return !(rhs == *this);
}

bool info::parse::detail::OptionString::hasShort() const {
    for (auto&& name : _names) {
        if (name.size() == 2)
            return true;
    }
    return false;
}
