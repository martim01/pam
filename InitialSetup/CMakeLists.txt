cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(InitialSetup LANGUAGES CXX VERSION 1.0.0.0)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories("../pambase/")
include_directories(${CMAKE_BINARY_DIR}/include)

link_directories("../lib")

set(SOURCES "InitialSetupApp.cpp" "InitialSetupMain.cpp" ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)

add_executable(InitialSetup ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(InitialSetup PRIVATE ${flags})
target_compile_definitions(InitialSetup PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ CREATING_LEVELDLL _app_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_libraries(InitialSetup ${wxWidgets_LIBRARIES} pambase)



set_property(TARGET InitialSetup PROPERTY CXX_STANDARD 14)
set_target_properties(InitialSetup PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../bin/")


#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
install(TARGETS InitialSetup RUNTIME DESTINATION /usr/local/bin)
endif()
