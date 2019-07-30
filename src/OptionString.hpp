//
// Created by bodand on 2019-07-24.
//

#pragma once

#include <string>
#include <vector>

#include "Lazy.hpp"
#include "utils.hpp"

#include <boost/algorithm/searching/knuth_morris_pratt.hpp>
#include <boost/algorithm/searching/boyer_moore.hpp>

namespace InfoParse::Internals {
  using boost::algorithm::knuth_morris_pratt;
  using boost::algorithm::boyer_moore;
  using StrIter = std::string::const_iterator;
  template<class T>
  using searchableOf = std::tuple<
          std::basic_string<T>,
          Lazy<knuth_morris_pratt<typename std::basic_string<T>::const_iterator>>,
          Lazy<boyer_moore<typename std::basic_string<T>::const_iterator>>
  >;

  class OptionString {
      /// Interface
  public:
      _retpure const std::vector<std::string>& getNames() const;
      _retpure std::vector<searchableOf<char>> get() const;

      /// Constructor
  public:
      OptionString(const std::string& name);

      /// Fields
  private:
      std::vector<std::string> names;
      std::vector<Lazy<knuth_morris_pratt<StrIter>>> kmpSearch;
      std::vector<Lazy<boyer_moore<StrIter>>> bmSearch;

      /// Methods
  private:
      _pure static std::vector<std::string> prependDashes(std::vector<std::string>&& rawNames);
  };
}

