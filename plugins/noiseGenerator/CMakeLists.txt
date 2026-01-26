cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(noiseGenerator LANGUAGES CXX C VERSION 1.0.1)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -DOUTPUT=${CMAKE_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("${CMAKE_SOURCE_DIR}/pambase/")
include_directories(${CMAKE_BINARY_DIR}/include)


file(GLOB SOURCES "*.cpp")
		

add_library(noiseGenerator MODULE ${SOURCES} ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)
target_link_directories(noiseGenerator PRIVATE ${CMAKE_SOURCE_DIR}/lib)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive")

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(noiseGenerator PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _generator_ MSL_STDINT_H wxDEBUG_LEVEL=0)
   set_target_properties(noiseGenerator PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/generator")
else()
   list(APPEND flags "-g")
   target_compile_definitions(noiseGenerator PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _generator_ MSL_STDINT_H)
   set_target_properties(noiseGenerator PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/generator/debug")
endif()
target_compile_options(noiseGenerator PRIVATE ${flags})

target_link_libraries(noiseGenerator ${wxWidgets_LIBRARIES})
target_link_libraries(noiseGenerator optimized pambase debug pambased)
set_target_properties(noiseGenerator PROPERTIES CXX_STANDARD 17)

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS noiseGenerator LIBRARY DESTINATION lib/pam2/generator)
endif()	
