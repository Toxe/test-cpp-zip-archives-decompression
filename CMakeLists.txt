cmake_minimum_required(VERSION 3.14)
project("ZIP Archives Decompression" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_HOST_UNIX)
    set(ENV{PKG_CONFIG_PATH} "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
    find_package(PkgConfig REQUIRED)
endif()

find_package(LibArchive REQUIRED)
find_package(BZip2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(LibXML2 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(LibLZMA CONFIG REQUIRED)
find_package(lz4 CONFIG REQUIRED)
find_package(unofficial-iconv CONFIG REQUIRED)

if(CMAKE_HOST_UNIX)
    pkg_check_modules(LibZip REQUIRED libzip)
    pkg_check_modules(LibLZO REQUIRED lzo2)
    pkg_check_modules(zstd REQUIRED libzstd)

    set(LibZip_LIBRARY "${pkgcfg_lib_LibZip_zip}")
    set(LibLZO_LIBRARY "${pkgcfg_lib_LibLZO_lzo2}")
    set(zstd_LIBRARY "${pkgcfg_lib_zstd_zstd}")
endif()

if(CMAKE_HOST_WIN32)
    find_library(LibZip_LIBRARY zip)
    find_library(LibLZO_LIBRARY lzo2)
    find_library(zstd_LIBRARY zstd)
endif()

set(ALL_TARGETS
    list_files_libarchive
    list_files_libzip
    libzip_textfile_line_reader
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

foreach(target ${ALL_TARGETS})
    add_executable(${target} ${target}.cpp)
    target_compile_features(${target} PUBLIC cxx_std_17)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wmost -pedantic -Wconversion -Wfloat-equal -Wold-style-cast -fcolor-diagnostics)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${target} PRIVATE /W4)
    endif()
endforeach()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(list_files_libarchive PRIVATE -Wno-old-style-cast)
    target_compile_options(list_files_libzip PRIVATE -Wno-nullability-extension)
    target_compile_options(libzip_textfile_line_reader PRIVATE -Wno-nullability-extension)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(list_files_libarchive PRIVATE /wd4706)
    target_compile_options(list_files_libzip PRIVATE /wd4706)
    target_compile_options(libzip_textfile_line_reader PRIVATE /wd4706)
endif()

target_link_libraries(list_files_libarchive PRIVATE ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LibLZO_LIBRARY} ${zstd_LIBRARY} ZLIB::ZLIB lz4::lz4 LibLZMA::LibLZMA OpenSSL::SSL OpenSSL::Crypto BZip2::BZip2 unofficial::iconv::libiconv unofficial::iconv::libcharset)
target_link_libraries(list_files_libzip PRIVATE ${LibZip_LIBRARY} ZLIB::ZLIB BZip2::BZip2)
target_link_libraries(libzip_textfile_line_reader PRIVATE ${LibZip_LIBRARY} ZLIB::ZLIB BZip2::BZip2)
