/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2023 Inviwo Foundation
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

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/factoryutil.h>
#include <inviwo/core/common/factoryutil.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/io/datareaderfactory.h>
#include <inviwo/core/util/filesystem.h>
#include <inviwo/core/io/datareaderexception.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/util/fileextension.h>

namespace inviwo {

namespace util {
inline void updateReaderFromFile(const FileProperty& file,
                                 OptionProperty<FileExtension>& extReader) {
    if ((file.getSelectedExtension() == FileExtension::all() &&
         !extReader.getSelectedValue().matches(file)) ||
        file.getSelectedExtension().empty()) {
        const auto& opts = extReader.getOptions();
        const auto it = std::find_if(opts.begin(), opts.end(),
                                     [&](const OptionPropertyOption<FileExtension>& opt) {
                                         return opt.value_.matches(file.get());
                                     });
        extReader.setSelectedValue(it != opts.end() ? it->value_ : FileExtension{});
    } else {
        extReader.setSelectedValue(file.getSelectedExtension());
    }
}

template <typename... Types>
void updateFilenameFilters(const DataReaderFactory& rf, FileProperty& file,
                           OptionProperty<FileExtension>& extReader) {
    std::vector<FileExtension> extensions;

    util::append(extensions, rf.getExtensionsForType<Types>()...);

    std::sort(extensions.begin(), extensions.end());

    std::vector<OptionPropertyOption<FileExtension>> options;
    std::transform(extensions.begin(), extensions.end(), std::back_inserter(options), [](auto& fe) {
        return OptionPropertyOption<FileExtension>{fe.toString(), fe.toString(), fe};
    });

    options.emplace_back("noreader", "No available reader", FileExtension{});

    file.clearNameFilters();
    file.addNameFilter(FileExtension::all());
    file.addNameFilters(extensions);
    extReader.replaceOptions(options);
}

}  // namespace util

/**
 * A base class for simple source processors.
 * Two functions to customize the behavior are available, dataLoaded and dataDeserialized.
 */
template <typename DataType, typename PortType>
class DataSource : public Processor {
public:
    /**
     * Construct a DataSource
     * @param app An InviwoApplication.
     * @param file A filename passed into the FileProperty
     * @param contentType A content type passed into the FileProperty, usually 'volume', 'image',
     * 'geometry', etc.
     * @see FileProperty
     */
    DataSource(DataReaderFactory* rf = util::getDataReaderFactory(), std::string_view filename = "",
               std::string_view contentType = FileProperty::defaultContentType);
    virtual ~DataSource() = default;

    virtual void process() override;
    virtual void deserialize(Deserializer& d) override;

    FileProperty file;
    OptionProperty<FileExtension> extReader;
    ButtonProperty reload;

protected:
    void load(bool deserialized);

    // Called when we load new data.
    virtual void dataLoaded(std::shared_ptr<DataType> data){};
    // Called when we deserialized old data.
    virtual void dataDeserialized(std::shared_ptr<DataType> data){};

    DataReaderFactory* rf_;
    PortType port_;
    std::shared_ptr<DataType> loadedData_;
    bool loadingFailed_ = false;

private:
    bool deserialized_ = false;
};

template <typename DataType, typename PortType>
DataSource<DataType, PortType>::DataSource(DataReaderFactory* rf, std::string_view filename,
                                           std::string_view content)
    : Processor()
    , file{"filename", "File", filename, content}
    , extReader{"reader", "Data Reader"}
    , reload{"reload", "Reload data",
             [this]() {
                 loadingFailed_ = false;
                 isReady_.update();
             }}
    , rf_{rf}
    , port_{"data"} {

    addPort(port_);
    addProperties(file, extReader, reload);

    // ensure that the file name is always serialized by setting the default to ""
    if (!filename.empty()) {
        Property::setStateAsDefault(file, std::string{});
    }

    util::updateFilenameFilters<DataType>(*rf_, file, extReader);
    util::updateReaderFromFile(file, extReader);

    // make sure that we always process even if not connected
    isSink_.setUpdate([]() { return true; });
    isReady_.setUpdate([this]() {
        return !loadingFailed_ && filesystem::fileExists(file.get()) &&
               !extReader.getSelectedValue().empty();
    });
    file.onChange([this]() {
        loadingFailed_ = false;
        util::updateReaderFromFile(file, extReader);
        isReady_.update();
    });
    extReader.onChange([this]() {
        loadingFailed_ = false;
        isReady_.update();
    });
}

template <typename DataType, typename PortType>
void DataSource<DataType, PortType>::process() {
    if (file.isModified() || reload.isModified()) {
        load(deserialized_);
        deserialized_ = false;
    }
}

template <typename DataType, typename PortType>
void DataSource<DataType, PortType>::load(bool deserialized) {
    if (file.get().empty()) return;

    const auto sext = extReader.getSelectedValue();
    if (auto reader = rf_->template getReaderForTypeAndExtension<DataType>(sext, file.get())) {
        try {
            auto data = reader->readData(file.get());
            port_.setData(data);
            loadedData_ = data;
            if (deserialized) {
                dataDeserialized(data);
            } else {
                dataLoaded(data);
            }
        } catch (DataReaderException const& e) {
            loadingFailed_ = true;
            port_.detachData();
            isReady_.update();
            LogProcessorError("Could not load data: " << file.get() << ", " << e.getMessage());
        }
    } else {
        loadingFailed_ = true;
        port_.detachData();
        isReady_.update();
        LogProcessorError("Could not find a data reader for file: " << file.get());
    }
}

template <typename DataType, typename PortType>
void DataSource<DataType, PortType>::deserialize(Deserializer& d) {
    Processor::deserialize(d);
    util::updateFilenameFilters<DataType>(*rf_, file, extReader);
    deserialized_ = true;
}

}  // namespace inviwo
