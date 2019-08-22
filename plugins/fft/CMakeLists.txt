cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(fft LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories("../../pamfft/")
include_directories("../../external/kiss_fft130")
include_directories("../../external/kiss_fft130/tools")

link_directories("../../lib")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS, "Build Type: ${CMAKE_BUILD_TYPE}")

file(GLOB SOURCES "*.cpp")

add_library(fft MODULE ${SOURCES})

target_compile_definitions(fft PUBLIC DLL_EXPORTS wxUSE_UNICODE _monitor_ _MSL_STDINT_H)

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	list(APPEND flags "-O3")
	target_compile_definitions(fft PUBLIC NDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
	list(APPEND flags "-Og")
	target_compile_options(fft PRIVATE ${flags})
	target_compile_definitions(fft PUBLIC __WXDEBUG__)
endif()

target_compile_options(fft PRIVATE ${flags})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(fft PUBLIC __WXGNU__)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_compile_definitions(fft PUBLIC __WXMSW__)
endif()


#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_libraries(fft ${wxWidgets_LIBRARIES})
target_link_libraries(fft pambase)
target_link_libraries(fft pamfft)

set_target_properties(fft PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS fft LIBRARY DESTINATION /usr/local/lib/pam2/monitor)
endif()

		
