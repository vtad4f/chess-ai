if(CMAKE_MAJOR_VERSION LESS 3)
   cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)
elseif()
   cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
endif()

# set explicity policies
if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
   cmake_policy(SET CMP0054 NEW)
endif(COMMAND cmake_policy)

get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

#prevent in-source build
if("${srcdir}" STREQUAL "${bindir}")
   message(FATAL_ERROR "In-source builds are not allowed.")
endif()

project(cpp-test)

# Get the set of files listed in the additional files text file
set(FILES "")

#for generated file dependencies
add_custom_target(dependencies ALL)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/../additional_files.txt extra_srcs)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/additional_files.txt extra_tests)

add_custom_target(Chess_additional ALL
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../additional_files.txt
            ${CMAKE_CURRENT_SOURCE_DIR}/additional_files.txt)

string(REPLACE "\n" ";" extra_srcs ${extra_srcs})
string(REPLACE "\n" ";" extra_tests ${extra_tests})

foreach(file IN ITEMS ${extra_srcs})
    string(SUBSTRING ${file} 0 1 check)
    if(NOT ${check} STREQUAL "#")
        if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../${file})
            message(FATAL_ERROR "Could not find additional file \"${file}\"!")
        endif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../${file})
        list(APPEND srcs_to_add ${CMAKE_CURRENT_SOURCE_DIR}/../${file})
        message(STATUS ${file})
    endif(NOT ${check} STREQUAL "#")
endforeach(file)

foreach(file IN ITEMS ${extra_tests})
    string(SUBSTRING ${file} 0 1 check)
    if(NOT ${check} STREQUAL "#")
        if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file})
            message(FATAL_ERROR "Could not find additional file \"${file}\"!")
        endif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file})
        list(APPEND tests_to_add ${CMAKE_CURRENT_SOURCE_DIR}/${file})
        message(STATUS ${file})
    endif(NOT ${check} STREQUAL "#")
endforeach(file)

add_dependencies(dependencies Chess_additional)

set(FILES
   ${srcs_to_add}
   ${tests_to_add})

add_executable(cpp-test ${FILES})

add_dependencies(cpp-test dependencies)

if(WIN32 OR MSYS)
   target_link_libraries(cpp-test ws2_32)
endif(WIN32 OR MSYS)

# Warnings
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR
   "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
   set_target_properties(cpp-test PROPERTIES COMPILE_OPTIONS
                         "-Wall" "-Wextra" "-pedantic")
elseif("$CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
   set_target_properties(cpp-test PROPERTIES COMPILE_OPTIONS
                         "/W4")
endif()

#set C++11
if(CMAKE_MAJOR_VERSION LESS 3)
   if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR
      "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      set_target_properties(cpp-test PROPERTIES COMPILE_OPTIONS "-std=c++11")
   endif()
else()
   set_target_properties(cpp-test PROPERTIES CXX_STANDARD 11)
   set_target_properties(cpp-test PROPERTIES CXX_STANDARD_REQUIRED ON)
endif()

