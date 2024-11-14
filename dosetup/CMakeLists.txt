cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(dosetup LANGUAGES CXX VERSION 1.0.0.0)


execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories("../pambase/")
include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(${DIR_LOG}/include)

link_directories("../lib")

set(SOURCES "main.cpp" ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)

add_executable(dosetup ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(dosetup PRIVATE ${flags})
target_compile_definitions(dosetup PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ CREATING_LEVELDLL _app_ _MSL_STDINT_H)
target_link_libraries(dosetup pml_log)



set_property(TARGET dosetup PROPERTY CXX_STANDARD 17)
set_target_properties(dosetup PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../bin/")


#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
install(TARGETS dosetup RUNTIME DESTINATION bin)
endif()

