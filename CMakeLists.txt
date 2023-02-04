cmake_minimum_required(VERSION 3.21)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}.")

# Define project
project(
        HotkeysSystem
        VERSION 2.0.1
        DESCRIPTION "A SKSE plugin built with CommonLibSSE."
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

include(GNUInstallDirs)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.rc
        @ONLY)

set(sources
        src/main.cpp
        src/Render.cpp
        src/Gui/GuiMenu.cpp
        src/Gui/Draw.cpp
        src/Gui/Normal.cpp
        src/Gui/Potion.cpp
        src/Gui/Cycle.cpp
        src/Config.cpp
        src/Translate.cpp
        src/Data.cpp
        src/ExtraData.cpp
        src/Actor.cpp
        src/Equipset.cpp
        src/EquipsetManager.cpp
        src/extern/imgui_stdlib.cpp
        src/extern/imgui_impl_dx11.cpp
        src/HUDHandler.cpp
        src/WidgetHandler.cpp
        src/Scaleform/Scaleform.cpp
        src/Scaleform/WidgetMenu.cpp
        src/Offset.h
        src/Utility.h
        src/Serialize.cpp
        src/Event/Input.cpp
        src/Event/Equip.cpp
        src/Event/Combat.cpp
        src/Event/Container.cpp
        src/Equipment.cpp

        ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${sources})

# Configure target DLL
find_package(CommonLibSSE CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${sources})
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_include_directories(${PROJECT_NAME}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
        $<INSTALL_INTERFACE:src>
        ${ARTICUNO_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        imgui::imgui)

target_precompile_headers(${PROJECT_NAME}
        PRIVATE
        src/PCH.h)

install(TARGETS ${PROJECT_NAME}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}")

# Automatic deployment.
foreach(DEPLOY_TARGET $ENV{SKYRIMSE_ROOT})
    message("Adding deployment target ${DEPLOY_TARGET}.")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${DEPLOY_TARGET}/SKSE/Plugins/")
    if(${CMAKE_BUILD_TYPE} STREQUAL Debug OR ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> "${DEPLOY_TARGET}/SKSE/Plugins/")
    endif()
endforeach()
