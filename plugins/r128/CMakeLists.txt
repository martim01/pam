cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(r128 LANGUAGES CXX VERSION 1.3.0)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -P ${CMAKE_SOURCE_DIR}/version.cmake)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories("../../pamlevel/")
include_directories(${DIR_JSON}/dist)
include_directories(${CMAKE_BINARY_DIR}/include)
link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(r128 MODULE ${SOURCES} ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive")

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(r128 PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _monitor_ MSL_STDINT_H wxDEBUG_LEVEL=0)
   set_target_properties(r128 PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor")
else()
   list(APPEND flags "-g")
   target_compile_definitions(r128 PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _monitor_ MSL_STDINT_H)
   set_target_properties(r128 PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor/debug")
endif()
target_compile_options(r128 PRIVATE ${flags})

target_link_libraries(r128 ${wxWidgets_LIBRARIES})
target_link_libraries(r128 optimized pambase debug pambased)
target_link_libraries(r128 optimized pamlevel debug pamleveld)
target_link_libraries(r128 optimized pamfft debug pamfftd)
set_target_properties(r128 PROPERTIES CXX_STANDARD 17)

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS r128 LIBRARY DESTINATION lib/pam2/monitor)
endif()	
