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

#include <nix/base/IDimensions.hpp>

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
    struct isValidUnit;
    /**
     * @brief Check if given class represents valid SI unit string(s)
     * 
     * Base struct to be inherited by the {@link isValidUnit}, {@link
     * isAtomicUnit}, {@link isCompoundUnit}. Not viable on its own!
     */
    struct isUnit {
        virtual bool operator()(const std::string &u) const = 0;
        
        template<typename T>
        bool operator()(const boost::optional<T> &u) const {
            // note: relying on short-curcuiting here
            return u && (*this)(*u);
        }
        
        template<typename T>
        bool operator()(const std::vector<std::string> &u, T obj) const {
            // if test fails we have invalid unit & find_if_not will return it != end
            return std::find_if_not(u.begin(), u.end(), obj) != u.end();
        }
    };

    /**
     * @brief Check if given class represents valid SI unit string(s)
     * 
     * One Check struct that checks whether the given string(s) represent(s)
     * a valid atomic or compound SI unit.
     * Parameter can be of type boost optional (containing nothing or 
     * string) or of type string or a vector of strings.
     */
    struct isValidUnit : public isUnit {
        bool operator()(const std::string &u) const {
            return (util::isSIUnit(u) || util::isCompoundSIUnit(u));
        }
        template<typename T>
        bool operator()(const boost::optional<T> &u) const {
            return isUnit::operator()<T>(u);
        }
        bool operator()(const std::vector<std::string> &u) const {
            return isUnit::operator()(u, *this);
        }
    };

    /**
     * @brief Check if given class represents valid atomic SI unit string(s)
     * 
     * One Check struct that checks whether the given string(s) represent(s)
     * a valid atomic SI unit.
     * Parameter can be of type boost optional (containing nothing or 
     * string) or of type string or a vector of strings.
     */
    struct isAtomicUnit : public isUnit {
        bool operator()(const std::string &u) const {
            return util::isSIUnit(u);
        }
        template<typename T>
        bool operator()(const boost::optional<T> &u) const {
            return isUnit::operator()<T>(u);
        }
        bool operator()(const std::vector<std::string> &u) const {
            return isUnit::operator()(u, *this);
        }
    };

    /**
     * @brief Check if given class represents valid compound SI unit string(s)
     * 
     * One Check struct that checks whether the given string(s) represent(s)
     * a valid compound SI unit.
     * Parameter can be of type boost optional (containing nothing or 
     * string) or of type string or a vector of strings.
     */
    struct isCompoundUnit : public isUnit {
        bool operator()(const std::string &u) const {
            return util::isCompoundSIUnit(u);
        }
        template<typename T>
        bool operator()(const boost::optional<T> &u) const {
            return isUnit::operator()<T>(u);
        }
        bool operator()(const std::vector<std::string> &u) const {
            return isUnit::operator()(u, *this);
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

    /**
     * @brief Check if given DataArray has given dimensionality
     * 
     * One Check struct that checks whether the given DataArray entity
     * has a dimensionality of the given uint value by getting its' 
     * NDSize class via the "dataExtent" method and checking its' size
     * via "size" the method.
     */
    struct dimEquals {
        const size_t value;
        
        dimEquals(const size_t &value) : value(value) {}
        
        template<typename T>
        bool operator()(const T &array) const {
            return (array.dataExtent().size() == value);
        }
    };

    /**
     * @brief Check if given referenced DataArrays' dimensions all have units
     * 
     * One Check struct that checks whether the given referenced
     * DataArrays' dimensions all have units defined where the tag has.
     * (where the tag has means at the same index in the tag's units
     * vector as the dimension index in the referenced DataArray)
     * Therefore it loops through all dimensions of all given references 
     * and checks whether the dimension has a unit set if the tag has. 
     * The test counts as passed if all dimensions have units set (where
     * the tag has). The test fails if the dimension has no unit set
     * (though the tag has) or if it is a dimension type without unit at
     * all (and the tag has a unit set) {@link SetDimension}.
     * This is an extension of {@link tagUnitsMatchRefsUnits}, thus 
     * checks things {@link tagUnitsMatchRefsUnits} does not.
     */
    struct tagRefsHaveUnits {
        const std::vector<std::string> units;
        
        tagRefsHaveUnits(const std::vector<std::string> &units) : units(units) {}
        
        template<typename T>
        bool operator()(const std::vector<T> &references) const {
            bool mismatch = false; // loop until true

            // check if each unit of tag is convertible to unit of each dim of each referenced DataArray
            if(!units.empty() && !references.empty()) {
                auto u = units;             // defined units to loop
                auto refs = references;     // referenced DataArrays to loop
                auto itU = u.begin();       // units iterator
                auto itRefs = refs.begin(); // references iterator
                // loop referenced DataArrays
                while(!mismatch && (itRefs != refs.end())) {
                    if(((*itRefs).dimensions().size() == u.size()) && (u.size() > 0)) {
                        auto itDims = (*itRefs).dimensions().begin();
                        // loop referenced DataArray dims
                        while(!mismatch && (itDims != (*itRefs).dimensions().end()) && (itU != u.end())) {
                            std::string dimUnit = valid::Unit<valid::hasUnit<decltype(*itDims)>::value>().get(*itDims);
                            // check if dim has unit method at all (is not SetDimension)
                            if(!(*itU).empty() && ((*itDims).dimensionType() == DimensionType::Set)) {
                                mismatch = true;
                            }
                            else if((dimUnit.empty() && !(*itU).empty()) || (!dimUnit.empty() && (*itU).empty())) {
                                mismatch = true;
                            }
                            ++itDims;
                            ++itU;
                        }
                        ++itRefs;
                    }
                    else {
                        mismatch = true;
                    }
                }
                ++itU;
            } // if vectors not empty
            
            return !mismatch;
        } // bool operator()
    }; // struct

    /**
     * @brief Check if given units match given referenced DataArrays' units
     * 
     * One Check struct that checks whether the given units (vector of
     * strings) match the given referenced DataArrays' (vector of
     * DataArray references) units. Therefore it loops through all
     * dimensions of all given references and checks whether the
     * dimension has a unit set and if so whether it is convertible to
     * the unit with the same index in the given units vector.
     * The test counts as passed if all dimensions that have units set,
     * have units that are convertible. Thus this test does _not_ fail
     * just because a dimension has no unit (by type or because not 
     * set). It _does_ fail if the number of dimensions in a referenced
     * DataArray differs the number of given units.
     * This is an extension of {@link tagRefsHaveUnits}, thus checks
     * things that {@link tagRefsHaveUnits} does not.
     */
    struct tagUnitsMatchRefsUnits {
        const std::vector<std::string> units;
        
        tagUnitsMatchRefsUnits(const std::vector<std::string> &units) : units(units) {}
        
        template<typename T>
        bool operator()(const std::vector<T> &references) const {
            bool mismatch = false; // loop until true

            // check if each unit of tag is convertible to unit of each dim of each referenced DataArray
            if(!units.empty() && !references.empty()) {
                auto u = units;             // defined units to loop
                auto refs = references;     // referenced DataArrays to loop
                auto itU = u.begin();       // units iterator
                auto itRefs = refs.begin(); // references iterator
                // loop referenced DataArrays
                while(!mismatch && (itRefs != refs.end())) {
                    if(((*itRefs).dimensions().size() == u.size()) && (u.size() > 0)) {
                        auto itDims = (*itRefs).dimensions().begin();
                        // loop referenced DataArray dims
                        while(!mismatch && (itDims != (*itRefs).dimensions().end()) && (itU != u.end())) {
                            std::string dimUnit = valid::Unit<valid::hasUnit<decltype(*itDims)>::value>().get(*itDims);
                            if(!dimUnit.empty()) {
                                if(!util::isScalable(*itU, dimUnit)) {
                                    mismatch = true;
                                }
                            }
                            ++itDims;
                            ++itU;
                        }
                        ++itRefs;
                    }
                    else {
                        mismatch = true;
                    }
                }
                ++itU;
            } // if vectors not empty
            
            return !mismatch;
        } // bool operator()
    }; // struct

    /**
     * @brief Check if given number of positions and extents matches
     * 
     * One Check struct that checks whether the given number of
     * positions matches the given number of extents. It is irrelevant
     * which gets passed at construction time and which via operator().
     */
    template<typename T1>
    struct extentsMatchPositions {
        const T1 extents;
        
        extentsMatchPositions(const T1 &extents) : extents(extents) {}
        
        template<typename T2>
        bool operator()(const T2 &positions) const {
            // check that positions.dataExtent()[0] == extents.dataExtent()[0]
            // and that   positions.dataExtent()[1] == extents.dataExtent()[1]
            // and that   positions.dataExtent().size() == extents.dataExtent().size()
            return positions.dataExtent() == extents.dataExtent();
        }
    };
    template<>
    struct extentsMatchPositions<std::vector<double>> {
        const std::vector<double> extents;
        
        extentsMatchPositions(const std::vector<double> &extents) : extents(extents) {}
        
        bool operator()(const std::vector<double> &positions) const {
            return positions.size() == extents.size();
        }
    };

    /**
     * @brief Check if number of extents (along 2nd dim) match number of references' data dims
     * 
     * One Check struct that checks whether the given number of
     * extents (if DataArray: size along 2nd dimensions of extents 
     * DataArray; if vector: size of vector) matches the given number of 
     * dimensions in each of the given referenced DataArrays.
     */
    template<typename T1>
    struct extentsMatchRefs {
        const T1 refs;
        
        extentsMatchRefs(const T1 &refs) : refs(refs) {}

        template<typename T2>
        bool operator()(const T2 &extents) const {
            bool mismatch = false;
            auto extExtent = extents.dataExtent();
            auto it = refs.begin();
            while(!mismatch && (it != refs.end())) {
                auto arrayExtent = (*it).dataExtent();
                mismatch = extExtent[1] != arrayExtent.size();
                ++it;
            }
            
            return mismatch;
        }
        bool operator()(const std::vector<double> &extents) const {
            bool mismatch = false;
            auto extSize = extents.size();
            auto it = refs.begin();
            while(!mismatch && (it != refs.end())) {
                auto arrayExtent = (*it).dataExtent();
                mismatch = extSize != arrayExtent.size();
                ++it;
            }
            
            return mismatch;
        }
    };

    /**
     * @brief Check if number of positions (along 2nd dim) match number of references' data dims
     * 
     * One Check struct that checks whether the given number of
     * positions (if DataArray: size along 2nd dimensions of positions 
     * DataArray; if vector: size of vector) matches the given number of 
     * dimensions in each of the given referenced DataArrays.
     * Note: this is just an alias for extentsMatchRefs wich does the
     * same thing.
     */
    template<typename T1>
    struct positionsMatchRefs {
        const T1 refs;

        positionsMatchRefs(const T1 &refs) : refs(refs) {}
    
        template<typename T2>
        bool operator()(const T2 &positions) const {
            extentsMatchRefs<T1> alias = extentsMatchRefs<T1>(refs);
            
            return alias(positions);
        }
    };

    /**
     * @brief Check if range dimension specifics ticks match data
     * 
     * One Check struct that checks whether the dimensions of type
     * "Range" in the given dimensions vector have ticks that match
     * the given DataArray's data: number of ticks == number of entries
     * along the corresponding dimension in the data.
     */
    template<typename T1>
    struct dimTicksMatchData {
        const T1 data;

        dimTicksMatchData(const T1 &data) : data(data) {}
    
        template<typename T2>
        bool operator()(const T2 &dims) const {
            bool mismatch = false;
            auto it = dims.begin();
            while(!mismatch && it != dims.end()) {
                if((*it).dimensionType() == DimensionType::Range) {
                    size_t dimIndex = (*it).index();
                    auto dim = (*it).asRangeDimension();
                    mismatch = dim.ticks().size() == data.dataExtent()[dimIndex];
                }
                ++it;
            }
            
            return mismatch;
        }
    };

    /**
     * @brief Check if set dimension specifics labels match data
     * 
     * One Check struct that checks whether the dimensions of type
     * "Set" in the given dimensions vector have labels that match
     * the given DataArray's data: number of labels == number of entries
     * along the corresponding dimension in the data.
     */
    template<typename T1>
    struct dimLabelsMatchData {
        const T1 data;

        dimLabelsMatchData(const T1 &data) : data(data) {}
    
        template<typename T2>
        bool operator()(const T2 &dims) const {
            bool mismatch = false;
            auto it = dims.begin();
            while(!mismatch && it != dims.end()) {
                if((*it).dimensionType() == DimensionType::Set) {
                    size_t dimIndex = (*it).index();
                    auto dim = (*it).asSetDimension();
                    mismatch = dim.labels().size() == data.dataExtent()[dimIndex];
                }
                ++it;
            }
            
            return mismatch;
        }
    };


} // namespace valid
} // namespace nix

#endif // NIX_CHECKS_H
