cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pambase LANGUAGES CXX)

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



#log
find_path(LOG_LIB_DIR NAMES "include/log.h" PATHS ${PROJECT_SOURCE_DIR}/../external/log)
if((NOT LOG_LIB_DIR) OR (NOT EXISTS ${LOG_LIB_DIR}))
    # we couldn't find the header files for FOO or they don't exist
    message("Unable to find pml::log")
    
    # now we need to clone this submodule
    execute_process(COMMAND git submodule update --init -- ../external/log WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    find_path(LOG_LIB_DIR NAMES "include/log.h" PATHS ${PROJECT_SOURCE_DIR}/../external/log)
else()
    message("Found pml::log")
endif()
include_directories(${LOG_LIB_DIR}/include)


#dnssd
find_path(DNSSD_LIB_DIR NAMES "include/dnssd.h" PATHS ${PROJECT_SOURCE_DIR}/../external/dnssd)
if((NOT DNSSD_LIB_DIR) OR (NOT EXISTS ${DNSSD_LIB_DIR}))
    # we couldn't find the header files for FOO or they don't exist
    message("Unable to find pml::dnssd")
    
    # we have a submodule setup for dnssd, assume it is under external/dnssd
    # now we need to clone this submodule
    execute_process(COMMAND git submodule update --init -- ${PROJECT_SOURCE_DIR}/../external/dnssd WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    find_path(DNSSD_LIB_DIR NAMES "include/dnssd.h" PATHS ${PROJECT_SOURCE_DIR}/../external/dnssd)
else()
    message("Found pml::dnssd")
endif()

include_directories(${DNSSD_LIB_DIR}/include)


#sapserver
find_path(SAP_LIB_DIR NAMES "/include/sapserver.h" PATHS ${PROJECT_SOURCE_DIR}/../external/sapserver)
if((NOT SAP_LIB_DIR) OR (NOT EXISTS ${SAP_LIB_DIR}))
    # we couldn't find the header files for FOO or they don't exist
    message("Unable to find sapserver")
    
    # we have a submodule setup for asio, assume it is under external/asio
    # now we need to clone this submodule
    execute_process(COMMAND git submodule update --init -- ../external/sapserver WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    find_path(SAP_LIB_DIR NAMES "/include/sapserver.h" PATHS ${PROJECT_SOURCE_DIR}/../external/sapserver)
    execute_process(COMMAND git submodule update --init -- ../external/sapserver/external/asio WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
else()
    message("Found sapserver")
endif()

include_directories(${SAP_LIB_DIR}/include)
include_directories(${SAP_LIB_DIR}/external/asio/asio/include)

set(SOURCES 
  "advelement.cpp"
 "aes67client.cpp"
 "aes67mediasession.cpp"
 "AES67ServerMediaSubsession.cpp"
 "aes67source.cpp"
 "anglemeter.cpp"
 "aoipsourcemanager.cpp"
 "audio.cpp"
 "audioevent.cpp"
 "generator.cpp"
 "generatorpluginbuilder.cpp"
 "generatorpluginfactory.cpp"
 "inimanager.cpp"
 "inisection.cpp"
 "iomanager.cpp"
 "levelcalculator.cpp"
 "levelgraph.cpp"
 "levelmeter.cpp"
 "liveaudiosource.cpp"
 "monitorevent.cpp"
 "monitorpluginbuilder.cpp"
 "monitorpluginfactory.cpp"
 "niir.cpp"
 "ondemandaes67mediasubsession.cpp"
 "ondemandpamsubsession.cpp"
 "ondemandstreamer.cpp"
 "ourRTSPClient.cpp"
 "ourSIPClient.cpp"
 "PamTaskScheduler.cpp"
 "PamUsageEnvironment.cpp"
 "pcstats.cpp"
 "pmcontrol.cpp"
 "pmpanel.cpp"
 "ppmtypes.cpp"
 "qos.cpp"
 "r128calculator.cpp"
 "r128thread.cpp"
 "rawvideosource.cpp"
 "rtpserverthread.cpp"
 "rtpthread.cpp"
 "rtsputil.cpp"
 "servicebrowser.cpp"
 "settingevent.cpp"
 "settings.cpp"
 "smpte2110mediasession.cpp"
 "soundcardmanager.cpp"
 "soundfile.cpp"
 "streamclientstate.cpp"
 "testpluginbuilder.cpp"
 "testpluginfactory.cpp"
 "uirect.cpp"
 "wmbutton.cpp"
 "wmedit.cpp"
 "wmfocusmanager.cpp"
 "wmipeditpnl.cpp"
 "wmkeyboard.cpp"
 "wmlabel.cpp"
 "wmline.cpp"
 "wmlist.cpp"
 "wmlistadv.cpp"
 "wmslider.cpp"
 "wmslidewnd.cpp"
 "wmswipebutton.cpp"
 "wmswitcherpanel.cpp"
 "wmtimeedit.cpp"
 "wxsaphandler.cpp"
 "wxsink.cpp"
 "wxtouchscreenhtml.cpp"
 "wxzxposter.cpp"
 "../external/sapserver/src/parser.cpp"
 "../external/sapserver/src/receiver.cpp"
 "../external/sapserver/src/saphandler.cpp"
 "../external/sapserver/src/sapserver.cpp" "../external/sapserver/src/sapserverimpl.cpp" "../external/sapserver/src/sender.cpp" 
 "../external/log/src/log.cpp"
 "../external/dnssd/src/dnssd.cpp"
 "../external/dnssd/src/zcposter.cpp"
 "../external/dnssd/src/gnu/avahibrowser.cpp"
 "../external/dnssd/src/gnu/avahipublisher.cpp")



option(optionPTP "Use PTPMonkey library" OFF)
message (${PROJECT_SOURCE_DIR})
if(optionPTP)
    find_path(PTPMONKEY_DIR NAMES "/include/ptpmonkey.h" PATHS ${PROJECT_SOURCE_DIR}/../external/ptpmonkey)
	
    if((NOT PTPMONKEY_DIR) OR (NOT EXISTS ${PTPMONKEY_DIR}))
	# we couldn't find the header files for FOO or they don't exist
	message("Unable to find PtpMonkey")
    
	# we have a submodule setup for asio, assume it is under external/asio
	# now we need to clone this submodule
	execute_process(COMMAND git submodule update --init -- ../external/ptpmonkey WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
        find_path(PTPMONKEY_DIR NAMES "/include/ptpmonkey.h" PATHS ${PROJECT_SOURCE_DIR}/../external/ptpmonkey)   
    else()
	    message("Found ptpmonkey...")
        message(${PTPMONKEY_DIR})
    endif()
    include_directories(${PTPMONKEY_DIR}/include)
    list(APPEND SOURCES ${PTPMONKEY_DIR}/source/loghander.cpp
 ${PTPMONKEY_DIR}/source/ptpeventloghandler.cpp
 ${PTPMONKEY_DIR}/source/ptpmonkeyhandler.cpp
 ${PTPMONKEY_DIR}/source/ptpstructs.cpp
 ${PTPMONKEY_DIR}/source/timeutils.cpp
 ${PTPMONKEY_DIR}/source/mac.cpp
 ${PTPMONKEY_DIR}/source/ptploghandler.cpp
 ${PTPMONKEY_DIR}/source/ptpmonkeyimplementation.cpp
 ${PTPMONKEY_DIR}/source/receiver.cpp
 ${PTPMONKEY_DIR}/source/ptpclock.cpp
 ${PTPMONKEY_DIR}/source/ptpmonkey.cpp
 ${PTPMONKEY_DIR}/source/ptpparser.cpp
 ${PTPMONKEY_DIR}/source/sender.cpp
 "wxptp.cpp")
    
endif()

add_library(pambase SHARED ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(pambase PRIVATE ${flags})
target_compile_definitions(pambase PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _pambase_ CREATING_DLL _core_ _MSL_STDINT_H NO_OPENSSL ASIO_STANDALONE wxDEBUG_LEVEL=0 DNSSD_DLL LOG_DLL)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(pambase ${wxWidgets_LIBRARIES})

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	#use pkgconfig to find the required libs
	find_package(PkgConfig REQUIRED QUIET)
	
	pkg_search_module(avahi REQUIRED avahi-client IMPORTED_TARGET)
	if(TARGET PkgConfig::avahi)
		message(STATUS "Found avahi")
		target_link_libraries(pambase PkgConfig::avahi)
	endif()
	
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
#target_link_libraries(pambase pthread)


set_target_properties(pambase PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib/)

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS pambase LIBRARY DESTINATION /usr/local/lib/pam2)
endif()
