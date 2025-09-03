cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(pophub LANGUAGES CXX VERSION 0.1.0)


execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories(${CMAKE_BINARY_DIR}/include)

include_directories("../../pamlevel")

link_directories("../../lib")


set(SOURCES "pophubpanel.cpp" "pophubbuilder.cpp" "pophubregister.cpp" "pnlMode.cpp" "pnlMeters.cpp" "pnlOptions.cpp" "pnlPhase.cpp"  "pnlr128.cpp" "pnlView.cpp" "r128meter.cpp")

add_library(pophub MODULE ${SOURCES} ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive"  "-std=c++17")

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(pophub PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _monitor_ MSL_STDINT_H wxDEBUG_LEVEL=0 _CMAKE_)
   set_target_properties(pophub PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor")
else()
   list(APPEND flags "-g")
   target_compile_definitions(pophub PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _monitor_ MSL_STDINT_H _CMAKE_)
   set_target_properties(pophub PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor/debug")
endif()
target_compile_options(pophub PRIVATE ${flags})

target_link_libraries(pophub ${wxWidgets_LIBRARIES})
target_link_libraries(pophub optimized pambase debug pambased)

   
target_link_libraries(pophub optimized pamlevel debug pamleveld)

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS pophub LIBRARY DESTINATION lib/pam2/monitor)
endif()		
