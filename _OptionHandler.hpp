//
// Created by bodand on 2019-02-05.
//

#pragma once

#include <vector>
#include <utility>
#include <functional>

#include "_Option.hpp"

namespace InfoParse {

    /**
     * Groups multiple _Option<T>-s together
     * for some type T
     * Can execute handling of argument string on all contained
     * _Option<T>-s.
     * Used internally, not really useful outside of
     * the internals of the project
     *
     * @tparam T The type for _Option<T>-s can be accepted into the group
     *
     * @see InfoParse::_Option<T>
     */
    template<class T>
    class _OptionHandler {
        std::vector<_Option<T>> options;

    public:
        /**
         * Adds an option to the internal collection
         * of _Option<T> type options
         *
         * @param option The _Option<T> to be added
         */
        void addOption(_Option<T>&& option);

        /**
         * Constructs an _Option<T> with the supplied
         * parameters within the inner collection thus
         * skipping a move operation.
         *
         * @param longName The LONG name of the option (--longname)
         * @param shortName The SHORT name of the option (-s)
         * @param exporter A pointer to a memory block of type T, into
         *                 which the value will be put
         */
        void addOption(std::string longName, char shortName, T* exporter);

        /**
         * Constructs an _Option<T> with the supplied
         * parameters within the inner collection thus
         * skipping a move operation.
         *
         * @param name The LONG name of the option. Short name is
         *             explicitly set to the first character of the name.
         * @param exporter A pointer to a memory block of type T, into
         *                 which the value will be pu
         *
         * @note The creation of the SHORT name doesn't check if there already
         *       exists an option with the same SHORT name, thus it can be
         *       shadowed by earlier created options' SHORT names
         */
        void addOption(std::string name, T* exporter);

        /**
         * Triggers the execution of argument processing on all
         * stored _Option<T>-s, each following the other in insertion
         * order.
         *
         * @param args The argument string to process
         * @return The remains of the processed string
         */
        std::string handle(const std::string& args) const;

        /**
         * Getter for the options
         * @return The internal collection of options
         */
        const std::vector<_Option<T>>& getOptions() const;

        /**
         * Removes the option with "name".
         * No-op if it is not found internally
         * @param name
         */
        void rmOption(const std::string& name);
    };

    template<class T>
    void _OptionHandler<T>::addOption(_Option<T>&& option) {
        options.push_back(std::move(option));
    }

    template<class T>
    std::string _OptionHandler<T>::handle(const std::string& args) const {
        std::string parsable(args);
        std::for_each(options.begin(), options.end(), [&](const _Option<T>& opt) {
            parsable = opt.match(parsable);
        });
        return parsable;
    }

    template<class T>
    void _OptionHandler<T>::addOption(std::string longName, char shortName, T* exporter) {
        options.emplace_back(std::move(longName), shortName, exporter);
    }

    template<class T>
    void _OptionHandler<T>::addOption(std::string name, T* exporter) {
        options.emplace_back(std::move(name), exporter);
    }

    template<class T>
    const std::vector<_Option<T>>& _OptionHandler<T>::getOptions() const {
        return options;
    }

    template<class T>
    void _OptionHandler<T>::rmOption(const std::string& name) {
        auto i = options.begin();
        while ((*i != name && i != options.end())) { ++i; };
        unless (i == options.end()) {
            options.erase(i);
        }
    }
}


