/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2022 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#pragma once

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/util/glmvec.h>
#include <inviwo/core/util/glmutils.h>

#include <inviwo/core/datastructures/unitsystem.h>
#include <inviwo/core/algorithm/linearmap.h>

namespace inviwo {

class DataFormatBase;

/**
 * \brief Map values into data or value ranges.
 * Data range refers to the range of the data type, i.e. [0 4095] for 12-bit unsigned integer data.
 * Value range refers to the physical meaning of the value, i.e. Hounsfield value range is from
 * [-1000 3000].
 * It is important that the data range is specified for data types with a large range (for example
 * 32/64-bit float and integer) since the data is often normalized to [0 1], when for example
 * performing color mapping, i.e. applying a transfer function.
 */
class IVW_CORE_API DataMapper {
public:
    DataMapper();
    DataMapper(const DataFormatBase* format);
    DataMapper(const DataMapper& rhs);
    DataMapper& operator=(const DataMapper& that);

    /**
     * The ´dataRange´ is used to normalize the "raw" values to [0,1]
     * Typically the minimum and maximum of the "raw" data is used. For 8 and 16 integer data that
     * is often the same or close to the same as the minimum and maximum of the data type. For data
     * where the minimum and maximum is far from the minimum and maximum of the data type, the
     * minimum and maximum has to either be calculated from the data, read from the input, or
     * entered by the user.
     */
    dvec2 dataRange;

    /**
     * The `valueRange` corresponds to the "true" values in the "raw" data. The "raw" data will
     * first be normalized to [0,1] using the `dataRange` and mapped to the "true" range using the
     * `valueRange`. An example might be helpful: if water temperature values in the range of
     * [0.0, 30.0] °C are stored as 8 bit unsigned int, using the full data range from [0,255]. Then
     * the `dataRange` would be [0, 255] and the `valueRange` [0.0, 30.0].
     * Using the same values for `dataRange` and `valueRange` means that the original "raw" values
     * will be used.
     */
    dvec2 valueRange;

    /**
     * The `valueAxis` holds the `name` and `unit` of the quantity. Following the example above
     * the `name` would be "water temperature" and `unit` "°C".
     */
    Axis valueAxis;  ///< Name and Unit, i.e. "absorption", "Hounsfield".

    void initWithFormat(const DataFormatBase* format);

    /**
     * Map from `dataRange` to the `valueRange`
     */
    template <typename T>
    util::same_extent_t<T, double> mapFromDataToValue(T val) const {
        return util::linearMap(static_cast<util::same_extent_t<T, double>>(val), dataRange,
                               valueRange);
    }

    /**
     * Map from `valueRange` to the `dataRange`
     */
    template <typename T>
    util::same_extent_t<T, double> mapFromValueToData(T val) const {
        return util::linearMap(static_cast<util::same_extent_t<T, double>>(val), valueRange,
                               dataRange);
    }

    /**
     * Map from `dataRange` to the Normalized range [0,1]
     */
    template <typename T>
    util::same_extent_t<T, double> mapFromDataToNormalized(T val) const {
        return util::linearMapToNormalized(static_cast<util::same_extent_t<T, double>>(val),
                                           dataRange);
    }

    /**
     * Map from Normalized range [0,1] to the `dataRange`
     */
    template <typename T>
    util::same_extent_t<T, double> mapFromNormalizedToData(T val) const {
        return util::linearMapFromNormalized(static_cast<util::same_extent_t<T, double>>(val),
                                             dataRange);
    }

    /**
     * Map from `valueRange` to the Normalized range [0,1]
     */
    template <typename T>
    util::same_extent_t<T, double> mapFromValueToNormalized(T val) const {
        return util::linearMapToNormalized(static_cast<util::same_extent_t<T, double>>(val),
                                           valueRange);
    }

    /**
     * Map from Normalized range [0,1] to the `valueRange`
     */
    template <typename T>
    util::same_extent_t<T, double> mapFromNormalizedToValue(T val) const {
        return util::linearMapFromNormalized(static_cast<util::same_extent_t<T, double>>(val),
                                             valueRange);
    }
};

}  // namespace inviwo
