include(FetchContent)

if(NOT TARGET boost-headers-only)
    FetchContent_Declare(
        boost-headers-only
        GIT_REPOSITORY https://github.com/gerwaric/boost-headers-only.git
        GIT_TAG        boost-1.88.0
    )
    FetchContent_MakeAvailable(boost-headers-only)
endif()

if(NOT TARGET spdlog)
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.15.3
    )
    FetchContent_MakeAvailable(spdlog)
endif()

if(NOT TARGET json_struct)
    FetchContent_Declare(
        json_struct
        GIT_REPOSITORY https://github.com/jorgen/json_struct.git
        GIT_TAG        1.0.1
    )
    FetchContent_MakeAvailable(json_struct)
endif()

if(NOT TARGET crashpad-binaries)
    FetchContent_Declare(
        crashpad-binaries
        GIT_REPOSITORY https://github.com/gerwaric/crashpad-binaries.git
        GIT_TAG        main
    )
    FetchContent_MakeAvailable(crashpad-binaries)
endif()
