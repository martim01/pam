cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pam2 LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../pambase/")
include_directories("../external/live/BasicUsageEnvironment/include")
include_directories("../external/live/groupsock/include")
include_directories("../external/live/liveMedia/include")
include_directories("../external/live/UsageEnvironment/include")
include_directories("images/")

link_directories("../lib")


file(GLOB SOURCES "*.cpp")

add_executable(pam2 ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(pam2 PRIVATE ${flags})
target_compile_definitions(pam2 PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ CREATING_LEVELDLL _app_ _MSL_STDINT_H)

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
		target_link_libraries(pam2 PkgConfig::curl)
	endif()

	pkg_search_module(avahi REQUIRED avahi-client IMPORTED_TARGET)
	if(TARGET PkgConfig::curl)
		message(STATUS "Found avahi-client")
		target_link_libraries(pam2 PkgConfig::avahi)
	endif()
	
	pkg_search_module(portaudio REQUIRED portaudio-2.0 IMPORTED_TARGET)
	if(TARGET PkgConfig::portaudio)
		message(STATUS "Found portaudio")
		target_link_libraries(pam2 PkgConfig::portaudio)
	endif()	
endif()

option(optionNMOS "Use nmos library" OFF)
message (${PROJECT_SOURCE_DIR})
if(optionNMOS)
	find_path(NMOS_LIB_DIR NAMES "libnmos.so" PATHS ${PROJECT_SOURCE_DIR}/../external/nmos)
	
	if((NOT NMOS_LIB_DIR) OR (NOT EXISTS ${NMOS_LIB_DIR}))
    # we couldn't find the header files for nmos or they don't exist
    message("Unable to find nmos")
    

    # we have a submodule setup for nmos, assume it is under external/nmos
    # now we need to clone this submodule
    execute_process(COMMAND git submodule update --init -- ../external/nmos
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

    
	add_subdirectory(${PROJECT_SOURCE_DIR}/../external/nmos ${CMAKE_CURRENT_BINARY_DIR}/nmos)
    else()
		message("Found nmos")
	endif()
	target_compile_definitions(pam2 PUBLIC __NMOS__)
	target_link_libraries(pam2 nmos)
endif()


target_link_libraries(pam2 ${wxWidgets_LIBRARIES})
target_link_libraries(pam2 pambase)

set_target_properties(pam2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../bin/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
install(TARGETS pam2 RUNTIME DESTINATION /usr/local/bin)
endif()
