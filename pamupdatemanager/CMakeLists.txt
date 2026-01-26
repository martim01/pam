cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(pamupdatemanager LANGUAGES CXX VERSION 1.2.2)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -DOUTPUT=${CMAKE_BINARY_DIR} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories("${CMAKE_SOURCE_DIR}/pambase/")
include_directories("${CMAKE_SOURCE_DIR}/pam2/")
include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(${DIR_RESTGOOSE}/include)


set(SOURCES "${CMAKE_SOURCE_DIR}/pambase/advelement.cpp" "${CMAKE_SOURCE_DIR}/pambase/inimanager.cpp" "${CMAKE_SOURCE_DIR}/pambase/inisection.cpp" "${CMAKE_SOURCE_DIR}/pambase/pmcontrol.cpp" "${CMAKE_SOURCE_DIR}/pambase/pmpanel.cpp" "${CMAKE_SOURCE_DIR}/pambase/dlgmask.cpp" "${CMAKE_SOURCE_DIR}/pambase/settingevent.cpp" "${CMAKE_SOURCE_DIR}/pambase/settings.cpp" 
"${CMAKE_SOURCE_DIR}/pambase/uirect.cpp" "${CMAKE_SOURCE_DIR}/pambase/wmbutton.cpp" "${CMAKE_SOURCE_DIR}/pambase/wmedit.cpp" "${CMAKE_SOURCE_DIR}/pambase/wmfocusmanager.cpp" "${CMAKE_SOURCE_DIR}/pambase/wmkeyboard.cpp" "${CMAKE_SOURCE_DIR}/pambase/wmlabel.cpp" "${CMAKE_SOURCE_DIR}/pambase/wmlist.cpp" "${CMAKE_SOURCE_DIR}/pambase/wmlistadv.cpp"
"${CMAKE_SOURCE_DIR}/pambase/wmslidewnd.cpp" "${CMAKE_SOURCE_DIR}/pambase/wmswitcherpanel.cpp" "pamupdatemanagerApp.cpp" "pamupdatemanagerMain.cpp" "releaseelement.cpp" ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)


add_executable(pamupdatemanager ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(pamupdatemanager PRIVATE ${flags})
target_compile_definitions(pamupdatemanager PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ CREATING_LEVELDLL _app_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_libraries(pamupdatemanager ${wxWidgets_LIBRARIES})
set_property(TARGET pamupdatemanager PROPERTY CXX_STANDARD 17)
set_target_properties(pamupdatemanager PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
