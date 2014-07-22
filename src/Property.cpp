// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include <list>

#include <nix.hpp>

using namespace std;
using namespace nix;

valid::Result Property::validate() {
    valid::Result result_base = base::Entity<base::IProperty>::validate();
    valid::Result result = valid::validate(std::initializer_list<valid::condition> {
        valid::must(*this, &Property::name, valid::notEmpty(), "name is not set!"),
        valid::could(*this, &Property::valueCount, valid::notFalse(), {
            valid::should(*this, &Property::unit, valid::notFalse(), "values are set, but unit is missing!") }),
        valid::must(*this, &Property::unit, valid::isValidUnit(), "Unit is not SI or composite of SI units.")
        // TODO: dataType to be tested too?
    });

    return result.concat(result_base);
}
