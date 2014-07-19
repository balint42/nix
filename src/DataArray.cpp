// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/DataArray.hpp>

namespace nix {

double DataArray::applyPolynomial(std::vector<double> &coefficients, double origin, double input) const{
    double value = 0.0;
    double term = 1.0;
    for(size_t i = 0; i < coefficients.size(); i++) {
        value += coefficients[i] * term;
        term *= input - origin;
    }
    return value;
}

valid::Result DataArray::validate() const {
    valid::Result result_base = base::EntityWithSources<base::IDataArray>::validate();
    valid::Result result = valid::validate(std::initializer_list<valid::condition> {
        valid::must(*this, &DataArray::dataType, valid::notEqual<DataType>(DataType::Nothing), "data type is not set!"),
        valid::should(*this, &DataArray::dimensionCount, valid::isEqual<size_t>(dataExtent().size()), "data dimensionality does not match number of defined dimensions!"),
        valid::must(*this, &DataArray::unit, valid::isValidUnit(), "Unit is not SI or composite of SI units."),
        valid::could(*this, &DataArray::polynomCoefficients, valid::notEmpty(), {
            valid::should(*this, &DataArray::expansionOrigin, valid::notFalse(), "polynomial coefficients for calibration are set, but expansion origin is missing!") }),
        valid::could(*this, &DataArray::expansionOrigin, valid::notFalse(), {
            valid::should(*this, &DataArray::polynomCoefficients, valid::notEmpty(), "expansion origin for calibration is set, but polynomial coefficients are missing!") }),
        valid::could(*this, &DataArray::dimensions, valid::notEmpty(), {
            valid::must(*this, &DataArray::dimensions, valid::dimTicksMatchData<DataArray>(*this), "in some of the Range dimensions the number of ticks differs from the number of data entries along the corresponding data dimension!"),
            valid::must(*this, &DataArray::dimensions, valid::dimLabelsMatchData<DataArray>(*this), "in some of the Set dimensions the number of labels differs from the number of data entries along the corresponding data dimension!") })
    });
        
    return result.concat(result_base);
}
    
}
