cmake_minimum_required(VERSION 3.14)
project("ZIP Archives Decompression" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ENV{PKG_CONFIG_PATH} "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
find_package(PkgConfig REQUIRED)

find_package(LibArchive REQUIRED)
find_package(BZip2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(LibXML2 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(LibLZMA CONFIG REQUIRED)
find_package(lz4 CONFIG REQUIRED)
find_package(unofficial-iconv CONFIG REQUIRED)

pkg_check_modules(LibZip REQUIRED libzip)
pkg_check_modules(LibLZO REQUIRED lzo2)

set(LibZip_LIBRARY "${pkgcfg_lib_LibZip_zip}")
set(LibLZO_LIBRARY "${pkgcfg_lib_LibLZO_lzo2}")

set(ALL_TARGETS
    list_files_libarchive
    list_files_libzip
    libzip_textfile_line_reader
)

foreach(target ${ALL_TARGETS})
    add_executable(${target} ${target}.cpp)
    target_compile_features(${target} PUBLIC cxx_std_17)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wmost -pedantic -Wconversion -Wfloat-equal -Wold-style-cast -fcolor-diagnostics)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
endforeach()

target_compile_options(list_files_libarchive PRIVATE -Wno-old-style-cast)
target_compile_options(list_files_libzip PRIVATE -Wno-nullability-extension)
target_compile_options(libzip_textfile_line_reader PRIVATE -Wno-nullability-extension)

target_include_directories(list_files_libarchive PRIVATE ${LibArchive_INCLUDE_DIRS})
target_include_directories(list_files_libzip PRIVATE ${LibZip_INCLUDE_DIRS})
target_include_directories(libzip_textfile_line_reader PRIVATE ${LibZip_INCLUDE_DIRS})

target_link_libraries(list_files_libarchive PRIVATE ${LibArchive_LIBRARY} ${LIBXML2_LIBRARIES} ${LibLZO_LIBRARY} ZLIB::ZLIB lz4::lz4 LibLZMA::LibLZMA OpenSSL::SSL OpenSSL::Crypto BZip2::BZip2 unofficial::iconv::libiconv unofficial::iconv::libcharset)
target_link_libraries(list_files_libzip PRIVATE ${LibZip_LIBRARY} ZLIB::ZLIB BZip2::BZip2)
target_link_libraries(libzip_textfile_line_reader PRIVATE ${LibZip_LIBRARY} ZLIB::ZLIB BZip2::BZip2)
