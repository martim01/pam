cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(polarscope LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(polarscope MODULE ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(polarscope PRIVATE ${flags})
target_compile_definitions(polarscope PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _monitor_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(polarscope ${wxWidgets_LIBRARIES})
target_link_libraries(polarscope pambase)

set_target_properties(polarscope PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor")


		
