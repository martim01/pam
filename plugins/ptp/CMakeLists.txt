project(ptp LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories("../../external/ptpmonkey/include")
include_directories("../../external/log/include")


link_directories("../../lib")

if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS, "Build Type: ${CMAKE_BUILD_TYPE}")

file(GLOB SOURCES "*.cpp")

add_library(ptp MODULE ${SOURCES})

target_compile_definitions(ptp PUBLIC DLL_EXPORTS wxUSE_UNICODE _monitor_ _MSL_STDINT_H wxDEBUG_LEVEL=0)

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
        list(APPEND flags "-O3")
        target_compile_definitions(ptp PUBLIC NDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        list(APPEND flags "-Og")
        target_compile_options(ptp PRIVATE ${flags})
        target_compile_definition(ptp PUBLIC __WXDEBUG__)
endif()

target_compile_options(ptp PRIVATE ${flags})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_compile_definitions(ptp PUBLIC __WXGNU__)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        target_compile_definitions(ptp PUBLIC __WXMSW__)
endif()


#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_libraries(ptp ${wxWidgets_LIBRARIES})
target_link_libraries(ptp pambase)

set_target_properties(ptp PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/monitor")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        install(TARGETS ptp LIBRARY DESTINATION /usr/local/lib/pam2/monitor)
endif()
