cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(InputAlign LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(InputAlign MODULE ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(InputAlign PRIVATE ${flags})
target_compile_definitions(InputAlign PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _test_ _MSL_STDINT_H wxDEBUG_LEVEL=0)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(InputAlign ${wxWidgets_LIBRARIES})
target_link_libraries(InputAlign pambase)

set_target_properties(InputAlign PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/test")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS InputAlign LIBRARY DESTINATION /usr/local/lib/pam2/test)
endif()
		
