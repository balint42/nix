// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix.hpp>
//#include <nix/SimpleTag.hpp>

using namespace std;

namespace nix {

valid::Result SimpleTag::validate() const {
    valid::Result result_base = base::EntityWithSources<base::ISimpleTag>::validate();
    valid::Result result = valid::validate(std::initializer_list<valid::condition> {
        valid::must(*this, &SimpleTag::referenceCount, valid::isGreater(0), "references are not set!"),
        // check units for validity
        valid::could(*this, &SimpleTag::units, valid::notEmpty(), {
            valid::must(*this, &SimpleTag::units, valid::isValidUnit(), "Unit is invalid: not an atomic SI. Note: So far composite units are not supported!") }),
        valid::must(*this, &SimpleTag::references, valid::tagRefsHaveUnits(units()), "Some of the referenced DataArrays' dimensions don't have units where the tag has. Make sure that all references have the same number of dimensions as the tag has units and that each dimension has a unit set."),
        valid::must(*this, &SimpleTag::references, valid::tagUnitsMatchRefsUnits(units()), "Some of the referenced DataArrays' dimensions have units that are not convertible to the units set in tag. Note: So far composite SI units are not supported!"),
        // check positions & extents
        valid::could(*this, &SimpleTag::extent, valid::notEmpty(), {
            valid::must(*this, &SimpleTag::position, valid::notEmpty(), "Extent is set but position is missing!"),
            valid::must(*this, &SimpleTag::position, valid::extentsMatchPositions<decltype(extent())>(extent()), "Number of entries in position and extent do not match!"),
            valid::must(*this, &SimpleTag::extent, valid::extentsMatchRefs<decltype(references())>(references()), "number of entries in extent does not match number of dimensions in all referenced DataArrays!") }),
        valid::could(*this, &SimpleTag::position, valid::notEmpty(), {
            valid::must(*this, &SimpleTag::position, valid::positionsMatchRefs<decltype(references())>(references()), "number of entries in position does not match number of dimensions in all referenced DataArrays!") })
    });
        
    return result.concat(result_base);
}
    
} // namespace nix
