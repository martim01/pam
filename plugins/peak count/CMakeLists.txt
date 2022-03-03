cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(peakcount LANGUAGES CXX VERSION 1.1.0)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories(${DIR_LOG}/include)
include_directories(${CMAKE_BINARY_DIR}/include)
link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(peakcount MODULE ${SOURCES} ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive"  "-std=c++14")

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(peakcount PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _test_ MSL_STDINT_H wxDEBUG_LEVEL=0)
   set_target_properties(peakcount PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/test")
else()
   list(APPEND flags "-g")
   target_compile_definitions(peakcount PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _test_ MSL_STDINT_H)
   set_target_properties(peakcount PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/test/debug")
endif()
target_compile_options(peakcount PRIVATE ${flags})

target_link_libraries(peakcount ${wxWidgets_LIBRARIES})
target_link_libraries(peakcount optimized pambase debug pambased)


#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS peakcount LIBRARY DESTINATION /usr/local/lib/pam2/test)
endif()		
