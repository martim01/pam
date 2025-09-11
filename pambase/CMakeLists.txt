cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(pambase LANGUAGES CXX VERSION 1.8.2)


execute_process(COMMAND ${CMAKE_COMMAND} -DNAMESPACE=${PROJECT_NAME} -DMAJOR=${PROJECT_VERSION_MAJOR} -DMINOR=${PROJECT_VERSION_MINOR} -DPATCH=${PROJECT_VERSION_PATCH} -P ${CMAKE_SOURCE_DIR}/version.cmake)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

#build the live555 libs
include_directories("../external/live/BasicUsageEnvironment/include")
include_directories("../external/live/groupsock/include")
include_directories("../external/live/liveMedia/include")
include_directories("../external/live/UsageEnvironment/include")

file(GLOB USAGE_SOURCES "../external/live/UsageEnvironment/*.cpp")
file(GLOB BASICUSAGE_SOURCES "../external/live/BasicUsageEnvironment/*.cpp")
file(GLOB GROUPSOCK_SOURCES "../external/live/groupsock/*.cpp")
file(GLOB LIVEMEDIA_SOURCES "../external/live/liveMedia/*.cpp")

add_library(groupsock STATIC ${GROUPSOCK_SOURCES} "../external/live/groupsock/inet.c")
add_library(UsageEnvironment STATIC ${USAGE_SOURCES} )
add_library(BasicUsageEnvironment STATIC ${BASICUSAGE_SOURCES} )
add_library(liveMedia STATIC ${LIVEMEDIA_SOURCES}  "../external/live/liveMedia/rtcp_from_spec.c")

list(APPEND flags "-fPIC" "-Wall" "-fpermissive")
target_compile_options(groupsock PRIVATE ${flags})
target_compile_options(UsageEnvironment PRIVATE ${flags})
target_compile_options(BasicUsageEnvironment PRIVATE ${flags})
target_compile_options(liveMedia PRIVATE ${flags})

target_compile_definitions(groupsock PUBLIC NO_OPENSSL)
target_compile_definitions(UsageEnvironment PUBLIC NO_OPENSSL)
target_compile_definitions(BasicUsageEnvironment PUBLIC NO_OPENSSL)
target_compile_definitions(liveMedia PUBLIC NO_OPENSSL)


message(${CMAKE_CURRENT_SOURCE_DIR})

#pambase settubgs

set(SOURCES  "advelement.cpp"  "AES67ServerMediaSubsession.cpp" "aes67source.cpp" "anglemeter.cpp" "aoipsourcemanager.cpp" "audio.cpp" "audioevent.cpp" "dlgEditName.cpp" "dlgmask.cpp" 
 "generator.cpp" "generatorpluginbuilder.cpp" "generatorpluginfactory.cpp" "histogram.cpp" "historygraph.cpp" "inimanager.cpp" "inisection.cpp" "iomanager.cpp" "levelcalculator.cpp" "levelgraph.cpp" "wxlivewirehandler.cpp"
 "levelmeter.cpp" "liveaudiosource.cpp" "monitorevent.cpp" "monitorpluginbuilder.cpp" "monitorpluginfactory.cpp" "networkcontrol.cpp" "pamRTSPServer.cpp" "niir.cpp" "ondemandaes67mediasubsession.cpp" "ondemandpamsubsession.cpp"
 "ondemandstreamer.cpp" "ourRTSPClient.cpp" "ourSIPClient.cpp" "PamTaskScheduler.cpp" "PamUsageEnvironment.cpp" "pcstats.cpp" "pmcontrol.cpp" "pmpanel.cpp" "ppmtypes.cpp" "qos.cpp" "r128calculator.cpp"
 "r128thread.cpp" "rawvideosource.cpp" "remoteapi.cpp" "RTCPTransmissionEvent.cpp" "rtpframeevent.cpp" "rtpserverthread.cpp" "rtpthread.cpp" "rtsputil.cpp" "servicebrowser.cpp" "session.cpp" "settingevent.cpp" "settings.cpp" "smpte2110mediasession.cpp"
 "soundcardmanager.cpp" "soundfile.cpp" "streamclientstate.cpp" "testpluginbuilder.cpp" "testpluginfactory.cpp" "timemanager.cpp" "uirect.cpp" "wmbutton.cpp" "wmedit.cpp" "wmfocusmanager.cpp" "wmipeditpnl.cpp"
 "wmkeyboard.cpp" "wmlabel.cpp" "wmline.cpp" "wmlist.cpp" "wmlistadv.cpp" "wmslider.cpp" "wmslidewnd.cpp" "wmswipebutton.cpp" "wmswitcherpanel.cpp" "wmtimeedit.cpp" "wxptp.cpp" "wxsaphandler.cpp"
 "wxsink.cpp" "wxtouchscreenhtml.cpp" "wxzxposter.cpp" ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME}_version.cpp "${DIR_JSON}/dist/jsoncpp.cpp")
 
add_library(pambase SHARED ${SOURCES})

target_include_directories(pambase PUBLIC ${DIR_LOG}/include)
target_include_directories(pambase PUBLIC ${DIR_DNSSD}/include)
target_include_directories(pambase PUBLIC ${DIR_SAPSERVER}/include)
target_include_directories(pambase PUBLIC ${DIR_LIVEWIRE}/include)
target_include_directories(pambase PUBLIC ${DIR_ASIO}/asio/include)
target_include_directories(pambase PUBLIC ${DIR_PTPMONKEY}/include)
target_include_directories(pambase PUBLIC ${DIR_RESTGOOSE}/include)
target_include_directories(pambase PUBLIC ${DIR_JSON}/dist)
target_include_directories(pambase PUBLIC ${CMAKE_BINARY_DIR}/include)


set_target_properties(pambase PROPERTIES CXX_STANDARD 17)
list(APPEND flags "-fPIC" "-Wall" "-fpermissive" )

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(pambase PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _pambase_ CREATING_DLL _core_ _MSL_STDINT_H NO_OPENSSL ASIO_STANDALONE  PTPMONKEY wxDEBUG_LEVEL=0)
else()
   list(APPEND flags "-g")
   target_compile_definitions(pambase PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _pambase_ CREATING_DLL _core_ _MSL_STDINT_H NO_OPENSSL ASIO_STANDALONE PTPMONKEY)
endif()
target_compile_options(pambase PRIVATE ${flags})


#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(pambase ${wxWidgets_LIBRARIES})

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	#use pkgconfig to find the required libs
	find_package(PkgConfig REQUIRED QUIET)
	
	pkg_search_module(portaudio REQUIRED portaudio-2.0 IMPORTED_TARGET)
	if(TARGET PkgConfig::portaudio)
		message(STATUS "Found portaudio")
		target_link_libraries(pambase PkgConfig::portaudio)
	endif()

	pkg_search_module(sndfile REQUIRED sndfile IMPORTED_TARGET)
	if(TARGET PkgConfig::sndfile)
		message(STATUS "Found sndfile")
		target_link_libraries(pambase PkgConfig::sndfile)
	endif()
endif()

link_directories(${CMAKE_BINARY_DIR}/../lib/)
target_link_libraries(pambase liveMedia)
target_link_libraries(pambase BasicUsageEnvironment)
target_link_libraries(pambase UsageEnvironment)
target_link_libraries(pambase groupsock)
#target_link_libraries(pambase jsoncpp_lib)
#target_link_libraries(pambase pthread)

target_link_libraries(pambase optimized pml_log debug pml_logd)
target_link_libraries(pambase optimized pml_dnssd debug pml_dnssdd)
target_link_libraries(pambase avahi-client)
target_link_libraries(pambase optimized sapserver debug sapserverd)
target_link_libraries(pambase optimized ptpmonkey debug ptpmonkeyd)
target_link_libraries(pambase optimized restgoose debug restgoosed)
target_link_libraries(pambase optimized livewireserver debug livewireserverd)

set_target_properties(pambase PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(pambase PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/)

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS pambase LIBRARY DESTINATION lib)
endif()
