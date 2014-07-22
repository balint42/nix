// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_VALIDATE_H
#define NIX_VALIDATE_H

#include <nix/Platform.hpp>
#include <nix/valid/result.hpp>
#include <nix/valid/conditions.hpp>
#include <cstdarg>

#include <nix.hpp>

namespace nix {
namespace valid {

/**
  * @brief Generic validator
  * 
  * Function taking a list of conditions that it will all execute and
  * return the results as one merged {@link Result} object
  *
  * @param li initializer list of conditions
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const std::vector<condition> &li);

/**
  * @brief base entity validator
  * 
  * Function taking a base entity and returning {@link Result} object
  *
  * @param entity base entity
  *
  * @returns The validation results as {@link Result} object
  */
template<typename T>
Result validate(const base::Entity<T> &entity) {
    return validate({
        must(entity, &base::Entity<T>::id, notEmpty(), "id is not set!"),
        must(entity, &base::Entity<T>::createdAt, notFalse(), "date is not set!")
    });
}

/**
  * @brief base named entity validator
  * 
  * Function taking a base named entity and returning {@link Result}
  * object
  *
  * @param named_entity base named entity
  *
  * @returns The validation results as {@link Result} object
  */
template<typename T>
Result validate(const base::NamedEntity<T> &named_entity) {
    Result result_base = validate(static_cast<base::Entity<T>>(named_entity));
    Result result = validate({
        must(named_entity, &base::NamedEntity<T>::name, notEmpty(), "no name set!"),
        must(named_entity, &base::NamedEntity<T>::type, notEmpty(), "no type set!")
    });
        
    return result.concat(result_base);
}

/**
  * @brief base entity with metadata validator
  * 
  * Function taking a base entity with metadata and returning
  * {@link Result} object
  *
  * @param entity_with_metadata base entity with metadata
  *
  * @returns The validation results as {@link Result} object
  */
template<typename T>
Result validate(const base::EntityWithMetadata<T> &entity_with_metadata) {
    return validate(static_cast<base::NamedEntity<T>>(entity_with_metadata));
}

/**
  * @brief base entity with sources validator
  * 
  * Function taking a base entity with sources and returning
  * {@link Result} object
  *
  * @param entity_with_sources base entity with sources
  *
  * @returns The validation results as {@link Result} object
  */
template<typename T>
Result validate(const base::EntityWithSources<T> &entity_with_sources) {
    return validate(static_cast<base::EntityWithMetadata<T>>(entity_with_sources));
}

/**
  * @brief Block entity validator
  * 
  * Function taking a Block entity and returning {@link Result} object
  *
  * @param block Block entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Block &block);

/**
  * @brief DataArray entity validator
  * 
  * Function taking a DataArray entity and returning {@link Result}
  * object
  *
  * @param data_array DataArray entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const DataArray &data_array);

/**
  * @brief SimpleTag entity validator
  * 
  * Function taking a SimpleTag entity and returning {@link Result}
  * object
  *
  * @param simple_tag SimpleTag entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const SimpleTag &simple_tag);

/**
  * @brief Property entity validator
  * 
  * Function taking a Property entity and returning {@link Result}
  * object
  *
  * @param property Property entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Property &property);

/**
  * @brief DataTag entity validator
  * 
  * Function taking a DataTag entity and returning {@link Result} object
  *
  * @param data_tag DataTag entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const DataTag &data_tag);

/**
  * @brief Dimension entity validator
  * 
  * Function taking a Dimension entity and returning {@link Result}
  * object
  *
  * @param dim Dimension entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Dimension &dim);

/**
  * @brief RangeDimension entity validator
  * 
  * Function taking a RangeDimension entity and returning {@link Result}
  * object
  *
  * @param range_dim RangeDimension entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const RangeDimension &range_dim);

/**
  * @brief SampledDimension entity validator
  * 
  * Function taking a SampledDimension entity and returning
  * {@link Result} object
  *
  * @param sampled_dim SampledDimension entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const SampledDimension &sampled_dim);

/**
  * @brief SetDimension entity validator
  * 
  * Function taking a SetDimension entity and returning {@link Result}
  * object
  *
  * @param set_dim SetDimension entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const SetDimension &set_dim);

/**
  * @brief Feature entity validator
  * 
  * Function taking a Feature entity and returning {@link Result} object
  *
  * @param feature Feature entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Feature &feature);

/**
  * @brief Section entity validator
  * 
  * Function taking a Section entity and returning {@link Result} object
  *
  * @param section Section entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Section &section);

/**
  * @brief Source entity validator
  * 
  * Function taking a Source entity and returning {@link Result} object
  *
  * @param source Source entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const Source &source);

/**
  * @brief File entity validator
  * 
  * Function taking a File entity and returning {@link Result} object
  *
  * @param file File entity
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validate(const nix::File &file);

} // namespace valid
} // namespace nix

#endif // NIX_VALIDATE_H
