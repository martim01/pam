cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pamfft LANGUAGES CXX C)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../pambase/")
include_directories("../externak/kiss_fft130")
include_directories("../externak/kiss_fft130/tools")
link_directories("../lib")


file(GLOB SOURCES "*.cpp" "../external/kiss_fft130/kiss_fft.c" "../external/kiss_fft130/tools/kfc.c" "../external/kiss_fft130/tools/kiss_fftnd.c" "../external/kiss_fft130/tools/kiss_fftndr.c" "../external/kiss_fft130/tools/kiss_fftr.c"	"kiss_xcorr.c")

add_library(pamfft SHARED ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(pamfft PRIVATE ${flags})
target_compile_definitions(pamfft PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ CREATING_LEVELDLL _core_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

target_link_libraries(pamfft ${wxWidgets_LIBRARIES})
target_link_libraries(pamfft pambase)

set_target_properties(pamfft PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../lib/")


		
