cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(pam2 LANGUAGES CXX VERSION 1.7.1)

execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../pambase/")
include_directories("../external/live/BasicUsageEnvironment/include")
include_directories("../external/live/groupsock/include")
include_directories("../external/live/liveMedia/include")
include_directories("../external/live/UsageEnvironment/include")
include_directories("images/")
include_directories(${DIR_LOG}/include)
include_directories(${DIR_RESTGOOSE}/include
include_directories(${DIR_JSON}/include))
include_directories(${DIR_PTPMONKEY}/include)
include_directories(${CMAKE_BINARY_DIR}/include)

if(${NMOS})
	include_directories(${DIR_NMOS}/include)
endif()
link_directories("../lib")



set(SOURCES "aboutelement.cpp" "dlgAoIP.cpp" "dlgEditName.cpp" "dlgEngineering.cpp" "dlgNoInput.cpp" "dlgPin.cpp" "dlgSequence.cpp" "dlgWav.cpp" "dlgWiFi.cpp" "logelement.cpp"
 "nmos.cpp" "nodebuttonfactory.cpp" "pam2App.cpp" "pam2Main.cpp" "pnlAbout.cpp" "pnlAoIPSelection.cpp" "pnlAoipManual.cpp" "pnlChannelMapping.cpp" "pnlHelp.cpp" "pnlLog.cpp"
 "pnlLogControl.cpp" "pnlNetworkSetup.cpp" "pnlRTCPTransmission.cpp" "pnlRTP.cpp" "pnlSettings.cpp" "pnlSettingsGenerators.cpp" "pnlSettingsInputNmos.cpp"
 "pnlSettingsNmos.cpp" "pnlSettingsOptions.cpp" "pnlSettingsOutput.cpp" "pnlSettingsPlugins.cpp" "pnlSettingsProfiles.cpp" "pnlSettingsTime.cpp"
 "pnlTestOptions.cpp" "pnlSettingsRemote.cpp" "pnlTests.cpp" "pnlUSB.cpp" "pnlUpdate.cpp" "senderbuttonfactory.cpp" "wxclientapiposter.cpp" "wxdirtraverseusb.cpp" "wxeventposter.cpp"
 "wxpammclient.cpp" "wxpammconnection.cpp" "usbchecker.cpp" "updater.cpp" "${DIR_LOG}/src/wx/wxlogoutput.cpp" ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp)


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

target_link_libraries(pam2 ${wxWidgets_LIBRARIES} cap ssl)
target_link_libraries(pam2 optimized pml_log debug pml_logd)
target_link_libraries(pam2 optimized pml_dnssd debug pml_dnssdd)
target_link_libraries(pam2 avahi-client)
target_link_libraries(pam2 optimized restgoose debug restgoosed)
if(${NMOS})
	target_link_libraries(pam2 optimized nmos_base debug nmos_based)
	target_link_libraries(pam2 optimized nmos_node debug nmos_node)
	target_link_libraries(pam2 optimized nmos_client debug nmos_client)
endif()
target_link_libraries(pam2 optimized sapserver debug sapserverd)
target_link_libraries(pam2 optimized livewireserver debug livewireserverd)
target_link_libraries(pam2 pthread)
target_link_libraries(pam2 optimized ptpmonkey debug ptpmonkeyd)
target_link_libraries(pam2 optimized pambase debug pambased)
target_link_libraries(pam2 optimized pamfft debug pamfftd)
target_link_libraries(pam2 optimized pamlevel debug pamleveld)


set_target_properties(pam2 PROPERTIES CXX_STANDARD 17)
set_target_properties(pam2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../bin/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
install(TARGETS pam2 RUNTIME DESTINATION bin)
install(CODE "execute_process(COMMAND setcap cap_sys_time,cap_sys_admin,cap_net_bind_service+ep /usr/local/bin/pam2 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})")
install(FILES ${CMAKE_SOURCE_DIR}/documents/pam2.conf DESTINATION "../../etc/ld.so.conf.d/")
install(CODE "execute_process(COMMAND ldconfig)")
install(FILES ${CMAKE_SOURCE_DIR}/documents/audio_hats.xml DESTINATION etc/pam2/)
install(FILES  ${CMAKE_SOURCE_DIR}/documents/macaddress.xml DESTINATION etc/pam2/)
install(FILES  ${CMAKE_SOURCE_DIR}/documents/ppmtypes.xml DESTINATION etc/pam2/)
install(FILES  ${CMAKE_SOURCE_DIR}/documents/pam2.ini DESTINATION etc/pam2/)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/documents/help DESTINATION etc/pam2/)
install(DIRECTORY  ${CMAKE_SOURCE_DIR}/documents/generator DESTINATION /etc/pam2/)
endif()
