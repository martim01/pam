#pragma once

#ifdef __WXMSW__
#ifdef CREATING_DLL
  	#define PAMBASE_IMPEXPORT __declspec(dllexport)
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
