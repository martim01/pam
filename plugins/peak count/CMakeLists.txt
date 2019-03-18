cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(peakcount LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(peakcount SHARED ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(peakcount PRIVATE ${flags})
target_compile_definitions(peakcount PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _test_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(peakcount ${wxWidgets_LIBRARIES})
target_link_libraries(peakcount pambase)

set_target_properties(peakcount PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/test")


		
