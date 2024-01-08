cmake_minimum_required (VERSION 3.8)

set(BOOST_ROOT "C:/local/boost_1_82_0")
# find_package(Boost 1.82.0 COMPONENTS filesystem)
find_package(Boost 1.50 REQUIRED COMPONENTS filesystem)

set(OPENSSL_USE_STATIC_LIBS TRUE)

find_package(OpenSSL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${OPENSSL_INSTALL_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS}) 
include_directories(${OPENSSL_INCLUDE_DIR})

add_executable (Sysdiff "Sysdiff.cpp" "Sysdiff.h"  "FileMap.h")
target_link_libraries(Sysdiff Boost::filesystem)

target_link_libraries(Sysdiff OpenSSL::SSL OpenSSL::Crypto)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sysdiff PROPERTY CXX_STANDARD 20)
endif()