add_library(libstore STATIC
    libstore/leaguestore.cpp
    libstore/leaguestore.h
    libstore/userstore.cpp
    libstore/userstore.h
)

target_include_directories(libstore PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(libstore PRIVATE
    Qt::Sql
    json_struct
    QsLog
    libpoe
)
