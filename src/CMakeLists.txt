target_sources(acquisition PUBLIC
    acquisition/main.cpp
)

target_include_directories(acquisition PRIVATE
    # Most include statements start here, e.g. <acquisition/common/logging.h>
    ${CMAKE_CURRENT_SOURCE_DIR}
    # Anything declared with QML_ELEMENT goes here because Qt
    # needs these classes to be directly on the include path
    # as of Qt 6.9.0.
    ${CMAKE_CURRENT_SOURCE_DIR}/acquisition/qml_elements
)

# The CMakeLists in these subdirectories add source files
# to acquisition. This is attempt to strike a balance
# between listing all the files in a single monolithic
# executable target versus the complexity of setting
# up multiple internal libraries.
add_subdirectory(acquisition/common)
add_subdirectory(acquisition/datastore)
add_subdirectory(acquisition/model)
add_subdirectory(acquisition/oauth)
add_subdirectory(acquisition/poe_api)
add_subdirectory(acquisition/poe_api/website)
add_subdirectory(acquisition/qml_elements)
