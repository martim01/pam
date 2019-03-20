cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(lissajou LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(lissajou MODULE ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(lissajou PRIVATE ${flags})
target_compile_definitions(lissajou PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _monitor_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(lissajou ${wxWidgets_LIBRARIES})
target_link_libraries(lissajou pambase)

set_target_properties(lissajou PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor")


		
