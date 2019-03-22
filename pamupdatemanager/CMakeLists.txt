cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pamupdatemanager LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../pambase/")

link_directories("../lib")


file(GLOB SOURCES "*.cpp")

add_executable(pamupdatemanager ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(pamupdatemanager PRIVATE ${flags})
target_compile_definitions(pamupdatemanager PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ CREATING_LEVELDLL _app_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	#use pkgconfig to find the required libs
	find_package(PkgConfig REQUIRED QUIET)
	
	pkg_search_module(curl REQUIRED libcurl IMPORTED_TARGET)
	if(TARGET PkgConfig::curl)
		message(STATUS "Found libcurl")
		target_link_libraries(pamupdatemanager PkgConfig::curl)
	endif()
	
endif()


target_link_libraries(pamupdatemanager ${wxWidgets_LIBRARIES})
target_link_libraries(pamupdatemanager pambase)

set_target_properties(pamupdatemanager PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../bin/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
install(TARGETS pamupdatemanager RUNTIME DESTINATION /usr/local/bin)
endif()
