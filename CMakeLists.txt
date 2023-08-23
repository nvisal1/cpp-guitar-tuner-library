cmake_minimum_required(VERSION 3.18)
project(tuner)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

function (download_dependencies)
    FetchContent_Declare(
            kissfft
            GIT_REPOSITORY https://github.com/mborgerding/kissfft.git
    )

    FetchContent_GetProperties(kissfft)

    if(NOT kissfft_POPULATED)
        FetchContent_Populate(kissfft)
        add_subdirectory(${kissfft_SOURCE_DIR} ${kissfft_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endfunction()

function (build_wasm_executable)
    target_compile_definitions(kissfft PRIVATE -DKISSFFT_TEST=OFF -DKISSFFT_TOOLS=OFF)

    add_executable(
            wasm_tuner
            tuner/tuner.hpp
            tuner/tuner.cpp
            tuner/global.hpp
            tuner/dsp.cpp
            tuner/dsp.hpp
            tuner/math.cpp
            tuner/math.hpp
            tuner/vector.cpp
            tuner/vector.hpp
            tuner/note.cpp
            tuner/note.hpp
    )

    target_include_directories(
            wasm_tuner
            PUBLIC
            /usr/include
            /usr/local/include
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${kissfft_SOURCE_DIR}
    )

    target_link_options(wasm_tuner PRIVATE
            -sEXPORTED_RUNTIME_METHODS=['ccall']
            -sEXPORTED_FUNCTIONS=['_get_pitch','_push_value','_clear_buffer_offset']
            -sINITIAL_MEMORY=1024mb
            -sTOTAL_STACK=512mb)


    target_link_libraries(wasm_tuner kissfft::kissfft)
endfunction()

function (build_library)
    target_compile_definitions(kissfft PRIVATE -DKISSFFT_TEST=OFF -DKISSFFT_TOOLS=OFF)

    add_library(
            tuner
            tuner/tuner.hpp
            tuner/tuner.cpp
            tuner/global.hpp
            tuner/dsp.cpp
            tuner/dsp.hpp
            tuner/math.cpp
            tuner/math.hpp
            tuner/vector.cpp
            tuner/vector.hpp
            tuner/note.cpp
            tuner/note.hpp
    )

    target_include_directories(
            tuner
            PUBLIC
            /usr/include
            /usr/local/include
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${kissfft_SOURCE_DIR}
    )

    target_link_libraries(tuner kissfft::kissfft)
endfunction()

function (build_unit_test)
    set(CMAKE_CXX_FLAGS "-O0 -coverage")
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.0.1
    )

    FetchContent_MakeAvailable(Catch2)

    target_compile_definitions(kissfft PRIVATE -DKISSFFT_TEST=OFF -DKISSFFT_TOOLS=OFF)

    add_executable(
            unit_test

            tuner/math.cpp
            tuner/math.hpp
            tuner/math.test.cpp

            tuner/dsp.cpp
            tuner/dsp.hpp
            tuner/dsp.test.cpp

            tuner/vector.cpp
            tuner/vector.hpp
            tuner/vector.test.cpp

            tuner/note.cpp
            tuner/note.hpp
            tuner/note.test.cpp
    )

    target_include_directories(
            unit_test
            PUBLIC
            /usr/include
            /usr/local/include
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${kissfft_SOURCE_DIR}
            ${Catch2_SOURCE_DIR}/src/catch2
    )

    target_link_libraries(
            unit_test
            kissfft::kissfft
            Catch2::Catch2WithMain
    )
endfunction()

function (build_acceptance_test)
    set(CMAKE_CXX_FLAGS "-O0 -coverage")
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.0.1
    )

    FetchContent_MakeAvailable(Catch2)

    target_compile_definitions(kissfft PRIVATE -DKISSFFT_TEST=OFF -DKISSFFT_TOOLS=OFF)

    add_executable(
            acceptance_test

            tuner/math.cpp
            tuner/math.hpp

            tuner/dsp.cpp
            tuner/dsp.hpp

            tuner/vector.cpp
            tuner/vector.hpp

            tuner/note.cpp
            tuner/note.hpp

            tuner/wa_tuner.cpp
            tuner/wa_tuner.hpp

            tuner/tuner.cpp
            tuner/tuner.cpp
            tuner/tuner.acceptance_test.cpp
    )

    target_include_directories(
            acceptance_test
            PUBLIC
            /usr/include
            /usr/local/include
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${kissfft_SOURCE_DIR}
            ${Catch2_SOURCE_DIR}/src/catch2
    )

    target_link_libraries(
            acceptance_test
            kissfft::kissfft
            Catch2::Catch2WithMain
    )
endfunction()

function (add_coverage)
    find_program(GEN genhtml REQUIRED)
    find_program(LCOV lcov REQUIRED)

    message(${CMAKE_BINARY_DIR})
    add_custom_target(coverage
            COMMAND ${LCOV} --directory . --capture --output-file coverage.info
            COMMAND ${LCOV} --remove coverage.info -o coverage_filtered.info '/Applications/*' '*/_deps/*' '*.hpp'
            COMMAND ${GEN} --demangle-cpp -o coverage coverage_filtered.info
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            )
endfunction()

download_dependencies()
build_wasm_executable()
build_library()
build_unit_test()
build_acceptance_test()
add_coverage()

