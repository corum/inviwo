# Create python module
set(HEADER_FILES
    include/inviwopy/inviwopy.h
    include/inviwopy/pybuffer.h
    include/inviwopy/pybitset.h
    include/inviwopy/pycamera.h
    include/inviwopy/pycameraproperty.h
    include/inviwopy/pycompositeproperties.h
    include/inviwopy/pydataformat.h
    include/inviwopy/pydatamapper.h
    include/inviwopy/pydatareaders.h
    include/inviwopy/pydatawriters.h
    include/inviwopy/pydocument.h
    include/inviwopy/pyevent.h
    include/inviwopy/pyflags.h
    include/inviwopy/pyglmmattypes.h
    include/inviwopy/pyglmports.h
    include/inviwopy/pyglmtypes.h
    include/inviwopy/pyimage.h
    include/inviwopy/pyinviwoapplication.h
    include/inviwopy/pyinviwomodule.h
    include/inviwopy/pylogging.h
    include/inviwopy/pymesh.h
    include/inviwopy/pynetwork.h
    include/inviwopy/pypickingmapper.h
    include/inviwopy/pyport.h
    include/inviwopy/pyprocessors.h
    include/inviwopy/pyproperties.h
    include/inviwopy/pypropertyowner.h
    include/inviwopy/pypropertytypehook.h
    include/inviwopy/pytfprimitiveset.h
    include/inviwopy/pyvolume.h
    include/inviwopy/vectoridentifierwrapper.h
)
ivw_group("Header Files" BASE ${CMAKE_CURRENT_SOURCE_DIR}/include/inviwopy ${HEADER_FILES})

set(SOURCE_FILES
    src/inviwopy.cpp
    src/pybuffer.cpp
    src/pybitset.cpp
    src/pycamera.cpp
    src/pycameraproperty.cpp
    src/pycompositeproperties.cpp
    src/pydataformat.cpp
    src/pydatamapper.cpp
    src/pydocument.cpp
    src/pydatareaders.cpp
    src/pydatawriters.cpp
    src/pyevent.cpp
    src/pyflags.cpp
    src/pyglmmattypes.cpp
    src/pyglmports.cpp
    src/pyglmtypes.cpp
    src/pyimage.cpp
    src/pyinviwoapplication.cpp
    src/pyinviwomodule.cpp
    src/pylogging.cpp
    src/pymesh.cpp
    src/pynetwork.cpp
    src/pypickingmapper.cpp
    src/pyport.cpp
    src/pyprocessors.cpp
    src/pyproperties.cpp
    src/pypropertyowner.cpp
    src/pypropertytypehook.cpp
    src/pytfprimitiveset.cpp
    src/pyvolume.cpp
    src/vectoridentifierwrapper.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

ivw_add_py_wrapper(inviwopy ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(inviwopy PUBLIC inviwo::core inviwo::module::python3)
target_include_directories(inviwopy PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

ivw_default_install_targets(inviwopy)
