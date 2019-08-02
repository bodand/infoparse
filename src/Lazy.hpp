//
// Created by bodand on 2019-07-24.
//

#pragma once

#include <functional>
#include <memory>
#include <exception>

#include "utils.hpp"

namespace InfoParse::Internals {
  /**
   * @brief Exception class; thrown
   *        if requesting casting Lazy<T> to T
   *        and T is not initializable without args
   *
   * Exception class thrown
   * if a Lazy<T> object is requested
   * to construct and initialize its
   * internal T object, however upon construction
   * of the Lazy<T> object the initializer function
   * requires arguments.
   */
  class bad_lazy_eval : public virtual std::exception {
      /// Interface
  public:
      const char* what() const noexcept override;

      /// Lifecycle
  public:
      bad_lazy_eval(std::string typname);

      /// Fields
  private:
      std::string typname;
  };

  /**
   * A class that removes the eager instantiation
   * required by RAII to take place.
   *
   * Instantiates an instance of type T on the heap,
   * whenever the value is first requested. If never
   * requested it will never be instantiated.
   *
   * @tparam T Type to instantiate
   * @tparam TArgs Arguments with which T is instantiated
   */
  template<class T,
          class... TArgs>
  class Lazy {
      /// Interface
  public:
      /**
       * Returns and possibly instantiates
       * an instance of type T with arguments TArgs
       * @param[in] args Arguments with which T is instantiated
       * @return A reference to new or an already instantiated
       *            heap-allocated object.
       *
       * @see operator()
       */
      _retval const T& get(TArgs... args) const;

      /**
       * Returns whether the value is already instantiated
       */
      _retpure bool isInited() const;

      /**
       * Implicit casts object to type T if plausible:
       *  - type T is already constructed; or
       *  - type T is constructable without arguments
       *    using the provided function (!= type T itself
       *    if default constructable)
       *
       * @return A const reference to the heap-allocated
       *         object of type T
       *
       * @throws bad_lazy_cast
       */
      _retpure operator const T&() const;

      /**
       * Returns a reference to
       * an instance of type T if plausible:
       *  - type T is already constructed; or
       *  - type T is constructable without arguments
       *    using the provided function (!= type T itself
       *    if default constructable)
       *
       * @return A const reference to the heap-allocated
       *         object of type T
       *
       * @throws bad_lazy_cast
       */
      _retpure const T& operator*() const;

      /**
       * Returns and possibly instantiates
       * an instance of type T with arguments TArgs
       * @param[in] args Arguments with which T is instantiated
       * @return A reference to new or an already instantiated
       *            heap-allocated object.
       *
       * @see get
       */
      _retpure const T& operator()(TArgs... args) const;

      /// Lifecycle
  public:
      Lazy() = delete;
      /**
       * Constructs Lazy<T> by initializing the initer function of
       * type `TArgs -> std::shared_ptr<T>`.
       *
       * @param[in] initer The function to instantiate and initialize
       *                the instance of type T whenever requested
       */
      Lazy(const std::function<std::shared_ptr<T>(TArgs...)>& initer);

      /// Fields
  private:
      /// Function to initialize `val`
      const std::function<std::shared_ptr<T>(TArgs...)> initer;
      /// The lazily allocated heap-object
      mutable std::shared_ptr<T> val;
      /// Determines whether `val` has been initialized already
      mutable bool inited;
  };

  template<class T, class... TArgs>
  inline Lazy<T, TArgs...>::Lazy(const std::function<std::shared_ptr<T>(TArgs...)>& initer)
          : initer(initer),
            inited(false) {}

  template<class T, class... TArgs>
  inline bool Lazy<T, TArgs...>::isInited() const {
      return inited;
  }

  template<class T, class... TArgs>
  inline const T& Lazy<T, TArgs...>::get(TArgs... args) const {
      if (!inited) {
          inited = true;
          val = initer(args...);
      }
      return *val;
  }

  template<class T, class... TArgs>
  inline Lazy<T, TArgs...>::operator const T&() const {
      return **this;
  }

  template<class T, class... TArgs>
  inline const T& Lazy<T, TArgs...>::operator*() const {
      if (inited) {
          return *val;
      }
      if constexpr (sizeof...(TArgs) == 0) {
          return get();
      } else {
          throw bad_lazy_eval(typeid(T).name());
      }
  }

  template<class T, class... TArgs>
  inline const T& Lazy<T, TArgs...>::operator()(TArgs... args) const {
      return get(args...);
  }

}
