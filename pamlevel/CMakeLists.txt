cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(pamlevel LANGUAGES CXX VERSION 1.3.0)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../pambase/")
include_directories(${CMAKE_BINARY_DIR}/include)
link_directories("../lib")



add_library(pamlevel SHARED "audioalgorithms.cpp" "filt.cpp" "truepeakcalculator.cpp" ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)

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

list(APPEND flags "-fPIC" "-Wall" "-fpermissive")

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
set_target_properties(pamlevel PROPERTIES CXX_STANDARD 17)
set_target_properties(pamlevel PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../lib/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS pamlevel LIBRARY DESTINATION lib)
endif()
		
