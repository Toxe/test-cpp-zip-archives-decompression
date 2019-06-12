cmake_minimum_required(VERSION 3.14)
project("ZIP Archives Decompression" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(LibArchive)

# Theoretisch sollte es reichen CMake mit -DCMAKE_PREFIX_PATH=$SUITE_BUILD aufzurufen,
# aber aus irgendeinem Grund wird dann die System Version von libarchive.dylib verwendet.
include_directories("$ENV{SUITE_BUILD}/include")

set(LibArchive_LIBRARY "$ENV{SUITE_BUILD}/lib/libarchive.dylib")
set(LibZip_LIBRARY "$ENV{SUITE_BUILD}/lib/libzip.dylib")

add_executable(list_files_libarchive list_files_libarchive.cpp)
add_executable(list_files_libzip list_files_libzip.cpp)

target_compile_features(list_files_libarchive PUBLIC cxx_std_17)
target_compile_features(list_files_libzip PUBLIC cxx_std_17)

target_link_libraries(list_files_libarchive PRIVATE "${LibArchive_LIBRARY}")
target_link_libraries(list_files_libzip PRIVATE "${LibZip_LIBRARY}")

set_target_properties(list_files_libarchive PROPERTIES CXX_EXTENSIONS OFF)
set_target_properties(list_files_libzip PROPERTIES CXX_EXTENSIONS OFF)
