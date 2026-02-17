cmake_minimum_required(VERSION 3.0)
project(myApp18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ENGINE_LOC "E:/opengl/Bins/glitterEngineBin")

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

add_executable(
myApp18
main.cpp
src/sources/WarriorCharacter.cpp
src/sources/WarriorStatemachine.cpp
src/sources/warriorPlayerController.cpp
src/sources/WarriorAIController.cpp
src/sources/WarriorAICharacter.cpp
)

target_include_directories(myApp18 PRIVATE "${CMAKE_SOURCE_DIR}/src/headers")

list(APPEND CMAKE_PREFIX_PATH ${ENGINE_LOC})
find_package(Glitter REQUIRED CONFIG)

# Link libraries if any
target_link_libraries(myApp18 PRIVATE Glitter::Glitter)

get_target_property(LUA_DLL_PATH Lua::Lua IMPORTED_LOCATION)

# 3. Debugging: This will print the path during configuration
message(STATUS "Lua DLL found at: ${LUA_DLL_PATH}")

if(LUA_DLL_PATH)
    add_custom_command(TARGET myApp18 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LUA_DLL_PATH}"
            "$<TARGET_FILE_DIR:myApp18>"
            COMMENT "Copying Lua DLL to executable directory..."
            VERBATIM
    )
else()
    message(FATAL_ERROR "Could not find Lua DLL path for copying!")
endif()

target_compile_definitions(myApp18  PRIVATE
    GLITTER_ENGINE=\"${ENGINE_LOC}\"
)

# At install time remember to not rely on this. And copy the files to root directory of game.exe.
target_compile_definitions(myApp18  PRIVATE
    CMAKE_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\"
)

get_target_property(_defs myApp18 COMPILE_DEFINITIONS)
message(STATUS "myApp18 COMPILE_DEFINITIONS = ${_defs}")