cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pam2 LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../pambase/")
include_directories("../external/live/BasicUsageEnvironment/include")
include_directories("../external/live/groupsock/include")
include_directories("../external/live/liveMedia/include")
include_directories("../external/live/UsageEnvironment/include")
include_directories("images/")
include_directories(${DIR_LOG}/include)
include_directories(${DIR_PTPMONKEY}/include)

if(${NMOS})
	include_directories(${DIR_NMOS}/include)
endif()
link_directories("../lib")

file(GLOB SOURCES "*.cpp")

add_executable(pam2 ${SOURCES})


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

target_link_libraries(pam2 ${wxWidgets_LIBRARIES})
target_link_libraries(pam2 pambase)
target_link_libraries(pam2 pml_log)
target_link_libraries(pam2 pml_dnssd)
target_link_libraries(pam2 ptpmonkey)
target_link_libraries(pam2 pthread)

if(${NMOS})
	target_link_libraries(pam2 restgoose)
	target_link_libraries(pam2 nmos_base)
	target_link_libraries(pam2 nmos_node)
	target_link_libraries(pam2 nmos_client)
endif()

set_target_properties(pam2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../../bin/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
install(TARGETS pam2 RUNTIME DESTINATION /usr/local/bin)
execute_process(COMMAND setcap cap_sys_time,cap_net_bind_service+ep /usr/local/bin/pam2 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()
