cmake_minimum_required(VERSION 3.12...3.27.4)

project(chillerbot_reply)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

SET(LIB_SOURCE_FILES
    src/chillerbot_reply/clan.cpp
    src/chillerbot_reply/chillerbot_reply.cpp
    src/chillerbot_reply/check_war_others.cpp
    src/chillerbot_reply/langparser.cpp
    src/chillerbot_reply/text_helper.cpp
    src/polynet/protocol.h
)

add_library(chillerbot_reply ${LIB_SOURCE_FILES})
add_subdirectory(src/external/ddnet_base)

target_include_directories(chillerbot_reply PRIVATE src)
include_directories(include)

set(DBG $<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>)
set(PLATFORM_LIBS)
if(APPLE)
    find_library(CORE_FOUNDATION_LIBRARY CoreFoundation REQUIRED)
    list(APPEND PLATFORM_LIBS ${CORE_FOUNDATION_LIBRARY}) # macOS CFLocaleCopyCurrent()
elseif(WIN32)
    list(APPEND PLATFORM_LIBS ws2_32.lib) # Windows sockets
    list(APPEND PLATFORM_LIBS version) # Windows GetFileVersionInfoSizeW()

    # definitions from ddnet
    # See https://learn.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers
    add_compile_definitions(chillerbot_reply PRIVATE NOMINMAX) # windows.h shouldn't define min/max macros
    add_compile_definitions(chillerbot_reply PRIVATE WIN32_LEAN_AND_MEAN) # windows.h shouldn't define the name IStorage
    # 0x0501 (Windows XP) is required for mingw to get getaddrinfo to work
    # 0x0600 (Windows Vista) is required to use RegGetValueW and RegDeleteTreeW
    add_compile_definitions(chillerbot_reply PRIVATE NTDDI_VERSION=0x06000000) # Minimum OS version (new macro, since Vista)
    add_compile_definitions(chillerbot_reply PRIVATE _WIN32_WINNT=0x0600) # Minimum OS version (old macro, both must be defined)
    add_compile_definitions(chillerbot_reply PRIVATE UNICODE) # Windows headers
    add_compile_definitions(chillerbot_reply PRIVATE _UNICODE) # C-runtime

    if(MSVC)
        # https://github.com/chillerbot/chillerbot_reply/issues/2
        # copied from chillerbot-ux
        # https://github.com/chillerbot/chillerbot-ux/blob/4d2c4173541a1c8a74c9f1fb2c83afbd967815c0/CMakeLists.txt#L812
        # has to be kept in sync
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<${DBG}:Debug>) # Use static CRT
        set_property(TARGET chillerbot_reply PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<${DBG}:Debug>)
    endif()
endif()

target_link_libraries(chillerbot_reply PRIVATE ddnet_base ${PLATFORM_LIBS})

# tools

add_executable(chillerbot_reply_cli ${LIB_SOURCE_FILES} src/tools/chillerbot_reply_cli.cpp)
target_include_directories(chillerbot_reply_cli PRIVATE src)
target_link_libraries(chillerbot_reply_cli ddnet_base ${PLATFORM_LIBS})

# tests

include(CTest)

function(register_test test_name)
    FILE(GLOB TEST_SOURCE_FILES
        src/test/*.cpp
        src/test/*.h
        src/test/${test_name}/*.cpp
        src/test/${test_name}/*.h)
    set(test_target "test_${test_name}")
    add_executable(${test_target} ${TEST_SOURCE_FILES})
    target_link_libraries(${test_target} ddnet_base chillerbot_reply ${PLATFORM_LIBS})
    target_include_directories(${test_target} PRIVATE src)
    # target_compile_options(${test_target} PRIVATE -Werror)
    add_test(NAME ${test_target}
             COMMAND ${test_target})
endfunction()

register_test("sample")
register_test("greet")
register_test("clan")
register_test("war")
register_test("langparser")

