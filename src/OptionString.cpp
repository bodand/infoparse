//
// Created by bodand on 2019-07-24.
//

#include "OptionString.hpp"
#include "utils.hpp"

InfoParse::Internals::OptionString::OptionString(const std::string& str)
        : names(OptionString::prependDashes(InfoParse::split(str, '|'))) {
    for (auto&& name : names) {
        kmpSearch.emplace_back([&]() {
          return std::make_shared<knuth_morris_pratt<StrIter>>(name.begin(), name.end());
        });
        bmSearch.emplace_back([&]() {
          return std::make_shared<boyer_moore<StrIter>>(name.begin(), name.end());
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

std::vector<InfoParse::Internals::searchableOf<char>> InfoParse::Internals::OptionString::get() const {
    typedef std::vector<InfoParse::Internals::searchableOf<char>> Vec;
    Vec retVal;
    for (Vec::size_type i = 0; i < names.size(); ++i) {
        retVal.emplace_back(std::make_tuple(names[i], kmpSearch[i], bmSearch[i]));
    }
    return retVal;
}
