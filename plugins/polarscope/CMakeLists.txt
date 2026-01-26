cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(polarscope LANGUAGES CXX VERSION 1.2.1)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -DOUTPUT=${CMAKE_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("${CMAKE_SOURCE_DIR}/pambase/")
include_directories(${CMAKE_BINARY_DIR}/include)


file(GLOB SOURCES "*.cpp")

add_library(polarscope MODULE ${SOURCES} ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)
target_link_directories(polarscope PRIVATE ${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(polarscope optimized pamfft debug pamfftd)
#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive")

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(polarscope PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _monitor_ MSL_STDINT_H wxDEBUG_LEVEL=0)
   set_target_properties(polarscope PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/monitor")
else()
   list(APPEND flags "-g")
   target_compile_definitions(polarscope PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _monitor_ MSL_STDINT_H)
   set_target_properties(polarscope PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/monitor/debug")
endif()
target_compile_options(polarscope PRIVATE ${flags})

target_link_libraries(polarscope ${wxWidgets_LIBRARIES})
target_link_libraries(polarscope optimized pambase debug pambased)
set_target_properties(polarscope PROPERTIES CXX_STANDARD 17)

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS polarscope LIBRARY DESTINATION lib/pam2/monitor)
endif()
		
