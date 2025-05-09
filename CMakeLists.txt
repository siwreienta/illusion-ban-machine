cmake_minimum_required(VERSION 3.12...3.31)

project(apotheosis CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_SERVER "Build the server target" ON)
option(BUILD_ALGORITHM "Build the algorithm target" ON)
option(BUILD_DEBUG "Debug mode" ON)

if(BUILD_DEBUG)
    add_definitions(-DAPOTHEOSIS_DEBUG)
endif()

include(default-options.cmake)

set(ALGORITHM_EXEC
    algorithm/graph-handler.cpp
    algorithm/apotheosis.cpp
    algorithm/VF2.cpp
)

if(BUILD_ALGORITHM)
    add_executable(${PROJECT_NAME}_algorithm 
        algorithm/main.cpp
        ${ALGORITHM_EXEC}
    )
    set_target_properties(${PROJECT_NAME}_algorithm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

if(BUILD_SERVER)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/server/cmake")
    include(DownloadUserver)
    find_package(userver COMPONENTS core postgresql QUIET)
    if(NOT userver_FOUND)
        download_userver(TRY_DIR third_party/userver)
    endif()
    userver_setup_environment()

    add_library(${PROJECT_NAME}_objs OBJECT
        server/src/server.hpp
        server/src/server.cpp
    )
    target_link_libraries(${PROJECT_NAME}_objs PUBLIC 
        userver::core 
        userver::postgresql
    )

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(RE2 REQUIRED libre2)
    pkg_check_modules(PQXX REQUIRED libpqxx)

    add_executable(${PROJECT_NAME}_server
        server/src/main.cpp
        joern_parse/input-parcer.cpp
        ${ALGORITHM_EXEC}
        database/database.cpp
    )
    
    target_include_directories(${PROJECT_NAME}_server PRIVATE 
        ${RE2_INCLUDE_DIRS}
        ${PQXX_INCLUDE_DIRS}
    )
    target_link_directories(${PROJECT_NAME}_server PRIVATE 
        ${RE2_LIBRARY_DIRS}
        ${PQXX_LIBRARY_DIRS}
    )
    target_link_libraries(${PROJECT_NAME}_server PRIVATE 
        ${PROJECT_NAME}_objs
        ${RE2_LIBRARIES}
        ${PQXX_LIBRARIES}
        icuuc
    )

    include(GNUInstallDirs)
    if(DEFINED ENV{PREFIX})
        message(STATUS "Set install prefix: $ENV{PREFIX}")
        file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
        set(CMAKE_INSTALL_PREFIX "${PREFIX_PATH}")
    endif()
    file(GLOB CONFIGS_FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml 
        ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json
    )
    install(TARGETS ${PROJECT_NAME}_server 
        DESTINATION ${CMAKE_INSTALL_BINDIR} 
        COMPONENT ${PROJECT_NAME}_server
    )
    install(FILES ${CONFIGS_FILES} 
        DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}_server 
        COMPONENT ${PROJECT_NAME}_server
    )
endif()
