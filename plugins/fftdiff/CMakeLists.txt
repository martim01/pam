cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(fftdiff LANGUAGES CXX VERSION 0.1.0)


execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories(${CMAKE_BINARY_DIR}/include)
include_directories("../../pamfft/" "../../external/kiss_fft130" "../../external/kiss_fft130/tools")
link_directories("../../lib")


set(SOURCES "fftdiffmeter.cpp" "fftdiffbuilder.cpp" "fftdiffregister.cpp" "pnlbins.cpp" "pnloverlap.cpp" "pnlwindow.cpp" "pnloptions.cpp" "pnlmode.cpp" "pnldelay.cpp" "pnlrange.cpp")
#file(GLOB SOURCES "*.cpp")

add_library(fftdiff MODULE ${SOURCES} ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive"  "-std=c++14")

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(fftdiff PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _monitor_ MSL_STDINT_H wxDEBUG_LEVEL=0)
   set_target_properties(fftdiff PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor")
else()
   list(APPEND flags "-g")
   target_compile_definitions(fftdiff PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _monitor_ MSL_STDINT_H)
   set_target_properties(fftdiff PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor/debug")
endif()
target_compile_options(fftdiff PRIVATE ${flags})

target_link_libraries(fftdiff ${wxWidgets_LIBRARIES})
target_link_libraries(fftdiff optimized pambase debug pambased)

target_link_libraries(fftdiff optimized pamfft debug pamfftd)   


#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS fftdiff LIBRARY DESTINATION /usr/local/lib/pam2/monitor)
endif()		
