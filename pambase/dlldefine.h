#pragma once
#define WINVER 0x0600


#ifdef __WXMSW__
    #ifdef CREATING_DLL
        #define PAMBASE_IMPEXPORT __declspec(dllexport)
        #define __PAMBASE__ 1
    #elif WXUSINGDLL
        #define PAMBASE_IMPEXPORT __declspec(dllimport)
    #else
        #define PAMBASE_IMPEXPORT
    #endif
#elif __WXGNU__
    #ifdef CREATING_DLL
        #define PAMBASE_IMPEXPORT __attribute__((visibility("default")))
        #define __PAMBASE__ 1
    #else
        #define PAMBASE_IMPEXPORT
    #endif
#else
    #define PAMBASE_IMPEXPORT
#endif



