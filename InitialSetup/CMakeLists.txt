cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(InitialSetup LANGUAGES CXX VERSION 1.0.0.0)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -DOUTPUT=${CMAKE_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories("${CMAKE_SOURCE_DIR}/pambase/")
include_directories(${CMAKE_BINARY_DIR}/include)



set(SOURCES "InitialSetupApp.cpp" "InitialSetupMain.cpp" ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp ${DIR_JSON}/dist/jsoncpp.cpp)

add_executable(InitialSetup ${SOURCES})
target_link_directories(InitialSetup PRIVATE ${CMAKE_SOURCE_DIR}/lib)

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(InitialSetup PRIVATE ${flags})
target_compile_definitions(InitialSetup PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ CREATING_LEVELDLL _app_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_libraries(InitialSetup ${wxWidgets_LIBRARIES} pambase restgoose ssl)



set_property(TARGET InitialSetup PROPERTY CXX_STANDARD 17)
set_target_properties(InitialSetup PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")


#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
install(TARGETS InitialSetup RUNTIME DESTINATION bin)
endif()
