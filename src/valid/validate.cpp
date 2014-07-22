// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/valid/validate.hpp>
#include <nix/valid/checks.hpp>
#include <nix/valid/conditions.hpp>
#include <nix/valid/result.hpp>

#include <nix.hpp>

namespace nix {
namespace valid {

Result validate(const std::vector<condition> &li) {
    Result result = Result();

    for (auto &sub : li) {
         result = result.concat(
            sub()
        );
    }

    return result;
}
    
Result validate(const Block &block) {
    return validate(static_cast<base::EntityWithMetadata<base::IBlock>>(block));
}

Result validate(const DataArray &data_array) {
    Result result_base = validate(static_cast<base::EntityWithSources<base::IDataArray>>(data_array));
    Result result = validate({
        must(data_array, &DataArray::dataType, notEqual<DataType>(DataType::Nothing), "data type is not set!"),
        should(data_array, &DataArray::dimensionCount, isEqual<size_t>(data_array.dataExtent().size()), "data dimensionality does not match number of defined dimensions!", {
            could(data_array, &DataArray::dimensions, notEmpty(), {
                must(data_array, &DataArray::dimensions, dimTicksMatchData<DataArray>(data_array), "in some of the Range dimensions the number of ticks differs from the number of data entries along the corresponding data dimension!"),
                must(data_array, &DataArray::dimensions, dimLabelsMatchData<DataArray>(data_array), "in some of the Set dimensions the number of labels differs from the number of data entries along the corresponding data dimension!") }) }),
        must(data_array, &DataArray::unit, isValidUnit(), "Unit is not SI or composite of SI units."),
        could(data_array, &DataArray::polynomCoefficients, notEmpty(), {
            should(data_array, &DataArray::expansionOrigin, notFalse(), "polynomial coefficients for calibration are set, but expansion origin is missing!") }),
        could(data_array, &DataArray::expansionOrigin, notFalse(), {
            should(data_array, &DataArray::polynomCoefficients, notEmpty(), "expansion origin for calibration is set, but polynomial coefficients are missing!") })
    });
    
    return result.concat(result_base);
}

Result validate(const SimpleTag &simple_tag) {
    Result result_base = validate(static_cast<base::EntityWithSources<base::ISimpleTag>>(simple_tag));
    Result result = validate({
        must(simple_tag, &SimpleTag::referenceCount, isGreater(0), "references are not set!"),
        // check units for validity
        could(simple_tag, &SimpleTag::units, notEmpty(), {
            must(simple_tag, &SimpleTag::units, isValidUnit(), "Unit is invalid: not an atomic SI. Note: So far composite units are not supported!") }),
        must(simple_tag, &SimpleTag::references, tagRefsHaveUnits(simple_tag.units()), "Some of the referenced DataArrays' dimensions don't have units where the tag has. Make sure that all references have the same number of dimensions as the tag has units and that each dimension has a unit set."),
        must(simple_tag, &SimpleTag::references, tagUnitsMatchRefsUnits(simple_tag.units()), "Some of the referenced DataArrays' dimensions have units that are not convertible to the units set in tag. Note: So far composite SI units are not supported!"),
        // check positions & extents
        could(simple_tag, &SimpleTag::extent, notEmpty(), {
            must(simple_tag, &SimpleTag::position, notEmpty(), "Extent is set but position is missing!"),
            must(simple_tag, &SimpleTag::position, extentsMatchPositions<decltype(simple_tag.extent())>(simple_tag.extent()), "Number of entries in position and extent do not match!"),
            must(simple_tag, &SimpleTag::extent, extentsMatchRefs<decltype(simple_tag.references())>(simple_tag.references()), "number of entries in extent does not match number of dimensions in all referenced DataArrays!") }),
        could(simple_tag, &SimpleTag::position, notEmpty(), {
            must(simple_tag, &SimpleTag::position, positionsMatchRefs<decltype(simple_tag.references())>(simple_tag.references()), "number of entries in position does not match number of dimensions in all referenced DataArrays!") })
    });
        
    return result.concat(result_base);
}

Result validate(const Property &property) {
    Result result_base = validate(static_cast<base::Entity<base::IProperty>>(property));
    Result result = validate({
        must(property, &Property::name, notEmpty(), "name is not set!"),
        could(property, &Property::valueCount, notFalse(), {
            should(property, &Property::unit, notFalse(), "values are set, but unit is missing!") }),
        must(property, &Property::unit, isValidUnit(), "Unit is not SI or composite of SI units.")
        // TODO: dataType to be tested too?
    });

    return result.concat(result_base);
}

Result validate(const DataTag &data_tag) {
    Result result_base = validate(static_cast<base::EntityWithSources<base::IDataTag>>(data_tag));
    Result result = validate({
        must(data_tag, &DataTag::positions, notFalse(), "positions are not set!"),
        // since extents & positions DataArray stores a vector of position / extent vectors it has to be 2-dim
        could(data_tag, &DataTag::positions, notFalse(), {
            must(data_tag, &DataTag::positions, dimEquals(2), "dimensionality of positions DataArray must be two!") }),
        could(data_tag, &DataTag::extents, notFalse(), {
            must(data_tag, &DataTag::extents, dimEquals(2), "dimensionality of positions DataArray must be two!") }),
        // check units for validity
        could(data_tag, &DataTag::units, notEmpty(), {
            must(data_tag, &DataTag::units, isValidUnit(), "Some of the units in tag are invalid: not an atomic SI. Note: So far composite SI units are not supported!") }),
        must(data_tag, &DataTag::references, tagRefsHaveUnits(data_tag.units()), "Some of the referenced DataArrays' dimensions don't have units where the tag has. Make sure that all references have the same number of dimensions as the tag has units and that each dimension has a unit set."),
        must(data_tag, &DataTag::references, tagUnitsMatchRefsUnits(data_tag.units()), "Some of the referenced DataArrays' dimensions have units that are not convertible to the units set in tag. Note: So far composite SI units are not supported!"),
        // check positions & extents
        could(data_tag, &DataTag::extents, notFalse(), {
            must(data_tag, &DataTag::positions, extentsMatchPositions<decltype(data_tag.extents())>(data_tag.extents()), "Number of entries in positions and extents do not match!") }),
        could(data_tag, &DataTag::references, notEmpty(), {
            could(data_tag, &DataTag::extents, notFalse(), {
                must(data_tag, &DataTag::extents, extentsMatchRefs<decltype(data_tag.references())>(data_tag.references()), "number of entries (in 2nd dim) in extents does not match number of dimensions in all referenced DataArrays!") }),
            must(data_tag, &DataTag::positions, positionsMatchRefs<decltype(data_tag.references())>(data_tag.references()), "number of entries (in 2nd dim) in positions does not match number of dimensions in all referenced DataArrays!") })
    });

    return result.concat(result_base);
}

Result validate(const Dimension &dim) {
    return validate({
        must(dim, &Dimension::index, notSmaller(1), "index is not set to valid value (> 0)!")
    });
}

Result validate(const RangeDimension &range_dim) {
    return validate({
        must(range_dim, &RangeDimension::index, notSmaller(1), "index is not set to valid value (size_t > 0)!"),
        must(range_dim, &RangeDimension::ticks, notEmpty(), "ticks are not set!"),
        must(range_dim, &RangeDimension::dimensionType, isEqual<DimensionType>(DimensionType::Range), "dimension type is not correct!"),
        could(range_dim, &RangeDimension::unit, notFalse(), {
            must(range_dim, &RangeDimension::unit, isAtomicUnit(), "Unit is set but not an atomic SI. Note: So far composite units are not supported!") }),
        must(range_dim, &RangeDimension::ticks, isSorted(), "Ticks are not sorted!")
    });
}

Result validate(const SampledDimension &sampled_dim) {
    return validate({
        must(sampled_dim, &SampledDimension::index, notSmaller(1), "index is not set to valid value (size_t > 0)!"),
        must(sampled_dim, &SampledDimension::samplingInterval, isGreater(0), "samplingInterval is not set to valid value (> 0)!"),
        must(sampled_dim, &SampledDimension::dimensionType, isEqual<DimensionType>(DimensionType::Sample), "dimension type is not correct!"),
        could(sampled_dim, &SampledDimension::offset, notFalse(), {
            should(sampled_dim, &SampledDimension::unit, isAtomicUnit(), "offset is set, but no valid unit set!") }),
        could(sampled_dim, &SampledDimension::unit, notFalse(), {
            must(sampled_dim, &SampledDimension::unit, isAtomicUnit(), "Unit is set but not an atomic SI. Note: So far composite units are not supported!") })
    });
}

Result validate(const SetDimension &set_dim) {
    return validate({
        must(set_dim, &SetDimension::index, notSmaller(1), "index is not set to valid value (size_t > 0)!"),
        must(set_dim, &SetDimension::dimensionType, isEqual<DimensionType>(DimensionType::Set), "dimension type is not correct!")
    });
}

Result validate(const Feature &feature) {
    Result result_base = validate(static_cast<base::Entity<base::IFeature>>(feature));
    Result result = validate({
        must(feature, &Feature::data, notFalse(), "data is not set!"),
        must(feature, &Feature::linkType, notSmaller(0), "linkType is not set!")
    });
        
    return result.concat(result_base);
}

Result validate(const Section &section) {
    return validate(static_cast<base::NamedEntity<base::ISection>>(section));
}
    
Result validate(const Source &source) {
    return validate(static_cast<base::EntityWithMetadata<base::ISource>>(source));
}

Result validate(const File &file) {
    Result result = validate({
        could(file, &File::isOpen, notFalse(), {
            must(file, &File::createdAt, notFalse(), "date is not set!"),
            should(file, &File::version, notEmpty(), "version is not set!"),
            should(file, &File::format, notEmpty(), "format is not set!"),
            should(file, &File::location, notEmpty(), "location is not set!") })
    });
        
    // now get all entities from the file: use the multi-getter for each type of entity
    // (the multi-getters use size_t-getter which in the end use H5Lget_name_by_idx
    // to get each file objects name - the count is determined by H5::Group::getNumObjs
    // so that in the end really all file objects are retrieved)
    
    // Blocks
    auto blcks = file.blocks();
    for(auto &block : blcks) {
        result.concat(validate(block));
        // DataArrays
        auto data_arrays = block.dataArrays();
        for(auto &data_array : data_arrays) {
            result.concat(validate(data_array));
            // Dimensions
            auto dims = data_array.dimensions();
            for(auto &dim : dims) {
                if(dim.dimensionType() == DimensionType::Range) {
                    auto d = dim.asRangeDimension();
                    result.concat(validate(d));
                }
                if(dim.dimensionType() == DimensionType::Set) {
                    auto d = dim.asSetDimension();
                    result.concat(validate(d));
                }
                if(dim.dimensionType() == DimensionType::Sample) {
                    auto d = dim.asSampledDimension();
                    result.concat(validate(d));
                }
            }
        }
        // DataTags
        auto data_tags = block.dataTags();
        for(auto &data_tag : data_tags) {
            result.concat(validate(data_tag));
            // Features
            auto features = data_tag.features();
            for(auto &feature : features) {
                result.concat(validate(feature));
            }
        }
        // SimpleTags
        auto simple_tags = block.simpleTags();
        for(auto &simple_tag : simple_tags) {
            result.concat(validate(simple_tag));
            // Features
            auto features = simple_tag.features();
            for(auto &feature : features) {
                result.concat(validate(feature));
            }
        }
        // Sources
        auto sources = block.findSources();
        for(auto &source : sources) {
            result.concat(validate(source));
        }
    }
    // Sections
    auto sections = file.findSections();
    for(auto &section : sections) {
        result.concat(validate(section));
        // Properties
        auto props = section.properties();
        for(auto &prop : props) {
            result.concat(validate(prop));
        }
    }

    return result;
}

} // namespace valid
} // namespace nix
