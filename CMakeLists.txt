cmake_minimum_required(VERSION 3.26)
project(EngineTechDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set path to engine install directory
set(ENGINE_LOC "")

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

add_executable(
EngineTechDemo
main.cpp
src/sources/WarriorAICharacter.cpp
src/sources/WarriorAIController.cpp
src/sources/WarriorCharacter.cpp
src/sources/WarriorPlayerController.cpp
src/sources/WarriorStatemachine.cpp
)

target_include_directories(EngineTechDemo PRIVATE "${CMAKE_SOURCE_DIR}/src/headers")

list(APPEND CMAKE_PREFIX_PATH ${ENGINE_LOC})
find_package(Glitter REQUIRED CONFIG)

# Link libraries if any
target_link_libraries(EngineTechDemo PRIVATE Glitter::Glitter)

get_target_property(LUA_DLL_PATH Lua::Lua IMPORTED_LOCATION)

# 3. Debugging: This will print the path during configuration
message(STATUS "Lua DLL found at: ${LUA_DLL_PATH}")

if(LUA_DLL_PATH)
    add_custom_command(TARGET EngineTechDemo POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LUA_DLL_PATH}"
            "$<TARGET_FILE_DIR:EngineTechDemo>"
            COMMENT "Copying Lua DLL to executable directory..."
            VERBATIM
    )
else()
    message(FATAL_ERROR "Could not find Lua DLL path for copying!")
endif()

target_compile_definitions(EngineTechDemo  PRIVATE
    GLITTER_ENGINE=\"${ENGINE_LOC}/bin/\"
)

# At install time remember to not rely on this. And copy the files to root directory of game.exe.
target_compile_definitions(EngineTechDemo  PRIVATE
    CMAKE_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\"
)

get_target_property(_defs EngineTechDemo COMPILE_DEFINITIONS)
message(STATUS "EngineTechDemo COMPILE_DEFINITIONS = ${_defs}")


# ============================================================
# INSTALL RULES
# ============================================================

include(GNUInstallDirs)

install(TARGETS EngineTechDemo
    RUNTIME_DEPENDENCY_SET EngineTechDemo_runtime_deps
    RUNTIME DESTINATION .
)

install(RUNTIME_DEPENDENCY_SET EngineTechDemo_runtime_deps
    DESTINATION .
    PRE_EXCLUDE_REGEXES
        "api-ms-.*"
        "ext-ms-.*"
        "azureattestmanager\\.dll"
        "azureattestnormal\\.dll"
        "hvsifiletrust\\.dll"
        "pdmutilities\\.dll"
        "wpaxholder\\.dll"
    POST_EXCLUDE_REGEXES
        ".*/System32/.*"
        ".*/Windows/.*"
)
# ============================================================
# INSTALL GAME / ENGINE DATA
# ============================================================
# Adjust these folders to match what your engine actually needs at runtime.

if(EXISTS "${CMAKE_SOURCE_DIR}/Assets")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/Assets"
        DESTINATION .
    )
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/Levels")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/Levels"
        DESTINATION .
    )
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/Project.manifest.json")
    install(FILES "${CMAKE_SOURCE_DIR}/Project.manifest.json"
        DESTINATION .
    )
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/Shaders")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/Shaders"
        DESTINATION .
    )
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/Config")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/Config"
        DESTINATION .
    )
endif()

# Engine-provided runtime folders
if(EXISTS "${ENGINE_LOC}/bin/EngineAssets")
    install(DIRECTORY "${ENGINE_LOC}/bin/EngineAssets"
        DESTINATION .
    )
endif()

if(EXISTS "${ENGINE_LOC}/bin/Shaders")
    install(DIRECTORY "${ENGINE_LOC}/bin/Shaders"
        DESTINATION .
    )
endif()

# ============================================================
# OPTIONAL: PACKAGE OUTPUT FOLDER NAME
# ============================================================
set(CPACK_PACKAGE_NAME "EngineTechDemo")
set(CPACK_PACKAGE_VENDOR "Subham")
set(CPACK_PACKAGE_VERSION "0.01")
include(CPack)
