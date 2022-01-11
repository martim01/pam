cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pamupdatemanager LANGUAGES CXX VERSION 1.2.0.0)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories("../pambase/")
include_directories("../pam2/")


set(SOURCES "../pambase/advelement.cpp" "../pambase/inimanager.cpp" "../pambase/inisection.cpp" "../pambase/pmcontrol.cpp" "../pambase/pmpanel.cpp" "../pambase/settingevent.cpp" "../pambase/settings.cpp" 
"../pambase/uirect.cpp" "../pambase/wmbutton.cpp" "../pambase/wmedit.cpp" "../pambase/wmfocusmanager.cpp" "../pambase/wmkeyboard.cpp" "../pambase/wmlabel.cpp" "../pambase/wmlist.cpp" "../pambase/wmlistadv.cpp"
"../pambase/wmslidewnd.cpp" "../pambase/wmswitcherpanel.cpp" "pamupdatemanagerApp.cpp" "pamupdatemanagerMain.cpp" "releaseelement.cpp")


add_executable(pamupdatemanager ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(pamupdatemanager PRIVATE ${flags})
target_compile_definitions(pamupdatemanager PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ CREATING_LEVELDLL _app_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_libraries(pamupdatemanager ${wxWidgets_LIBRARIES})

set_property(TARGET pamupdatemanager PROPERTY CXX_STANDARD 14)
set_target_properties(pamupdatemanager PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../bin/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
install(TARGETS pamupdatemanager RUNTIME DESTINATION /usr/local/bin)
endif()
