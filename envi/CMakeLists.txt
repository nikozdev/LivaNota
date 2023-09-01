cmake_minimum_required(VERSION 3.24)

include(CMakeDependentOption)
include(FetchContent)

# proj

set(vProjName "LivaNota")
set(vProjVnum "0.0")
set(vProjDesc "Life Note")
set(vProjPath "${CMAKE_SOURCE_DIR}/..")

project(${vProjName} VERSION ${vProjVnum} DESCRIPTION ${vProjDesc})

# lang

enable_language(CXX)
set(CXX_STANDARD 23)
set(CXX_STANDARD_REQUIRED TRUE)

# code

set(vCodePath "${vProjPath}/code")
set(vCodeFile "${vCodePath}/${vProjName}.cpp")

# head

set(vHeadPath "${vProjPath}/code")
set(vHeadFile "${vHeadPath}/${vProjName}.hpp")

# make

set(CMAKE_EXPORT_COMPILE_COMMANDS ON FORCE)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "type of build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

set(vMakePath "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${vMakePath}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${vMakePath}")

##exe

set(vTexeName "t${vProjName}Exe")
set(vTexePath "${vMakePath}")
set(vTexeFile ${vTexePath}/${vTexeName})

add_executable(${vTexeName})

target_sources(${vTexeName} PRIVATE ${vCodeFile})
target_include_directories(${vTexeName} PRIVATE ${vHeadPath})
target_precompile_headers(${vTexeName} PRIVATE ${vHeadFile})

# test

include(CTest)
enable_testing()

function(fTest vName vText)
  add_test(${vName} COMMAND ${vTexeFile} "test" "${vName}")
  set_tests_properties(${vName} PROPERTIES PASS_REGULAR_EXPRESSION ${vText})
endfunction(fTest)

fTest("Hello" "HelloWorld")
