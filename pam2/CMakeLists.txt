cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pam2 LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../pambase/")
include_directories("../external/live/BasicUsageEnvironment/include")
include_directories("../external/live/groupsock/include")
include_directories("../external/live/liveMedia/include")
include_directories("../external/live/UsageEnvironment/include")
include_directories("images/")
include_directories("../external/log/include")
include_directories("../external/ptpmonkey/include")

link_directories("../lib")


file(GLOB SOURCES "*.cpp")

add_executable(pam2 ${SOURCES})


list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(pam2 PRIVATE ${flags})
target_compile_definitions(pam2 PUBLIC NDEBUG wxUSE_UNICODE __WXGNU__ _app_ NO_OPENSSL wxDEBUG_LEVEL=0 PTPMONKEY)

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


target_link_libraries(pam2 ${wxWidgets_LIBRARIES})
target_link_libraries(pam2 pambase)
target_link_libraries(pam2 pthread)

set_target_properties(pam2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../bin/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
install(TARGETS pam2 RUNTIME DESTINATION /usr/local/bin)
endif()
