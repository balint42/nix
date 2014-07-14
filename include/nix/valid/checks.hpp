// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_CHECKS_H
#define NIX_CHECKS_H

#include <boost/optional.hpp>
#include <nix/util/util.hpp>
#include <nix/valid/helper.hpp>

namespace nix {
namespace valid {
    

    /**
     * @brief Check if later given not greater than initally defined value.
     * 
     * One Check struct that checks whether the given value is not greater
     * than the initially given other value, both of which have to be 
     * convertible to double.
     */
    struct notGreater {
        const double value;
        
        template<typename T>
        notGreater(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return !(static_cast<double>(val) > value);
        }
    };

    /**
     * @brief Check if later given greater than initally defined value.
     * 
     * One Check struct that checks whether the given value is greater than
     * the initially given other value, both of which have to be 
     * convertible to double.
     */
    struct isGreater {
        const double value;
        
        template<typename T>
        isGreater(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return static_cast<double>(val) > value;
        }
    };

    /**
     * @brief Check if later given not smaller than initally defined value.
     * 
     * One Check struct that checks whether the given value is not smaller
     * than the initially given other value, both of which have to be 
     * convertible to double.
     */
    struct notSmaller {
        const double value;
        
        template<typename T>
        notSmaller(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return !(static_cast<double>(val) < value);
        }
    };

    /**
     * @brief Check if later given smaller than initally defined value.
     * 
     * One Check struct that checks whether the given value is smaller than
     * the initially given other value, both of which have to be 
     * convertible to double.
     */
    struct isSmaller {
        const double value;
        
        template<typename T>
        isSmaller(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return static_cast<double>(val) < value;
        }
    };

    /**
     * @brief Check for un-equality of initally defined and later given value.
     * 
     * One Check struct that checks whether the given value is not equal
     * to the initially given other value.
     */
    template<typename T>
    struct notEqual {
        const T value;
        
        notEqual(T value) : value(value) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return value != val;
        }
    };

    /**
     * @brief Check for equality of initally defined and later given value.
     * 
     * One Check struct that checks whether the given value is equal to
     * the initially given other value.
     */
    template<typename T>
    struct isEqual {
        const T value;
        
        isEqual(T value) : value(value) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return value == val;
        }
    };
    // needed because: for bizarre reasons bool converts to int when compared to boost::optional
    template<>
    struct isEqual<bool> {
        const bool value;
        
        isEqual(bool value) : value(value) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return value ? !!val : !val;
        }
    };

    /**
     * @brief Check if given value casts to boolean true
     * 
     * One Check struct that checks whether the given value casts to true
     * or to false.
     * T can be: boost::optional, boost::none, nix-entity
     * and any basic type.
     */
    struct notFalse {
        // WARNING: enum will convert via int, which means 0 = false !
        template<typename T>
        bool operator()(const T &val) const {
            return !!val;
        }
    };

    /**
     * @brief Check if given value casts to boolean false
     * 
     * One Check struct that checks whether the given value casts to false
     * or to true.
     * T can be: boost::optional, boost::none, nix-entity
     * and any basic type.
     */
    struct isFalse {
        // WARNING: enum will convert via int, which means 0 = false !
        template<typename T>
        bool operator()(const T &val) const {
            return !val;
        }
    };

    /**
     * @brief Check if given class/struct returns "empty() == false"
     * 
     * One Check struct that checks whether the given value is not empty
     * or is empty.
     * T can be: any STL container.
     */
    struct notEmpty {
        template<typename T>
        bool operator()(const T &val) const {
            return !(val.empty());
        }
    };

    /**
     * @brief Check if given class/struct returns "empty() == true"
     * 
     * One Check struct that checks whether the given value is empty or
     * not.
     * T can be: any STL container.
     */
    struct isEmpty {
        template<typename T>
        bool operator()(const T &val) const {
            return val.empty();
        }
    };

    /**
     * @brief Check if given class represents a valid unit string
     * 
     * One Check struct that checks whether the given string represents
     * a valid atomic SI or compound SI unit.
     * Parameter can be of type boost optional (containing nothing or 
     * string) or of type string.
     */
    struct isValidUnit {
        template<typename T>
        bool operator()(const boost::optional<T> &u) const {
            return u ? (util::isSIUnit(*u) || util::isCompoundSIUnit(*u)) : false;
        }
        bool operator()(const std::string &u) const {
            return (util::isSIUnit(u) || util::isCompoundSIUnit(u));
        }
    };

    /**
     * @brief Check if given class represents a valid atomic SI unit string
     * 
     * One Check struct that checks whether the given string represents
     * a valid atomic SI unit.
     * Parameter can be of type boost optional (containing nothing or 
     * string) or of type string.
     */
    struct isAtomicUnit {
        template<typename T>
        bool operator()(const boost::optional<T> &u) const {
            return u ? util::isSIUnit(*u) : false;
        }
        bool operator()(const std::string &u) const {
            return util::isSIUnit(u);
        }
    };

    /**
     * @brief Check if given class represents a valid compound SI unit string
     * 
     * One Check struct that checks whether the given string represents
     * a valid compound SI unit.
     * Parameter can be of type boost optional (containing nothing or 
     * string) or of type string.
     */
    struct isCompoundUnit {
        template<typename T>
        bool operator()(const boost::optional<T> &u) const {
            return u ? util::isCompoundSIUnit(*u) : false;
        }
        bool operator()(const std::string &u) const {
            return util::isCompoundSIUnit(u);
        }
    };

    /**
     * @brief Check if given value can be regarded as being set
     * 
     * One Check struct that checks whether the given value can be
     * considered set, by applying {@link notFalse} and {@link notEmpty}
     * checks. Value thus is set if: STL cotnainer not empty,
     * bool is true, boost optional is set, number is not 0.
     * Parameter can be of above types or even boost none_t.
     */
    struct isSet {
        template<typename T>
        bool operator()(const T &val) const {
            typedef typename std::conditional<hasEmpty<T>::value, notEmpty, notFalse>::type subCheck;
            return subCheck(val);
        }
    };

    /**
     * @brief Check if given container is sorted using std::is_sorted
     * 
     * One Check struct that checks whether the given container is sorted
     * according to std::is_sorted. Thus supports types that are 
     * supported by std::is_sorted.
     */
    struct isSorted {
        template<typename T>
        bool operator()(const T &container) const {
            return std::is_sorted(container.begin(), container.end());
        }
    };

} // namespace valid
} // namespace nix

#endif // NIX_CHECKS_H
