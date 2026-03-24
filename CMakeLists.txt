cmake_minimum_required(VERSION 3.21)

project(MathHelperCpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(mathhelper_core
    src/app/session_service.cpp
    src/model/ranking.cpp
    src/quiz/quiz_service.cpp
    src/repository/file_student_repository.cpp
    src/ui/console_ui.cpp
)

target_include_directories(mathhelper_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

function(mathhelper_apply_compile_options target_name)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4 /permissive- /utf-8)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

mathhelper_apply_compile_options(mathhelper_core)

add_executable(mathhelper_cli src/main.cpp)
target_link_libraries(mathhelper_cli PRIVATE mathhelper_core)
mathhelper_apply_compile_options(mathhelper_cli)

option(MATHHELPER_BUILD_TESTS "Build smoke test target" ON)
if(MATHHELPER_BUILD_TESTS)
    add_executable(mathhelper_smoke tests/smoke_main.cpp)
    target_link_libraries(mathhelper_smoke PRIVATE mathhelper_core)
    mathhelper_apply_compile_options(mathhelper_smoke)

    enable_testing()
    add_test(NAME mathhelper_smoke COMMAND mathhelper_smoke)
endif()
