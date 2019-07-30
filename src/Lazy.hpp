//
// Created by bodand on 2019-07-24.
//

#pragma once

#include <functional>
#include <memory>
#include "utils.hpp"

namespace InfoParse::Internals {
  template<class T>
  class Lazy {
      /// Interface
  public:
      _retval const T& get() const;
      _retpure bool isInited() const;

      _retpure operator const T&() const;
      _retpure operator bool() const;
      _retpure const T& operator*() const;

      /// Constructors
  public:
      Lazy() = delete;
      Lazy(const std::function<std::shared_ptr<T>()>& initer);

      /// Fields
  private:
      const std::function<std::shared_ptr<T>()> initer;
      mutable std::shared_ptr<T> val;
      mutable bool inited;
  };

  template<class T>
  inline Lazy<T>::operator const T&() const {
      return get();
  }

  template<class T>
  inline const T& Lazy<T>::get() const {
      if (!inited) {
          inited = true;
          val = initer();
      }
      return *val;
  }

  template<class T>
  inline Lazy<T>::Lazy(const std::function<std::shared_ptr<T>()>& initer)
          : initer(initer),
            inited(false) {
  }

  template<class T>
  inline bool Lazy<T>::isInited() const {
      return inited;
  }

  template<class T>
  inline Lazy<T>::operator bool() const {
      return inited;
  }

  template<class T>
  const T& Lazy<T>::operator*() const {
      return this->get();
  }
}
