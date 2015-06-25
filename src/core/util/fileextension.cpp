/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
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

#include <inviwo/core/util/fileextension.h>

#include <inviwo/core/util/singleton.h>
#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

FileExtension::FileExtension()
    : extension_("txt")
    , description_("Textfile") {};

FileExtension::FileExtension(std::string extension, std::string description)
    : extension_(toLower(extension)) // Make sure that the extension is given in lower case 
    , description_(description) {
    
};

FileExtension FileExtension::createFileExtensionFromString(const std::string &str) {
    // try to split extension string
    std::size_t extStart = str.find('(');
    if (extStart == std::string::npos) {
        // could not find extension inside ()
        return{};
    }

    std::size_t extEnd = str.rfind(')');
    if (extEnd == std::string::npos) {
        // not matching ')' for extension string
        return{};
    }
    std::string desc{ str.substr(0, extStart) };
    // trim trailing white spaces
    std::size_t whiteSpacePos = desc.find_last_not_of(" \t\n\r(");
    if (whiteSpacePos != std::string::npos) {
        desc = desc.substr(0, whiteSpacePos + 1);
    }
    else {
        // description only consisted of whitespace characters
        desc.clear();
    }

    std::string ext = toLower(str.substr(extStart + 1, extEnd - extStart - 1));
    // '*.*' should not be used as it is not platform-independent
    // ('*' should be used instead of '*.*')
    
    // special case '*', i.e. '*.*', this should result in an empty string
    if ((ext == "*") || (ext == "*.*")) {
        ext.clear();
    }
    // get rid of '*.'
    if (ext.compare(0, 2, "*.") == 0) {
        ext.erase(0, 2);
    }

    return{ ext, desc };
}

} // namespace

