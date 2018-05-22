#pragma once

#ifdef __WXMSW__
#ifdef CREATING_LEVELDLL
  	#define PAMLEVEL_IMPEXPORT __declspec(dllexport)
#else
    #ifdef WXUSINGDLL
        #define PAMLEVEL_IMPEXPORT __declspec(dllimport)
    #else
        #define PAMLEVEL_IMPEXPORT
    #endif
#endif
#else
#define PAMLEVEL_IMPEXPORT
#endif
