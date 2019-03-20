cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(r128 LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories("../../pamlevel/")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(r128 MODULE ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(r128 PRIVATE ${flags})
target_compile_definitions(r128 PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _monitor_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(r128 ${wxWidgets_LIBRARIES})
target_link_libraries(r128 pambase)
target_link_libraries(r128 pamlevel)

set_target_properties(r128 PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor")


		
