// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix.hpp>
//#include <nix/Feature.hpp>

namespace nix {

valid::Result Feature::validate() const {
    valid::Result result_base = base::Entity<base::IFeature>::validate();
    valid::Result result = valid::validate(std::initializer_list<valid::condition> {
        valid::must(*this, &Feature::data, valid::notFalse(), "data is not set!"),
        valid::must(*this, &Feature::linkType, valid::notSmaller(0), "linkType is not set!")
    });
        
    return result.concat(result_base);
}
    
}
