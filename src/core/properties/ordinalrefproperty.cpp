/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2018-2022 Inviwo Foundation
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

#include <inviwo/core/properties/ordinalrefproperty.h>

namespace inviwo {

// Scalar properties
template class IVW_CORE_TMPL_INST OrdinalRefProperty<float>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<int>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<size_t>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<glm::i64>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<double>;

// Vector properties
template class IVW_CORE_TMPL_INST OrdinalRefProperty<vec2>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<vec3>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<vec4>;

template class IVW_CORE_TMPL_INST OrdinalRefProperty<dvec2>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<dvec3>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<dvec4>;

template class IVW_CORE_TMPL_INST OrdinalRefProperty<ivec2>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<ivec3>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<ivec4>;

template class IVW_CORE_TMPL_INST OrdinalRefProperty<size2_t>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<size3_t>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<size4_t>;

// Matrix properties
template class IVW_CORE_TMPL_INST OrdinalRefProperty<mat2>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<mat3>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<mat4>;

template class IVW_CORE_TMPL_INST OrdinalRefProperty<dmat2>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<dmat3>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<dmat4>;

template class IVW_CORE_TMPL_INST OrdinalRefProperty<glm::dquat>;
template class IVW_CORE_TMPL_INST OrdinalRefProperty<glm::fquat>;

OrdinalRefPropertyState<vec4> util::ordinalRefColor(InvalidationLevel invalidationLevel) {
    return {vec4{0.0f},
            ConstraintBehavior::Immutable,
            vec4{1.0f},
            ConstraintBehavior::Immutable,
            vec4{0.01f},
            invalidationLevel,
            PropertySemantics::Color};
}

}  // namespace inviwo
