cmake_minimum_required(VERSION 3.12...3.27.4)

project(chillerbot_reply)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

SET(LIB_SOURCE_FILES
    src/chillerbot_reply/chillerbot_reply.cpp
    src/ddnet/protocol.h
)

add_library(chillerbot_reply STATIC ${LIB_SOURCE_FILES})
add_subdirectory(src/external/ddnet_base)
target_link_libraries(chillerbot_reply PRIVATE ddnet_base)

target_include_directories(chillerbot_reply PRIVATE src)
include_directories(include)

if(TARGET_OS STREQUAL "windows")
    # definitions from ddnet
    # See https://learn.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers
    add_compile_definitions(${target} PRIVATE NOMINMAX) # windows.h shouldn't define min/max macros
    add_compile_definitions(${target} PRIVATE WIN32_LEAN_AND_MEAN) # windows.h shouldn't define the name IStorage
    # 0x0501 (Windows XP) is required for mingw to get getaddrinfo to work
    # 0x0600 (Windows Vista) is required to use RegGetValueW and RegDeleteTreeW
    add_compile_definitions(${target} PRIVATE NTDDI_VERSION=0x06000000) # Minimum OS version (new macro, since Vista)
    add_compile_definitions(${target} PRIVATE _WIN32_WINNT=0x0600) # Minimum OS version (old macro, both must be defined)
    add_compile_definitions(${target} PRIVATE UNICODE) # Windows headers
    add_compile_definitions(${target} PRIVATE _UNICODE) # C-runtime
endif()

# tools

add_executable(chillerbot_reply_cli ${LIB_SOURCE_FILES} src/tools/chillerbot_reply_cli.cpp)
target_link_libraries(chillerbot_reply_cli ddnet_base ${LINK_FLAGS_PLATFORM})

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
    target_link_libraries(${test_target} chillerbot_reply ${TWBL_DEPS} ${LINK_FLAGS_PLATFORM})
    target_include_directories(${test_target} PRIVATE src)
    # target_compile_options(${test_target} PRIVATE -Werror)
    add_test(NAME ${test_target}
             COMMAND ${test_target})
endfunction()

register_test("sample")
register_test("greet")

