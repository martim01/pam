#pragma once
#define WINVER 0x0600
#include <wx/string.h>

#ifdef __WXMSW__
#ifdef CREATING_DLL
  	#define PAMBASE_IMPEXPORT __declspec(dllexport)
  	#define __PAMBASE__ 1
#else
    #ifdef WXUSINGDLL
        #define PAMBASE_IMPEXPORT __declspec(dllimport)
    #else
        #define PAMBASE_IMPEXPORT
    #endif
#endif
#else
#define PAMBASE_IMPEXPORT
#endif



