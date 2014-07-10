// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DIMENSIONS_H
#define NIX_DIMENSIONS_H

#include <nix/base/ImplContainer.hpp>
#include <nix/base/IDimensions.hpp>
#include <nix/Exception.hpp>

#include <nix/valid/validate.hpp>

namespace nix {


class SampledDimension;
class RangeDimension;
class SetDimension;

/**
 * @brief Instances of the Dimension subclasses are used to define the different dimensions of data in a DataArray.
 *
 * The real dimension descriptor are defined in three subclasses: RangeDimension, SampledDimension and  SetDimension
 */
class NIXAPI Dimension : public virtual base::IDimension, public base::ImplContainer<base::IDimension> {

public:

    /**
     * @brief Constructor that creates an uninitialized Dimension.
     *
     * Calling any method on an uninitialized dimension will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a dimension is initialized:
     *
     * ~~~
     * Dimension e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    Dimension();

    /**
     * @brief Constructor that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Dimension(const std::shared_ptr<base::IDimension> &p_impl);

    /**
     * @brief Constructor with move semantics that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Dimension(std::shared_ptr<base::IDimension> &&ptr);


    /**
     * @brief Copy constructor
     *
     * Copying of all NIX front facing objects like Dimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    Dimension(const Dimension &other);

    /**
     * @brief Copy constructor that converts a SampledDimension to Dimension.
     *
     * Copying of all NIX front facing objects like Dimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    Dimension(const SampledDimension &other);

    /**
     * @brief Copy constructor that converts a RangeDimension to Dimension.
     *
     * Copying of all NIX front facing objects like Dimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    Dimension(const RangeDimension &other);

    /**
     * @brief Copy constructor that converts a SetDimension to Dimension.
     *
     * Copying of all NIX front facing objects like Dimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    Dimension(const SetDimension &other);


    size_t index() const {
        return backend()->index();
    }


    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

    /**
     * @brief Assignment operator that converts a SampledDimension to Dimension.
     *
     * @param other     The dimension to assign.
     */
    Dimension& operator=(const SampledDimension &other);

    /**
     * @brief Assignment operator that converts a RangeDimension to Dimension.
     *
     * @param other     The dimension to assign.
     */
    Dimension& operator=(const RangeDimension &other);

    /**
     * @brief Assignment operator that converts a SetDimension to Dimension.
     *
     * @param other     The dimension to assign.
     */
    Dimension& operator=(const SetDimension &other);
    
    //------------------------------------------------------
    // Validation
    //------------------------------------------------------
    
    valid::Result validate() {
        return valid::validate(std::initializer_list<valid::condition> {
            valid::must(*this, &Dimension::index, valid::notSmaller(1), "index is not set to valid value (> 0)!")
        });
    }

};


/**
 * @brief Dimension descriptor for regularly sampled dimensions.
 *
 * Instances of the SampledDimension Class are used to describe a dimension of data in
 * a DataArray that has been sampled in regular intervals. For example this can be a time axis.
 *
 * Sampled dimensions are characterized by the label for the dimension, the unit in which the
 * sampling interval is given. If not specified otherwise the dimension starts with zero offset.
 *
 * ### Create a sampled dimension
 *
 * The following example will create a sampled dimension on a data array. The dimension
 * has a sampling rate of 10kHz and represents the time axis of a recording.
 *
 * ~~~
 * DataArray da = ...;
 * SampledDimension sd = da.appendSampledDimension(0.1);
 *
 * sd.unit("ms");
 * sd.label("time")
 * sd.offset(10000)
 * ~~~
 */
class NIXAPI SampledDimension : public virtual base::ISampledDimension, public base::ImplContainer<base::ISampledDimension> {

public:

    /**
     * @brief Constructor that creates an uninitialized SampledDimension.
     *
     * Calling any method on an uninitialized dimension will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a dimension is initialized:
     *
     * ~~~
     * SampledDimension e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    SampledDimension();

    /**
     * @brief Constructor that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    SampledDimension(const std::shared_ptr<base::ISampledDimension> &p_impl);

    /**
     * @brief Constructor with move semantics that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    SampledDimension(std::shared_ptr<base::ISampledDimension> &&ptr);

    /**
     * @brief Copy constructor
     *
     * Copying of all NIX front facing objects like SampledDimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    SampledDimension(const SampledDimension &other);

    size_t index() const {
        return backend()->index();
    }

    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

    boost::optional<std::string> label() const {
        return backend()->label();
    }

    void label(const std::string &label) {
        backend()->label(label);
    }

    void label(const none_t t)
    {
        backend()->label(t);
    }

    boost::optional<std::string> unit() const {
        return backend()->unit();
    }

    void unit(const std::string &unit) {
        if (!(util::isSIUnit(unit))) {
            throw InvalidUnit("Unit is not a SI unit. Note: so far, only atomic SI units are supported.", "SampledDimension::unit(const string &unit)");
        }
        backend()->unit(unit);
    }

    void unit(const none_t t)
    {
        backend()->unit(t);
    }

    double samplingInterval() const {
       return backend()->samplingInterval();
    }

    void samplingInterval(double interval) {
        if(interval <= 0.0) {
            throw std::runtime_error("SampledDimenion::samplingInterval: Sampling intervals must be larger than 0.0!");
        }
        backend()->samplingInterval(interval);
    }

    boost::optional<double> offset() const {
        return backend()->offset();
    }

    void offset(double offset) {
        return backend()->offset(offset);
    }

    void offset(const boost::none_t t) {
        backend()->offset(t);
    }

    /**
     * @brief Assignment operator.
     *
     * @param other     The dimension to assign.
     */
    SampledDimension& operator=(const SampledDimension &other);

    /**
     * @brief Assignment operator that converts a Dimension to a SampledDimension.
     *
     * @param other     The dimension to assign.
     */
    SampledDimension& operator=(const Dimension &other);
    
    //------------------------------------------------------
    // Validation
    //------------------------------------------------------
    
    valid::Result validate() {
        valid::Result result_sub = valid::Result();
        valid::Result result = valid::validate(std::initializer_list<valid::condition> {
            valid::must(*this, &SampledDimension::index, valid::notSmaller(1), "index is not set to valid value (size_t > 0)!"),
            valid::must(*this, &SampledDimension::samplingInterval, valid::isGreater(0), "samplingInterval is not set to valid value (> 0)!"),
            valid::must(*this, &SampledDimension::dimensionType, valid::isEqual<DimensionType>(DimensionType::Sample), "dimension type is not correct!")
        });
        
        if(!util::isSIUnit(*unit()) && unit()) {
            result_sub.concat(valid::validate(std::initializer_list<valid::condition> {
                valid::should(*this, &SampledDimension::offset, valid::isFalse(), "offset is set, but no valid unit set!")
            }));
            result_sub.addError(valid::Message(util::numToStr(index()), "Unit is not an atomic SI. Note: So far composite units are not supported!"));
        }
        
        result.concat(result_sub);
        
        return result;
    }

};


/**
 * @brief Dimension descriptor for a dimension that represents just a list or set of values.
 *
 * The SetDimension is used in cases where data is given as a set or list. This can be just a collection of values but
 * also a list of recorded signals or a stack of images. Optionally an array of labels, one for each index of this
 * dimension, can be specified.
 */
class NIXAPI SetDimension : virtual public base::ISetDimension, public base::ImplContainer<base::ISetDimension> {

public:

    /**
     * @brief Constructor that creates an uninitialized SetDimension.
     *
     * Calling any method on an uninitialized dimension will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a dimension is initialized:
     *
     * ~~~
     * SetDimension e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    SetDimension();

    /**
     * @brief Constructor that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    SetDimension(const std::shared_ptr<base::ISetDimension> &p_impl);

    /**
     * @brief Constructor with move semantics that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    SetDimension(std::shared_ptr<base::ISetDimension> &&ptr);

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like SetDimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    SetDimension(const SetDimension &other);

    size_t index() const {
        return backend()->index();
    }

    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

    std::vector<std::string> labels() const {
        return backend()->labels();
    }

    void labels(const std::vector<std::string> &labels) {
        backend()->labels(labels);
    }

    void labels(const boost::none_t t) {
        backend()->labels(t);
    }

    /**
     * @brief Assignment operator.
     *
     * @param other     The dimension to assign.
     */
    SetDimension& operator=(const SetDimension &other);

    /**
     * @brief Assignment operator that converts a Dimension to a SetDimension.
     *
     * @param other     The dimension to assign.
     */
    SetDimension& operator=(const Dimension &other);
    
    //------------------------------------------------------
    // Validation
    //------------------------------------------------------
    
    valid::Result validate() {
        return valid::validate(std::initializer_list<valid::condition> {
            valid::must(*this, &SetDimension::index, valid::notSmaller(1), "index is not set to valid value (size_t > 0)!"),
            valid::must(*this, &SetDimension::dimensionType, valid::isEqual<DimensionType>(DimensionType::Set), "dimension type is not correct!")
        });
    }

};


/**
 * @brief Dimension descriptor for a dimension that is irregularly sampled.
 *
 * The RangeDimension covers cases when indexes of a dimension are mapped to other values
 * in a not regular fashion. A use-case for this would be for example irregularly sampled
 * time-series or certain kinds of histograms. To achieve the mapping of the indexes an
 * array of mapping values must be provided. Those values are stored in the dimensions {@link ticks}
 * property. In analogy to the sampled dimension a {@link unit} and a {@link label} can be defined.
 */
class NIXAPI RangeDimension : virtual public base::IRangeDimension, public base::ImplContainer<base::IRangeDimension> {

public:

    /**
     * @brief Constructor that creates an uninitialized RangeDimension.
     *
     * Calling any method on an uninitialized dimension will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a dimension is initialized:
     *
     * ~~~
     * RangeDimension e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    RangeDimension();

    /**
     * @brief Constructor that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    RangeDimension(const std::shared_ptr<base::IRangeDimension> &p_impl);

    /**
     * @brief Constructor with move semantics that creates a new dimension from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    RangeDimension(std::shared_ptr<base::IRangeDimension> &&ptr);

    /**
     * @brief Copy constructor
     *
     * Copying of all NIX front facing objects like RangeDimension is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The dimension to copy.
     */
    RangeDimension(const RangeDimension &other);

    size_t index() const {
        return backend()->index();
    }

    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

     boost::optional<std::string> label() const {
        return backend()->label();
    }

    void label(const std::string &label) {
        backend()->label(label);
    }

    void label(const none_t t)
    {
        backend()->label(t);
    }

    boost::optional<std::string> unit() const {
        return backend()->unit();
    }

    void unit(const std::string &unit) {
        if (!(util::isSIUnit(unit))) {
            throw InvalidUnit("Unit is not an atomic SI. Note: So far composite units are not supported", "RangeDimension::unit(const string &unit)");
        }
        backend()->unit(unit);
    }

    void unit(const none_t t)
    {
        backend()->unit(t);
    }

    std::vector<double> ticks() const {
        return backend()->ticks();
    }

    void ticks(const std::vector<double> &ticks) {
        if (!std::is_sorted(ticks.begin(), ticks.end())) {
            std::string caller = "Range::ticks()";
            throw UnsortedTicks(caller);
        }
        backend()->ticks(ticks);
    }

    /**
     * @brief Assignment operator.
     *
     * @param other     The dimension to assign.
     */
    RangeDimension& operator=(const RangeDimension &other);

    /**
     * @brief Assignment operator that converts a Dimension to a RangeDimension.
     *
     * @param other     The dimension to assign.
     */
    RangeDimension& operator=(const Dimension &other);
    
    //------------------------------------------------------
    // Validation
    //------------------------------------------------------
    
    valid::Result validate() {
        valid::Result result_sub = valid::Result();
        valid::Result result = valid::validate(std::initializer_list<valid::condition> {
            valid::must(*this, &RangeDimension::index, valid::notSmaller(1), "index is not set to valid value (size_t > 0)!"),
            valid::must(*this, &RangeDimension::ticks, valid::notEmpty(), "ticks are not set!"),
            valid::must(*this, &RangeDimension::dimensionType, valid::isEqual<DimensionType>(DimensionType::Range), "dimension type is not correct!")
        });
        
        if(!util::isSIUnit(*unit()) && unit()) {
            result_sub.addError(valid::Message(util::numToStr(index()), "Unit is not an atomic SI. Note: So far composite units are not supported!"));
        }
        if(!std::is_sorted(ticks().begin(), ticks().end())) {
            result_sub.addError(valid::Message(util::numToStr(index()), "Ticks are not sorted!"));
        }
        
        result.concat(result_sub);
        
        return result;
    }

};


} // namespace nix

#endif // NIX_DIMENSIONS_H
