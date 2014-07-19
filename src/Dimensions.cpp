// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/Dimensions.hpp>

using namespace std;

namespace nix {


//-------------------------------------------------------
// Implementation of Dimension
//-------------------------------------------------------

Dimension::Dimension()
    : ImplContainer()
{
}


Dimension::Dimension(const shared_ptr<IDimension> &p_impl)
    : ImplContainer(p_impl)
{
}


Dimension::Dimension(shared_ptr<IDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}


Dimension::Dimension(const Dimension &other)
    : ImplContainer(other)
{
}


Dimension::Dimension(const SampledDimension &other)
    : ImplContainer(dynamic_pointer_cast<IDimension>(other.impl()))
{
}


Dimension::Dimension(const RangeDimension &other)
    : ImplContainer(dynamic_pointer_cast<IDimension>(other.impl()))
{
}


Dimension::Dimension(const SetDimension &other)
    : ImplContainer(dynamic_pointer_cast<IDimension>(other.impl()))
{
}


Dimension& Dimension::operator=(const SampledDimension &other) {
    shared_ptr<IDimension> tmp(dynamic_pointer_cast<IDimension>(other.impl()));

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


Dimension& Dimension::operator=(const RangeDimension &other) {
    shared_ptr<IDimension> tmp(dynamic_pointer_cast<IDimension>(other.impl()));

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


Dimension& Dimension::operator=(const SetDimension &other) {
    shared_ptr<IDimension> tmp(dynamic_pointer_cast<IDimension>(other.impl()));

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}

valid::Result Dimension::validate() const {
    return valid::validate(std::initializer_list<valid::condition> {
        valid::must(*this, &Dimension::index, valid::notSmaller(1), "index is not set to valid value (> 0)!")
    });
}

//-------------------------------------------------------
// Implementation of SampledDimension
//-------------------------------------------------------

SampledDimension::SampledDimension()
    : ImplContainer()
{
}


SampledDimension::SampledDimension(const std::shared_ptr<ISampledDimension> &p_impl)
    : ImplContainer(p_impl)
{
}


SampledDimension::SampledDimension(std::shared_ptr<ISampledDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}



SampledDimension::SampledDimension(const SampledDimension &other)
    : ImplContainer(other)
{
}


SampledDimension& SampledDimension::operator=(const SampledDimension &other) {
    shared_ptr<ISampledDimension> tmp(other.impl());

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


SampledDimension& SampledDimension::operator=(const Dimension &other) {
    shared_ptr<ISampledDimension> tmp(dynamic_pointer_cast<ISampledDimension>(other.impl()));

    if (other.dimensionType() == DimensionType::Sample && impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}

valid::Result SampledDimension::validate() const {
    valid::Result result = valid::validate(std::initializer_list<valid::condition> {
        valid::must(*this, &SampledDimension::index, valid::notSmaller(1), "index is not set to valid value (size_t > 0)!"),
        valid::must(*this, &SampledDimension::samplingInterval, valid::isGreater(0), "samplingInterval is not set to valid value (> 0)!"),
        valid::must(*this, &SampledDimension::dimensionType, valid::isEqual<DimensionType>(DimensionType::Sample), "dimension type is not correct!"),
        valid::could(*this, &SampledDimension::offset, valid::notFalse(), {
            valid::should(*this, &SampledDimension::unit, valid::isAtomicUnit(), "offset is set, but no valid unit set!") }),
        valid::could(*this, &SampledDimension::unit, valid::notFalse(), {
            valid::must(*this, &SampledDimension::unit, valid::isAtomicUnit(), "Unit is set but not an atomic SI. Note: So far composite units are not supported!") })
    });
        
    return result;
}
//-------------------------------------------------------
// Implementation of SetDimension
//-------------------------------------------------------


SetDimension::SetDimension()
    : ImplContainer()
{
}


SetDimension::SetDimension(const std::shared_ptr<ISetDimension> &p_impl)
    : ImplContainer(p_impl)
{
}

SetDimension::SetDimension(std::shared_ptr<ISetDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}


SetDimension::SetDimension(const SetDimension &other)
    : ImplContainer(other)
{
}


SetDimension& SetDimension::operator=(const SetDimension &other) {
    shared_ptr<ISetDimension> tmp(other.impl());

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


SetDimension& SetDimension::operator=(const Dimension &other) {
    shared_ptr<ISetDimension> tmp(dynamic_pointer_cast<ISetDimension>(other.impl()));

    if (other.dimensionType() == DimensionType::Set && impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}

valid::Result SetDimension::validate() const {
    return valid::validate(std::initializer_list<valid::condition> {
        valid::must(*this, &SetDimension::index, valid::notSmaller(1), "index is not set to valid value (size_t > 0)!"),
        valid::must(*this, &SetDimension::dimensionType, valid::isEqual<DimensionType>(DimensionType::Set), "dimension type is not correct!")
    });
}

//-------------------------------------------------------
// Implementation of RangeDimension
//-------------------------------------------------------


RangeDimension::RangeDimension()
    : ImplContainer()
{
}


RangeDimension::RangeDimension(const std::shared_ptr<IRangeDimension> &p_impl)
    : ImplContainer(p_impl)
{
}

RangeDimension::RangeDimension(std::shared_ptr<IRangeDimension> &&ptr)
    : ImplContainer(std::move(ptr))
{
}


RangeDimension::RangeDimension(const RangeDimension &other)
    : ImplContainer(other)
{
}


RangeDimension& RangeDimension::operator=(const RangeDimension &other) {
    shared_ptr<IRangeDimension> tmp(other.impl());

    if (impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}


RangeDimension& RangeDimension::operator=(const Dimension &other) {
    shared_ptr<IRangeDimension> tmp(dynamic_pointer_cast<IRangeDimension>(other.impl()));

    if (other.dimensionType() == DimensionType::Range && impl() != tmp) {
        std::swap(impl(), tmp);
    }

    return *this;
}

valid::Result RangeDimension::validate() const {
    valid::Result result = valid::validate(std::initializer_list<valid::condition> {
        valid::must(*this, &RangeDimension::index, valid::notSmaller(1), "index is not set to valid value (size_t > 0)!"),
        valid::must(*this, &RangeDimension::ticks, valid::notEmpty(), "ticks are not set!"),
        valid::must(*this, &RangeDimension::dimensionType, valid::isEqual<DimensionType>(DimensionType::Range), "dimension type is not correct!"),
        valid::could(*this, &RangeDimension::unit, valid::notFalse(), {
            valid::must(*this, &RangeDimension::unit, valid::isAtomicUnit(), "Unit is set but not an atomic SI. Note: So far composite units are not supported!") }),
        valid::must(*this, &RangeDimension::ticks, valid::isSorted(), "Ticks are not sorted!")
    });
        
    return result;
}

} // namespace nix
