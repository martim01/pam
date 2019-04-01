cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pambase LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#build the live555 libs
include_directories("../external/live/BasicUsageEnvironment/include")
include_directories("../external/live/groupsock/include")
include_directories("../external/live/liveMedia/include")
include_directories("../external/live/UsageEnvironment/include")

file(GLOB USAGE_SOURCES "../external/live/UsageEnvironment/*.cpp")
file(GLOB BASICUSAGE_SOURCES "../external/live/BasicUsageEnvironment/*.cpp")
file(GLOB GROUPSOCK_SOURCES "../external/live/groupsock/*.cpp")
file(GLOB LIVEMEDIA_SOURCES "../external/live/liveMedia/*.cpp")


add_library(groupsock STATIC ${GROUPSOCK_SOURCES} "../external/live/groupsock/inet.c")
add_library(UsageEnvironment STATIC ${USAGE_SOURCES} )
add_library(BasicUsageEnvironment STATIC ${BASICUSAGE_SOURCES} )
add_library(liveMedia STATIC ${LIVEMEDIA_SOURCES}  "../external/live/liveMedia/rtcp_from_spec.c")

list(APPEND flags "-fPIC" "-Wall" "-fpermissive")
target_compile_options(groupsock PRIVATE ${flags})
target_compile_options(UsageEnvironment PRIVATE ${flags})
target_compile_options(BasicUsageEnvironment PRIVATE ${flags})
target_compile_options(liveMedia PRIVATE ${flags})


#pambase settubgs

file(GLOB SOURCES "*.cpp")

add_library(pambase SHARED ${SOURCES} )

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(pambase PRIVATE ${flags})
target_compile_definitions(pambase PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _pambase_ CREATING_DLL _core_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(pambase ${wxWidgets_LIBRARIES})

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	#use pkgconfig to find the required libs
	find_package(PkgConfig REQUIRED QUIET)
	
	pkg_search_module(avahi REQUIRED avahi-client IMPORTED_TARGET)
	if(TARGET PkgConfig::avahi)
		message(STATUS "Found avahi")
		target_link_libraries(pambase PkgConfig::avahi)
	endif()
	
	pkg_search_module(portaudio REQUIRED portaudio-2.0 IMPORTED_TARGET)
	if(TARGET PkgConfig::portaudio)
		message(STATUS "Found portaudio")
		target_link_libraries(pambase PkgConfig::portaudio)
	endif()

	pkg_search_module(sndfile REQUIRED sndfile IMPORTED_TARGET)
	if(TARGET PkgConfig::sndfile)
		message(STATUS "Found sndfile")
		target_link_libraries(pambase PkgConfig::sndfile)
	endif()
endif()

target_link_libraries(pambase liveMedia)
target_link_libraries(pambase BasicUsageEnvironment)
target_link_libraries(pambase UsageEnvironment)
target_link_libraries(pambase groupsock)

set_target_properties(pambase PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../lib/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS pambase LIBRARY DESTINATION /usr/local/lib/pam2)
endif()