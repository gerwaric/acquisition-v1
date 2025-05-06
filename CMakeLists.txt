cmake_minimum_required(VERSION 3.19)

project(acquisition
    VERSION 1.0
    LANGUAGES CXX
    DESCRIPTION "Stash management and forum shop automation for Path of Exile 1 & 2"
    HOMEPAGE_URL "https://github.com/gerwaric/acquisition-v1"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    boost-headers-only
    GIT_REPOSITORY https://github.com/gerwaric/boost-headers-only.git
    GIT_TAG        boost-1.86.0
)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.15.2
)
FetchContent_Declare(
    json_struct
    GIT_REPOSITORY https://github.com/jorgen/json_struct.git
    GIT_TAG        1.0.1
)

FetchContent_MakeAvailable(boost-headers-only json_struct spdlog)

find_package(Qt6 REQUIRED COMPONENTS
    HttpServer
    Network
    Quick
    Sql
    WebSockets
)

# Compiler options must be set before calling the qt project setup.
if(MSVC)
    add_compile_options(/EHsc)
    add_compile_options(/W4)
endif()

qt_standard_project_setup(REQUIRES 6.9)

add_subdirectory(acquisition)
