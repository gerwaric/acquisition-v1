add_library(libpoe INTERFACE
    libpoe/account.h
    libpoe/character.h
    libpoe/cruciblenode.h
    libpoe/displaymode.h
    libpoe/eventladderentry.h
    libpoe/frametype.h
    libpoe/guild.h
    libpoe/item.h
    libpoe/itemfilter.h
    libpoe/itemjeweldata.h
    libpoe/itemproperty.h
    libpoe/itemsocket.h
    libpoe/ladderentry.h
    libpoe/league.h
    libpoe/leagueaccount.h
    libpoe/leaguerule.h
    libpoe/passivegroup.h
    libpoe/passivenode.h
    libpoe/publicstashchange.h
    libpoe/pvpladderteamentry.h
    libpoe/pvpladderteammember.h
    libpoe/pvpmatch.h
    libpoe/stashtab.h
    libpoe/website/webcharacter.h
    libpoe/website/webstashtab.h
)

target_include_directories(libpoe INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(libpoe PROPERTIES
    INTERFACE_COMPILE_FEATURES cxx_std_17
    INTERFACE_CXX_EXTENSIONS OFF
)

target_link_libraries(libpoe INTERFACE
    Qt::Core
    json_struct
)
