cmake_minimum_required(VERSION 3.9)
project(GPR5204)


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


SET(COURSE_DIR ${CMAKE_SOURCE_DIR}/tests)
file(GLOB COURSE_FILES ${COURSE_DIR}/*.cpp )

foreach(course_file ${COURSE_FILES} )
    # I used a simple string replace, to cut off .cpp.
    file(RELATIVE_PATH course_relative_path ${COURSE_DIR} ${course_file})
    string( REPLACE ".cpp" "" course_name ${course_relative_path} )
    add_executable(${course_name} ${course_file} ${SRC_FILES} src/job_system.cpp include/job_system.h include/misc.h)
    # Make sure YourLib is linked to each app
    target_link_libraries(${course_name} PUBLIC benchmark)
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

