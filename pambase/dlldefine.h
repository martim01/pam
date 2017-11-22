#pragma once

#ifdef CREATING_DLL
  	#define PAMBASE_IMPEXPORT __declspec(dllexport)
#else
    #ifdef WXUSINGDLL
        #define PAMBASE_IMPEXPORT __declspec(dllimport)
    #else
        #define PAMBASE_IMPEXPORT
    #endif
#endif
