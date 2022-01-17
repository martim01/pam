cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pam2 LANGUAGES CXX VERSION 1.2.0.0)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../pambase/")
include_directories("../external/live/BasicUsageEnvironment/include")
include_directories("../external/live/groupsock/include")
include_directories("../external/live/liveMedia/include")
include_directories("../external/live/UsageEnvironment/include")
include_directories("images/")
include_directories(${DIR_LOG}/include)
include_directories(${DIR_RESTGOOSE}/include)
include_directories(${DIR_PTPMONKEY}/include)
include_directories(${CMAKE_BINARY_DIR}/include)

if(${NMOS})
	include_directories(${DIR_NMOS}/include)
	include_directories(${DIR_JSON}/include)
endif()
link_directories("../lib")



set(SOURCES "aboutelement.cpp" "dlgAoIP.cpp" "dlgEditName.cpp" "dlgEngineering.cpp" "dlgNoInput.cpp" "dlgPin.cpp" "dlgSequence.cpp" "dlgWiFi.cpp" "dlgmask.cpp" "logelement.cpp"
 "networkcontrol.cpp" "nmos.cpp" "nodebuttonfactory.cpp" "pam2App.cpp" "pam2Main.cpp" "pnlAbout.cpp" "pnlAoIPSelection.cpp" "pnlAoipManual.cpp" "pnlHelp.cpp" "pnlLog.cpp"
 "pnlLogControl.cpp" "pnlNetworkSetup.cpp" "pnlRTCPTransmission.cpp" "pnlRTP.cpp" "pnlSettings.cpp" "pnlSettingsGenerators.cpp" "pnlSettingsInputNmos.cpp"
 "pnlSettingsNmos.cpp" "pnlSettingsOptions.cpp" "pnlSettingsOutput.cpp" "pnlSettingsPlugins.cpp" "pnlSettingsProfiles.cpp" "pnlSettingsTime.cpp"
 "pnlTestOptions.cpp" "pnlTests.cpp" "pnlUSB.cpp" "pnlUpdate.cpp" "senderbuttonfactory.cpp" "wxclientapiposter.cpp" "wxdirtraverseusb.cpp" "wxeventposter.cpp"
 "wxlogoutput.cpp" "wxpammclient.cpp" "wxpammconnection.cpp" "usbchecker.cpp" "updater.cpp" ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)


add_executable(pam2 ${SOURCES})

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	#use pkgconfig to find the required libs
	find_package(PkgConfig REQUIRED)
	pkg_search_module(avahi avahi-client REQUIRED)

endif()


list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(pam2 PRIVATE ${flags})

list(APPEND definitions "NDEBUG" "wxUSE_UNICODE" "__WXGNU__" "_app_" "NO_OPENSSL" "wxDEBUG_LEVEL=0" "PTPMONKEY")
if(${NMOS})
	list(APPEND definitions "__NMOS__")
endif()
target_compile_definitions(pam2 PUBLIC ${definitions})

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_libraries(pam2 ${wxWidgets_LIBRARIES} cap)
target_link_libraries(pam2 pml_log)
target_link_libraries(pam2 pml_dnssd)
target_link_libraries(pam2 avahi-client)
if(${NMOS})
	target_link_libraries(pam2 restgoose)
	target_link_libraries(pam2 nmos_base)
	target_link_libraries(pam2 nmos_node)
	target_link_libraries(pam2 nmos_client)
endif()
target_link_libraries(pam2 sapserver)
target_link_libraries(pam2 pthread)
target_link_libraries(pam2 ptpmonkey)
target_link_libraries(pam2 pambase)
target_link_libraries(pam2 pamfft)
target_link_libraries(pam2 pamlevel)


set_target_properties(pam2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../bin/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
message($ENV{USER})
install(TARGETS pam2 RUNTIME DESTINATION /usr/local/bin)
install(CODE "execute_process(COMMAND setcap cap_sys_time,cap_sys_admin,cap_net_bind_service+ep /usr/local/bin/pam2 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})")
install(CODE "execute_process(COMMAND cp ${CMAKE_SOURCE_DIR}/documents/pam2.conf /etc/ld.so.conf.d/)")
install(CODE "execute_process(COMMAND ldconfig)")
install(CODE "execute_process(COMMAND cp ${CMAKE_SOURCE_DIR}/documents/audio_hats.xml ${DIR_PAM})")

install(CODE "execute_process(COMMAND cp ${CMAKE_SOURCE_DIR}/documents/macaddress.xml ${DIR_PAM})")
install(CODE "execute_process(COMMAND cp ${CMAKE_SOURCE_DIR}/documents/ppmtypes.xml ${DIR_PAM})")
install(CODE "execute_process(COMMAND cp ${CMAKE_SOURCE_DIR}/documents/pam2.ini ${DIR_PAM})")
install(CODE "execute_process(COMMAND cp -r ${CMAKE_SOURCE_DIR}/documents/help ${DIR_PAM})")
install(CODE "execute_process(COMMAND cp -r ${CMAKE_SOURCE_DIR}/documents/generator ${DIR_PAM})")
install(CODE "execute_process(COMMAND chown $ENV{USER} ${DIR_PAM}")
endif()
