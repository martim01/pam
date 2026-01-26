cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(correlation LANGUAGES CXX VERSION 0.1.0)


execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -DOUTPUT=${CMAKE_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("${CMAKE_SOURCE_DIR}/pambase/")
include_directories(${CMAKE_BINARY_DIR}/include)

include_directories("${CMAKE_SOURCE_DIR}/pamlevel")

set(SOURCES "correlationpanel.cpp" "correlationbuilder.cpp" "correlationregister.cpp")
#file(GLOB SOURCES "*.cpp")

add_library(correlation MODULE ${SOURCES} ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)
target_link_directories(correlation PRIVATE ${CMAKE_SOURCE_DIR}/lib)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" )

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(correlation PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _monitor_ MSL_STDINT_H wxDEBUG_LEVEL=0)
   set_target_properties(correlation PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/monitor")
else()
   list(APPEND flags "-g")
   target_compile_definitions(correlation PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _monitor_ MSL_STDINT_H)
   set_target_properties(correlation PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/monitor/debug")
endif()
target_compile_options(correlation PRIVATE ${flags})

target_link_libraries(correlation ${wxWidgets_LIBRARIES})
target_link_libraries(correlation optimized pambase debug pambased)

   
target_link_libraries(correlation optimized pamlevel debug pamleveld)

set_target_properties(correlation PROPERTIES CXX_STANDARD 17)

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS correlation LIBRARY DESTINATION lib/pam2/monitor)
endif()		
