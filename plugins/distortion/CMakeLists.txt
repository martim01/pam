cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(distortion LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include_directories("../../pambase/")
include_directories("../../pamfft/")
include_directories("../../external/kiss_fft130")
include_directories("../../external/kiss_fft130/tools")

link_directories("../../lib")


file(GLOB SOURCES "*.cpp")

add_library(distortion SHARED ${SOURCES})

list(APPEND flags "-fPIC" "-Wall" "-fpermissive" "-O3")
target_compile_options(distortion PRIVATE ${flags})
target_compile_definitions(distortion PUBLIC NDEBUG DLL_EXPORTS wxUSE_UNICODE __WXGNU__ _test_ _MSL_STDINT_H)

#find external libraries
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})


target_link_libraries(distortion ${wxWidgets_LIBRARIES})
target_link_libraries(distortion pambase)
target_link_libraries(distortion pamfft)

set_target_properties(distortion PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../../../lib/test")


		
