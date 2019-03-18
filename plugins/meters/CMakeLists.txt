cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(meters LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(meters SHARED ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(meters PRIVATE ${flags})
target_compile_definitions(meters PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _monitor_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(meters ${wxWidgets_LIBRARIES})
target_link_libraries(meters pambase)

set_target_properties(meters PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor")


		
