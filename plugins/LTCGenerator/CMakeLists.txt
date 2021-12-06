cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(LTCGenerator LANGUAGES CXX C)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories("../../external/libltc")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")
list(APPEND LTC_SOURCES  "../../external/libltc/decoder.c"
						 "../../external/libltc/encoder.c"
						 "../../external/libltc/ltc.c"
						 "../../external/libltc/timecode.c")
		

add_library(LTCGenerator MODULE ${SOURCES} ${LTC_SOURCES} )

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive"  "-std=c++14")

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(LTCGenerator PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _generator_ MSL_STDINT_H wxDEBUG_LEVEL=0)
   set_target_properties(LTCGenerator PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/generator")
else()
   list(APPEND flags "-g")
   target_compile_definitions(LTCGenerator PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _generator_ MSL_STDINT_H)
   set_target_properties(LTCGenerator PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/generator/debug")
endif()
target_compile_options(LTCGenerator PRIVATE ${flags})

target_link_libraries(LTCGenerator ${wxWidgets_LIBRARIES})
target_link_libraries(LTCGenerator optimized pambase debug pambased)


#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS LTCGenerator LIBRARY DESTINATION /usr/local/lib/pam2/monitor)
endif()	
