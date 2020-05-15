cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(channeldelay LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories("../../pamfft/")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(channeldelay MODULE ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(channeldelay PRIVATE ${flags})
target_compile_definitions(channeldelay PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _test_ _MSL_STDINT_H wxDEBUG_LEVEL=0)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(channeldelay ${wxWidgets_LIBRARIES})
target_link_libraries(channeldelay pambase)
target_link_libraries(channeldelay pamfft)

set_target_properties(channeldelay PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/test")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS channeldelay LIBRARY DESTINATION /usr/local/lib/pam2/test)
endif()
		
