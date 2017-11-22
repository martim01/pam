/*
 * kiss_xcorr.h
 *
 *  Created on: Nov 16, 2013
 *      Author: du
 */

#ifndef KISS_XCORR_H
#define KISS_XCORR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "kiss_fftndr.h"

enum
{
    KISS_CONV = 0,
    KISS_XCORR,
};

/*
 * calculate frequency domain length when doing real-fft
 */
size_t calc_freq_comain_len(const int *dims, int ndims);

/*
 * compute 1D cross-correlation function with automatic buffer allocation
 */
void rfft_xcorr(size_t n, const kiss_fft_scalar *x, const kiss_fft_scalar *y, kiss_fft_scalar *z, int mode);

/*
 * compute 2D cross-correlation function with pre-allocated buffer
 */
void rfft_xcorr2d(size_t h, size_t w, const kiss_fft_scalar *x, const kiss_fft_scalar *y, kiss_fft_scalar *z, int mode, kiss_fftndr_cfg fft_fwd, kiss_fftndr_cfg fft_bwd, kiss_fft_cpx *X, kiss_fft_cpx *Y, kiss_fft_cpx *Z);

/*
 * compute 2D cross-correlation function with automatic buffer allocation
 */
void rfft_xcorr2d2(size_t h, size_t w, const kiss_fft_scalar *x, const kiss_fft_scalar *y, kiss_fft_scalar *z, int mode);


/*
 * compute nD cross-correlation function with pre-allocated buffer
 */
void rfft_xcorrnd(size_t freq_len, const kiss_fft_scalar *x,
                  const kiss_fft_scalar *y, kiss_fft_scalar *z, int mode, kiss_fftndr_cfg fft_fwd,
                  kiss_fftndr_cfg fft_bwd, kiss_fft_cpx *X, kiss_fft_cpx *Y,
                  kiss_fft_cpx *Z);

/*
 * compute nD cross-correlation function with automatic buffer allocation
 */
void rfft_xcorrnd2(const int *dims, int ndims, const kiss_fft_scalar *x,
                   const kiss_fft_scalar *y, kiss_fft_scalar *z, int mode);


#ifdef __cplusplus
}
#endif

#endif /* KISS_XCORR_H */
