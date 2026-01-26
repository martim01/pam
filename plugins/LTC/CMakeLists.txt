cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(LTC LANGUAGES CXX C VERSION 1.2.0)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -DOUTPUT=${CMAKE_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("${CMAKE_SOURCE_DIR}/pambase/")
include_directories("${CMAKE_SOURCE_DIR}/external/libltc")
include_directories(${CMAKE_BINARY_DIR}/include)


file(GLOB SOURCES "*.cpp")
list(APPEND LTC_SOURCES  "${CMAKE_SOURCE_DIR}/external/libltc/decoder.c"
						 "${CMAKE_SOURCE_DIR}/external/libltc/encoder.c"
						 "${CMAKE_SOURCE_DIR}/external/libltc/ltc.c"
						 "${CMAKE_SOURCE_DIR}/external/libltc/timecode.c")
		

add_library(LTC MODULE ${SOURCES} ${LTC_SOURCES} ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)
target_link_directories(LTC PRIVATE ${CMAKE_SOURCE_DIR}/lib)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" )

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(LTC PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _test_ MSL_STDINT_H wxDEBUG_LEVEL=0)
   set_target_properties(LTC PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/test")
else()
   list(APPEND flags "-g")
   target_compile_definitions(LTC PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _test_ MSL_STDINT_H)
   set_target_properties(LTC PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/test/debug")
endif()
target_compile_options(LTC PRIVATE ${flags})

target_link_libraries(LTC ${wxWidgets_LIBRARIES})
target_link_libraries(LTC optimized pambase debug pambased)

set_target_properties(LTC PROPERTIES CXX_STANDARD 17)

	#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS LTC LIBRARY DESTINATION lib/pam2/test)
endif()	
