cmake_minimum_required (VERSION 3.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable (Sysdiff "SysdiffViewer.cpp" "SysdiffViewer.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sysdiff PROPERTY CXX_STANDARD 20)
endif()