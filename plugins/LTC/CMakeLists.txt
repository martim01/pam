cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(LTC LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories("../../external/libltc")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")
list(APPEND LTC_SOURCES  "../../external/libltc/decoder.c"
						 "../../external/libltc/encoder.c"
						 "../../external/libltc/ltc.c"
						 "../../external/libltc/timecode.c")
		

add_library(LTC SHARED ${SOURCES} ${LTC_SOURCES} )

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(LTC PRIVATE ${flags})
target_compile_definitions(LTC PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _test_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(LTC ${wxWidgets_LIBRARIES})
target_link_libraries(LTC pambase)

set_target_properties(LTC PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/test")


		
