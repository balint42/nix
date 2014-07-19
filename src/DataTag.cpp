// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix.hpp>
//#include <nix/DataTag.hpp>

namespace nix {

valid::Result DataTag::validate() const {
    valid::Result result_base = base::EntityWithSources<base::IDataTag>::validate();
    valid::Result result = valid::validate(std::initializer_list<valid::condition> {
        valid::must(*this, &DataTag::positions, valid::notFalse(), "positions are not set!"),
        // since extents & positions DataArray stores a vector of position / extent vectors it has to be 2-dim
        valid::could(*this, &DataTag::positions, valid::notFalse(), {
            valid::must(*this, &DataTag::positions, valid::dimEquals(2), "dimensionality of positions DataArray must be two!") }),
        valid::could(*this, &DataTag::extents, valid::notFalse(), {
            valid::must(*this, &DataTag::extents, valid::dimEquals(2), "dimensionality of positions DataArray must be two!") }),
        // check units for validity
        valid::could(*this, &DataTag::units, valid::notEmpty(), {
            valid::must(*this, &DataTag::units, valid::isValidUnit(), "Some of the units in tag are invalid: not an atomic SI. Note: So far composite SI units are not supported!") }),
        valid::must(*this, &DataTag::references, valid::tagRefsHaveUnits(units()), "Some of the referenced DataArrays' dimensions don't have units where the tag has. Make sure that all references have the same number of dimensions as the tag has units and that each dimension has a unit set."),
        valid::must(*this, &DataTag::references, valid::tagUnitsMatchRefsUnits(units()), "Some of the referenced DataArrays' dimensions have units that are not convertible to the units set in tag. Note: So far composite SI units are not supported!"),
        // check positions & extents
        valid::could(*this, &DataTag::extents, valid::notFalse(), {
            valid::must(*this, &DataTag::positions, valid::extentsMatchPositions<decltype(extents())>(extents()), "Number of entries in positions and extents do not match!") }),
        valid::could(*this, &DataTag::references, valid::notEmpty(), {
            valid::could(*this, &DataTag::extents, valid::notFalse(), {
                valid::must(*this, &DataTag::extents, valid::extentsMatchRefs<decltype(references())>(references()), "number of entries (in 2nd dim) in extents does not match number of dimensions in all referenced DataArrays!") }),
            valid::must(*this, &DataTag::positions, valid::positionsMatchRefs<decltype(references())>(references()), "number of entries (in 2nd dim) in positions does not match number of dimensions in all referenced DataArrays!") })
    });

    return result.concat(result_base);
}
    
}
