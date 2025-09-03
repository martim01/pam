cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(anglemeters LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(anglemeters SHARED ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(anglemeters PRIVATE ${flags})
target_compile_definitions(anglemeters PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _monitor_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(anglemeters ${wxWidgets_LIBRARIES})
target_link_libraries(anglemeters pambase)

set_target_properties(anglemeters PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../lib/monitor")


		
