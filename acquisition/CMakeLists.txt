qt_add_executable(Acquisition
    src/appconstants.h
    src/appcontroller.cpp
    src/appcontroller.h
    src/datastore/leaguestore.cpp
    src/datastore/leaguestore.h
    src/datastore/userstore.cpp
    src/datastore/userstore.h
    src/footercontroller.cpp
    src/footercontroller.h
    src/main.cpp
    src/model/characternode.cpp
    src/model/characternode.h
    src/model/iteminfo.cpp
    src/model/iteminfo.h
    src/model/itemnode.cpp
    src/model/itemnode.h
    src/model/rootnode.cpp
    src/model/rootnode.h
    src/model/stashnode.cpp
    src/model/stashnode.h
    src/model/treemodel.cpp
    src/model/treemodel.h
    src/model/treenode.cpp
    src/model/treenode.h
    src/oauth/oauthmanager.cpp
    src/oauth/oauthmanager.h
    src/oauth/oauthtoken.cpp
    src/oauth/oauthtoken.h
    src/utils/json.h
    src/utils/json_struct_qt.h
    src/utils/logging.h
    src/utils/rfc2822.cpp
    src/utils/rfc2822.h
)

target_include_directories(Acquisition PRIVATE
    src
)

add_subdirectory(libpoe)

target_link_libraries(Acquisition PRIVATE
    Qt6::HttpServer
    Qt6::Network
    Qt6::Quick
    Qt6::Sql
    Qt6::WebSockets
    json_struct
    spdlog
    libpoe
)

qt_add_qml_module(Acquisition
    URI com.gerwaric.acquisition
    VERSION 1.0
    QML_FILES
        qml/ActionListMenu.qml
        qml/AuthenticationTab.qml
        qml/AutoComplete.qml
        qml/FilterSettingsDialog.qml
        qml/FilterPanel.qml
        qml/Main.qml
        qml/MainMenuBar.qml
        qml/ResultsPanel.qml
        qml/SearchTab.qml
        qml/SearchFilterGroup.qml
        qml/SettingsTab.qml
        qml/TradeFilters/ArmourFilters.qml
        qml/TradeFilters/HeistFilters.qml
        qml/TradeFilters/MapFilters.qml
        qml/TradeFilters/MiscellaneousFilters.qml
        qml/TradeFilters/RequirementFilters.qml
        qml/TradeFilters/SanctumFilters.qml
        qml/TradeFilters/SocketFilters.qml
        qml/TradeFilters/TypeFilters.qml
        qml/TradeFilters/UltimatumFilters.qml
        qml/TradeFilters/WeaponFilters.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(Acquisition PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.gerwaric.acquisition
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Acquisition
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
