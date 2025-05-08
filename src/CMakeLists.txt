target_include_directories(acquisition PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/acquisition/qml_elements
)

add_subdirectory(acquisition)
add_subdirectory(acquisition/common)
add_subdirectory(acquisition/datastore)
add_subdirectory(acquisition/model)
add_subdirectory(acquisition/oauth)
add_subdirectory(acquisition/poe_api)
add_subdirectory(acquisition/poe_api/website)
add_subdirectory(acquisition/qml_elements)
