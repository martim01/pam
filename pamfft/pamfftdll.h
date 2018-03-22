#pragma once

#ifdef __WXMSW__
#ifdef CREATING_FFTDLL
  	#define PAMFFT_IMPEXPORT __declspec(dllexport)
#else
    #ifdef WXUSINGDLL
        #define PAMFFT_IMPEXPORT __declspec(dllimport)
    #else
        #define PAMFFT_IMPEXPORT
    #endif
#endif
#else
#define PAMFFT_IMPEXPORT
#endif
