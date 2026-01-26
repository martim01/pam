cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(pamfft LANGUAGES CXX C VERSION 1.2.0.0)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("${CMAKE_SOURCE_DIR}/pambase/")
include_directories(${DIR_RESTGOOSE}/include)
include_directories("${CMAKE_SOURCE_DIR}/external/kiss_fft130")
include_directories("${CMAKE_SOURCE_DIR}/external/kiss_fft130/tools")
include_directories(${CMAKE_BINARY_DIR}/include)



execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -DOUTPUT=${CMAKE_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/version.cmake)

file(GLOB SOURCES "*.cpp" "${CMAKE_SOURCE_DIR}/external/kiss_fft130/kiss_fft.c" "${CMAKE_SOURCE_DIR}/external/kiss_fft130/tools/kfc.c" 
 "${CMAKE_SOURCE_DIR}/external/kiss_fft130/tools/kiss_fftnd.c" "${CMAKE_SOURCE_DIR}/external/kiss_fft130/tools/kiss_fftndr.c" "${CMAKE_SOURCE_DIR}/external/kiss_fft130/tools/kiss_fftr.c"	"kiss_xcorr.c" ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)

add_library(pamfft SHARED ${SOURCES})
#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_directories(pamfft PRIVATE ${CMAKE_SOURCE_DIR}/lib)

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" )

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(pamfft PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _core_ MSL_STDINT_H wxDEBUG_LEVEL=0)
else()
   list(APPEND flags "-g")
   target_compile_definitions(pamfft PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _core_ MSL_STDINT_H)
endif()
target_compile_options(pamfft PRIVATE ${flags})

target_link_libraries(pamfft ${wxWidgets_LIBRARIES})
target_link_libraries(pamfft optimized pambase debug pambased)
target_link_libraries(pamfft optimized restgoose debug restgoosed)

set_target_properties(pamfft PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(pamfft PROPERTIES CXX_STANDARD 17)



set_target_properties(pamfft PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS pamfft LIBRARY DESTINATION lib)
endif()
		
