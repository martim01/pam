cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pamfft LANGUAGES CXX C)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../pambase/")
include_directories("../external/kiss_fft130")
include_directories("../external/kiss_fft130/tools")
link_directories("../lib")


file(GLOB SOURCES "*.cpp" "../external/kiss_fft130/kiss_fft.c" "../external/kiss_fft130/tools/kfc.c" "../external/kiss_fft130/tools/kiss_fftnd.c" "../external/kiss_fft130/tools/kiss_fftndr.c" "../external/kiss_fft130/tools/kiss_fftr.c"	"kiss_xcorr.c")

add_library(pamfft SHARED ${SOURCES})
#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


list(APPEND flags "-fPIC" "-Wall" "-fpermissive"  "-std=c++14")

if(CMAKE_BUILD_TYPE MATCHES Release)
   list(APPEND flags "-O3")
   target_compile_definitions(pamfft PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _core_ MSL_STDINT_H wxDEBUG_LEVEL=0)
else()
   list(APPEND flags "-g")
   target_compile_definitions(pamfft PUBLIC __WXDEBUG__ DLL_EXPORTS wxUSE_UNICODE __GNU__ __WXGNU__ _core_ MSL_STDINT_H)
endif()
target_compile_options(pamfft PRIVATE ${flags})

target_link_libraries(pamfft ${wxWidgets_LIBRARIES})
target_link_libraries(pamfft optimized pambase debug pambased)

set_target_properties(pamfft PROPERTIES DEBUG_POSTFIX "d")




set_target_properties(pamfft PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../lib/")

#linux specific
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	install(TARGETS pamfft LIBRARY DESTINATION /usr/local/lib/pam2)
endif()
		
