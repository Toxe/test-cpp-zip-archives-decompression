cmake_minimum_required(VERSION 3.14)
project("libarchive Tests" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Theoretisch sollte es reichen CMake mit -DCMAKE_PREFIX_PATH=$SUITE_BUILD aufzurufen,
# aber aus irgendeinem Grund wird dann die System Version von libarchive.dylib verwendet.
set(LibArchive_INCLUDE_DIR "$ENV{SUITE_BUILD}/include")
set(LibArchive_LIBRARY     "$ENV{SUITE_BUILD}/lib/libarchive.dylib")
# find_package(LibArchive)

add_executable(list_files_libarchive list_files_libarchive.cpp)
target_compile_features(list_files_libarchive PUBLIC cxx_std_17)
target_include_directories(list_files_libarchive PRIVATE "${LibArchive_INCLUDE_DIR}")
target_link_libraries(list_files_libarchive PRIVATE "${LibArchive_LIBRARY}")
set_target_properties(list_files_libarchive PROPERTIES CXX_EXTENSIONS OFF)
