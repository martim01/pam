cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(identify LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(identify MODULE ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(identify PRIVATE ${flags})
target_compile_definitions(identify PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _test_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(identify ${wxWidgets_LIBRARIES})
target_link_libraries(identify pambase)

set_target_properties(identify PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/test")


		#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS identify LIBRARY DESTINATION /usr/lib/pam2/monitor)
endif()
