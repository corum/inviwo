/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2021 Inviwo Foundation
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

#include <modules/basegl/processors/volumeprocessing/volumenormalizationprocessor.h>
#include <inviwo/core/network/networklock.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo VolumeNormalizationProcessor::processorInfo_{
    "org.inviwo.VolumeNormalizationProcessor",  // Class identifier
    "Volume Normalization",                     // Display name
    "Volume Operation",                         // Category
    CodeState::Stable,                          // Code state
    Tags::GL,                                   // Tags
};
const ProcessorInfo VolumeNormalizationProcessor::getProcessorInfo() const {
    return processorInfo_;
}

VolumeNormalizationProcessor::VolumeNormalizationProcessor()
    : Processor()
    , volumeInport_("volumeInport")
    , volumeOutport_("volumeOutport")
    , channels_("channels", "Channels")
    , originalDataRange_{dvec2{0}}
    , originalValueRange_{dvec2{0}}
    , volumeNormalization_([&]() { this->invalidate(InvalidationLevel::InvalidOutput); }) {

    addPorts(volumeInport_, volumeOutport_);
    addProperties(channels_);

    volumeInport_.onChange([this]() {
        if (volumeInport_.hasData()) {
            auto volume = volumeInport_.getData();

            originalDataRange_ = volume->dataMap_.dataRange;
            originalValueRange_ = volume->dataMap_.valueRange;

            const auto channels = static_cast<int>(volume->getDataFormat()->getComponents());
            if (channels == static_cast<int>(channels_.getProperties().size())) return;

            NetworkLock l;

            channels_.clear();
            for (int i = 0; i < channels; i++) {
                const auto numString = std::to_string(i);
                auto prop = new BoolProperty("channel" + numString, "Channel " + numString, true);
                channels_.addProperty(prop);
            }
        }
    });
}

void VolumeNormalizationProcessor::process() {

    auto channelProperties = channels_.getProperties();

    bool apply = false;
    for (size_t i{0}; i < channelProperties.size(); ++i) {
        apply = apply || dynamic_cast<BoolProperty*>(channelProperties[i])->get();
    }

    if (!apply) {
        volumeOutport_.setData(volumeInport_.getData());
    } else {
        for (size_t i{0}; i < channelProperties.size(); ++i) {
            volumeNormalization_.setNormalizeChannel(
                i, dynamic_cast<BoolProperty*>(channelProperties[i])->get());
        }

        volumeOutport_.setData(volumeNormalization_.normalize(*volumeInport_.getData()));
    }
}

}  // namespace inviwo
