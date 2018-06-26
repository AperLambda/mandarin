cmake_minimum_required(VERSION 3.8)
project(mandarin)

option(MANDARIN_INSTALL "Generate installation target" ON)

include(GenerateExportHeader)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)

find_package(lambdacommon REQUIRED)
set(JSON_DIR dependencies/json)
set(FIFO_MAP dependencies/fifo_map)
set(TINY_PROCESS_LIB_DIR dependencies/tiny-process-library)

add_subdirectory(${JSON_DIR})
add_subdirectory(${TINY_PROCESS_LIB_DIR})
include_directories(${LAMBDACOMMON_INCLUDE_DIR} ${FIFO_MAP}/src ${JSON_DIR}/single_include ${TINY_PROCESS_LIB_DIR})

set(DEPENDENCIES_HEADERS_FILES include/args.hxx)

set(MANDARIN_HEADERS_FILES include/mandarin/mandarin.h include/mandarin/package_manager.h include/mandarin/package.h)
set(MANDARIN_SOURCES_FILES src/mandarin.cpp src/package_manager.cpp src/package.cpp)
set(MANDARIN_LIBRARY_FILES ${DEPENDENCIES_HEADERS_FILES} ${MANDARIN_HEADERS_FILES} ${MANDARIN_SOURCES_FILES})

add_library(libmandarin SHARED ${MANDARIN_LIBRARY_FILES})
target_link_libraries(libmandarin ${LAMBDACOMMON_LIBRARIES} nlohmann_json)
set_target_properties(libmandarin PROPERTIES OUTPUT_NAME mandarin)

# Generate the export header and include it.
GENERATE_EXPORT_HEADER(libmandarin
        BASE_NAME mandarin
        EXPORT_MACRO_NAME MANDARIN_API
        EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/mandarin_exports.h
        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)
target_include_directories(libmandarin PUBLIC ${CMAKE_BINARY_DIR}/exports)

include_directories(include)
add_executable(mandarin executable/main.cpp)
target_link_libraries(mandarin libmandarin ${LAMBDACOMMON_LIBRARIES} nlohmann_json tiny-process-library)