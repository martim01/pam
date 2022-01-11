message(STATUS "Create version")

if(NOT NAMESPACE)
    message(FATAL_ERROR "Namespace not set")
endif()

message(STATUS "Namespace ${NAMESPACE} ")

execute_process(COMMAND git log --pretty=format:'%h' -n 1 OUTPUT_VARIABLE GIT_REV ERROR_QUIET)
execute_process(COMMAND git log --pretty=format:'%ai' -n 1 OUTPUT_VARIABLE GIT_TIME ERROR_QUIET)

# Check whether we got any revision (which isn't
# always the case, e.g. when someone downloaded a zip
# file from Github instead of a checkout)
if ("${GIT_REV}" STREQUAL "")
    set(GIT_REV "N/A")
    set(GIT_DIFF "")
    set(GIT_TAG "N/A")
    set(GIT_BRANCH "N/A")
    set(GIT_DATE "N/A")
else()
    execute_process(COMMAND bash -c "git diff --quiet --exit-code || echo .x" OUTPUT_VARIABLE GIT_DIFF)
    execute_process(COMMAND git describe --exact-match --tags OUTPUT_VARIABLE GIT_TAG ERROR_QUIET)
    execute_process(COMMAND git rev-parse --abbrev-ref HEAD OUTPUT_VARIABLE GIT_BRANCH)

    string(STRIP "${GIT_REV}" GIT_REV)
    string(SUBSTRING "${GIT_REV}" 1 7 GIT_REV)
    string(STRIP "${GIT_DIFF}" GIT_DIFF)
    string(STRIP "${GIT_TAG}" GIT_TAG)
    string(STRIP "${GIT_BRANCH}" GIT_BRANCH)
    string(SUBSTRING "${GIT_TIME}" 1 10 GIT_DATE)
endif()


set(VERSION "#include \"${NAMESPACE}_version.h\"
const char* pml::${NAMESPACE}::GIT_REV=\"${GIT_REV}${GIT_DIFF}\";
const char* pml::${NAMESPACE}::GIT_TAG=\"${GIT_TAG}\";
const char* pml::${NAMESPACE}::GIT_BRANCH=\"${GIT_BRANCH}\";
const char* pml::${NAMESPACE}::GIT_DATE=\"${GIT_DATE}\";
unsigned long pml::${NAMESPACE}::VERSION_MAJOR=${MAJOR};
unsigned long pml::${NAMESPACE}::VERSION_MINOR=${MINOR};
unsigned long pml::${NAMESPACE}::VERSION_PATCH=${PATCH};
const char* pml::${NAMESPACE}::VERSION_STRING=\"${MAJOR}.${MINOR}.${PATCH}-${GIT_REV}${GIT_DIFF}\";
")

set(SRC_FILE ${CMAKE_BINARY_DIR}/src/${NAMESPACE}_version.cpp)
if(EXISTS ${SRC_FILE})
    file(READ ${SRC_FILE} VERSION_)
else()
    set(VERSION_ "")
endif()

if (NOT "${VERSION}" STREQUAL "${VERSION_}")
    message(STATUS "Write to ${SRC_FILE}")
    file(WRITE ${SRC_FILE} "${VERSION}")
endif()

#now do the include file
set(INCLUDE "#ifndef __${NAMESPACE}_VERSION_H__
#define __${NAMESPACE}_VERSION_H__
// These variables are autogenerated and compiled
// into the library by the version.cmake script
namespace pml
{
    namespace ${NAMESPACE}
    {
        extern const char* GIT_TAG;
        extern const char* GIT_REV;
        extern const char* GIT_BRANCH;
        extern const char* GIT_DATE;
        extern unsigned long VERSION_MAJOR;
        extern unsigned long VERSION_MINOR;
        extern unsigned long VERSION_PATCH;
        extern const char* VERSION_STRING;
    }   
}
#endif")

set(INC_FILE ${CMAKE_BINARY_DIR}/include/${NAMESPACE}_version.h)
if(EXISTS ${INC_FILE})
    file(READ ${INC_FILE} INCLUDE_)
else()
    set(INCLUDE_ "")
endif()

if (NOT "${INCLUDE}" STREQUAL "${INCLUDE_}")
    message(STATUS "Write to ${INC_FILE}")
    file(WRITE ${INC_FILE} "${INCLUDE}")
endif()