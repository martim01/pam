cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(spectogram LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories("../../pamfft/")
include_directories("../../external/kiss_fft130")
include_directories("../../external/kiss_fft130/tools")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(spectogram MODULE ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(spectogram PRIVATE ${flags})
target_compile_definitions(spectogram PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _monitor_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(spectogram ${wxWidgets_LIBRARIES})
target_link_libraries(spectogram pambase)
target_link_libraries(spectogram pamfft)

set_target_properties(spectogram PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS spectogram LIBRARY DESTINATION /usr/lib/pam2/monitor)
endif()
		
