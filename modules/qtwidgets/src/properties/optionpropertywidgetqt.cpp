/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2022 Inviwo Foundation
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

#include <modules/qtwidgets/properties/optionpropertywidgetqt.h>
#include <modules/qtwidgets/inviwowidgetsqt.h>
#include <modules/qtwidgets/editablelabelqt.h>
#include <modules/qtwidgets/inviwoqtutils.h>

#include <typeinfo>

#include <warn/push>
#include <warn/ignore/all>
#include <QHBoxLayout>
#include <QSignalBlocker>
#include <QMenu>
#include <warn/pop>

namespace inviwo {

OptionPropertyWidgetQt::OptionPropertyWidgetQt(BaseOptionProperty* property)
    : PropertyWidgetQt(property)
    , property_(property)
    , comboBox_{new IvwComboBox(this)}
    , label_{new EditableLabelQt(this, property_)} {

    setFocusPolicy(comboBox_->focusPolicy());
    setFocusProxy(comboBox_);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(7);
    setLayout(hLayout);

    comboBox_->setEnabled(!property_->getReadOnly());
    comboBox_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(comboBox_, &IvwComboBox::customContextMenuRequested, this, [this](const QPoint& pos) {
        if (auto menu = getContextMenu()) {
            menu->exec(comboBox_->mapToGlobal(pos));
        }
    });

    QSizePolicy slidersPol = comboBox_->sizePolicy();
    slidersPol.setHorizontalStretch(3);
    comboBox_->setSizePolicy(slidersPol);

    hLayout->addWidget(label_);

    {
        QWidget* widget = new QWidget(this);
        QSizePolicy sliderPol = widget->sizePolicy();
        sliderPol.setHorizontalStretch(3);
        widget->setSizePolicy(sliderPol);
        QGridLayout* vLayout = new QGridLayout();
        widget->setLayout(vLayout);
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->setSpacing(0);

        vLayout->addWidget(comboBox_);
        hLayout->addWidget(widget);
    }

    connect(comboBox_, static_cast<void (QComboBox::*)(int)>(&IvwComboBox::currentIndexChanged),
            this, &OptionPropertyWidgetQt::optionChanged);

    updateFromProperty();
}

void OptionPropertyWidgetQt::optionChanged(int) {
    if (comboBox_->count() && comboBox_->currentIndex() >= 0 &&
        static_cast<size_t>(comboBox_->currentIndex()) != property_->getSelectedIndex()) {
        property_->setInitiatingWidget(this);
        property_->setSelectedIndex(comboBox_->currentIndex());
        property_->clearInitiatingWidget();
    }
}

void OptionPropertyWidgetQt::updateFromProperty() {
    QSignalBlocker block{comboBox_};

    size_t i = 0;
    for (; i < std::min(property_->size(), static_cast<size_t>(comboBox_->count())); ++i) {
        const auto text = utilqt::toQString(property_->getOptionDisplayName(i));
        if (comboBox_->itemText(static_cast<int>(i)) != text) {
            comboBox_->setItemText(static_cast<int>(i), text);
        }
    }
    for (; i < property_->size(); ++i) {
        comboBox_->addItem(utilqt::toQString(property_->getOptionDisplayName(i)));
    }
    while (static_cast<size_t>(comboBox_->count()) > property_->size()) {
        comboBox_->removeItem(comboBox_->count() - 1);
    }

    comboBox_->setCurrentIndex(static_cast<int>(property_->getSelectedIndex()));
}

}  // namespace inviwo
