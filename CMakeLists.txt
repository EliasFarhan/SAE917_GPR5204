cmake_minimum_required(VERSION 3.9)
project(GPR5204)
enable_language(ASM_NASM)

if(UNIX AND NOT APPLE)
    MESSAGE("Linux Compiler ${CMAKE_CXX_COMPILER_ID}")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        MESSAGE("LLVM_ROOT ${LLVM_ROOT}")
        # using Clang

        if(LLVM_ROOT)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --stdlib=libc++ -I${LLVM_ROOT}/include/c++/v1/ -L${LLVM_ROOT}/lib -Wl,-rpath,${LLVM_ROOT}/lib  -lc++ -lc++abi ")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -nostdinc++")

            include_directories(${LLVM_ROOT}/include/c++/v1/)
            LIST(APPEND Neko_LIBRARIES
                    c++fs)
            SET (CMAKE_AR      "${LLVM_ROOT}/bin/llvm-ar")
            SET (CMAKE_LINKER  "${LLVM_ROOT}/bin/llvm-ld")
            SET (CMAKE_NM      "${LLVM_ROOT}/bin/llvm-nm")
            SET (CMAKE_OBJDUMP "${LLVM_ROOT}/bin/llvm-objdump")
            SET (CMAKE_RANLIB  "${LLVM_ROOT}/bin/llvm-ranlib")
                #Google benchmark specifics
            SET(BENCHMARK_USE_LIBCXX ON)
            set(RUN_HAVE_STD_REGEX 0)
            set(RUN_HAVE_POSIX_REGEX 0)
        else()
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
        endif()

    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # using GCC
        LIST(APPEND Neko_LIBRARIES
                stdc++fs)
    endif()


ENDIF()


if(UNIX)
    if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")
        message(" * C++ compiler: Emscripten")
        set(CMAKE_CXX_COMPILER_ID "Emscripten")
    else()
        message(" * C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
    endif()
endif()

SET(BENCHMARK_ENABLE_TESTING OFF CACHE STRING "Description")

#Adding google Benchmark
add_subdirectory(benchmark)
include_directories(benchmark/include)

#Adding ctpl
include_directories(CTPL/)

#source files
include_directories(include/)
file(GLOB_RECURSE SRC_FILES src/*.cpp)

file(GLOB_RECURSE ASM_FILES asm/*.asm)


foreach(asm_file ${ASM_FILES})
    get_filename_component(asm_name ${asm_file} NAME)
    set(output_file ${CMAKE_BINARY_DIR}/asm/${asm_name}.o)
    list(APPEND asm_out ${output_file})
    add_custom_command(OUTPUT ${asm_name}.o COMMAND ${YASM_EXE}
            ARGS -f elf64 -o ${CMAKE_BINARY_DIR}/asm/${asm_name}.o ${asm_file})
endforeach()
add_library(ASM_LIB ${ASM_FILES})
set_target_properties(ASM_LIB PROPERTIES LINKER_LANGUAGE CXX)


SET(COURSE_DIR ${CMAKE_SOURCE_DIR}/tests)
file(GLOB COURSE_FILES ${COURSE_DIR}/*.cpp )

foreach(course_file ${COURSE_FILES} )
    # I used a simple string replace, to cut off .cpp.
    file(RELATIVE_PATH course_relative_path ${COURSE_DIR} ${course_file})
    string( REPLACE ".cpp" "" course_name ${course_relative_path} )
    add_executable(${course_name} ${course_file} ${SRC_FILES} src/job_system.cpp include/job_system.h include/misc.h)
    # Make sure YourLib is linked to each app
    target_link_libraries(${course_name} PUBLIC benchmark ASM_LIB)
    set_property(TARGET ${course_name} PROPERTY CXX_STANDARD 17)


	IF(MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /Oi")
    elseif(UNIX)
    set_target_properties(${course_name} PROPERTIES COMPILE_FLAGS "-save-temps -ffast-math -march=native")
    ENDIF(MSVC)
    set_target_properties (${course_name} PROPERTIES
        FOLDER Courses)
    IF(APPLE)
        set_target_properties(${course_name} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
                RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
    ENDIF()
endforeach()

SET(COURSE_GRAPHICS_DIR ${CMAKE_SOURCE_DIR}/tests_cuda)
file(GLOB COURSE_GRAPHICS_FILES ${COURSE_GRAPHICS_DIR}/*.cu )
foreach(course_file ${COURSE_GRAPHICS_FILES} )
    # I used a simple string replace, to cut off .cpp.
    file(RELATIVE_PATH course_relative_path ${COURSE_GRAPHICS_DIR} ${course_file})
    string( REPLACE ".cu" "" course_name ${course_relative_path} )
    add_executable(${course_name} ${course_file})
    # Make sure YourLib is linked to each app
    target_link_libraries(${course_name} PUBLIC benchmark)
    set_property(TARGET ${course_name} PROPERTY CUDA_SEPARABLE_COMPILATION ON)

    set_target_properties (${course_name} PROPERTIES
            FOLDER Courses)
    IF(APPLE)
        set_target_properties(${course_name} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
                RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
        set_property(TARGET ${course_name}
                PROPERTY
                BUILD_RPATH ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
    ENDIF()
endforeach()

