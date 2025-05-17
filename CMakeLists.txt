cmake_minimum_required(VERSION 3.22)

project(acquisition
    VERSION 1.0
    LANGUAGES CXX
    DESCRIPTION "Stash management and forum shop automation for Path of Exile 1 & 2"
    HOMEPAGE_URL "https://github.com/gerwaric/acquisition-v1"
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler options must be set before calling the qt project setup.
if(MSVC)
    add_compile_options(/EHsc)
    add_compile_options(/W4)
endif()

include(cmake/FetchDependencies.cmake)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    HttpServer
    Network
    Quick
    Sql
    WebSockets
)

qt_standard_project_setup(REQUIRES 6.9)

# Define the executable target.
qt_add_executable(acquisition WIN32 MACOSX_BUNDLE)

# Add source files to the executable target.
add_subdirectory(src)

# Define the main QML module.
qt_add_qml_module(acquisition
    URI Acquisition
    QML_FILES
        qml/Main.qml
)

# Include other QML modules so we can link and import them.
add_subdirectory(qml/AppWindow)
add_subdirectory(qml/TradeFilters)

target_link_libraries(acquisition PRIVATE
    # Qt libraries
    Qt6::HttpServer
    Qt6::Network
    Qt6::Quick
    Qt6::Sql
    Qt6::WebSockets
    # Third-party libraries
    boost-headers-only
    crashpad
    json_struct
    spdlog
    # QML modules
    AppWindow_plugin
    TradeFilters_plugin
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(acquisition PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.gerwaric.acquisition
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS acquisition
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
