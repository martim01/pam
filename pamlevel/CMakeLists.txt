cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pamlevel LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../pambase/")
link_directories("../lib")


file(GLOB SOURCES "*.cpp")

add_library(pamlevel SHARED ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(pamlevel PRIVATE ${flags})
target_compile_definitions(pamlevel PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ CREATING_LEVELDLL _core_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	#use pkgconfig to find the required libs
	find_package(PkgConfig REQUIRED QUIET)
	pkg_search_module(samplerate REQUIRED samplerate IMPORTED_TARGET)
	if(TARGET PkgConfig::samplerate)
		message(STATUS "Found libsamplerate")
		target_link_libraries(pamlevel PkgConfig::samplerate)
	endif()
endif()

list(APPEND flags "-fPIC" "-Wall" "-fpermissive"  "-std=c++14")

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(pamlevel PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _core_ MSL_STDINT_H wxDEBUG_LEVEL=0)
else()
   list(APPEND flags "-g")
   target_compile_definitions(pamlevel PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _core_ MSL_STDINT_H)
endif()
target_compile_options(pamlevel PRIVATE ${flags})

target_link_libraries(pamlevel ${wxWidgets_LIBRARIES})
target_link_libraries(pamlevel optimized pambase debug pambased)

set_target_properties(pamlevel PROPERTIES DEBUG_POSTFIX "d")

set_target_properties(pamlevel PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../lib/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS pamlevel LIBRARY DESTINATION /usr/local/lib/pam2)
endif()
		
