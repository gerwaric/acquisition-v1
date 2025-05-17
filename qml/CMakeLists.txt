qt_add_qml_module(acquisition
    URI Acquisition
    QML_FILES
        Main.qml
)

add_subdirectory(AppWindow)
add_subdirectory(TradeFilters)

# Link QML plugins so we can import modules.
target_link_libraries(acquisition PRIVATE
    AppWindow_plugin
    TradeFilters_plugin
)
