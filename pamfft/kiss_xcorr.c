/*
 * kiss_xcorr.c
 *
 *  Created on: Nov 16, 2013
 *      Author: du
 */

#include <stdlib.h>
#include "kiss_xcorr.h"
#include "_kiss_fft_guts.h"

static int prod(const int *dims, int ndims) {
	int x = 1;
	while (ndims--)
		x *= *dims++;
	return x;
}

void rfft_xcorr(size_t n, const kiss_fft_scalar *x, const kiss_fft_scalar *y,
		kiss_fft_scalar *z, int mode) {
	int freq_len = n / 2 + 1;
	int i;

	// FFT configs
	kiss_fftr_cfg fft_fwd = kiss_fftr_alloc(n, 0, NULL, NULL );
	kiss_fftr_cfg fft_bwd = kiss_fftr_alloc(n, 1, NULL, NULL );

	// Freq-domain data buffer
	kiss_fft_cpx *X = calloc(freq_len, sizeof(kiss_fft_cpx));
	kiss_fft_cpx *Y = calloc(freq_len, sizeof(kiss_fft_cpx));
	kiss_fft_cpx *Z = calloc(freq_len, sizeof(kiss_fft_cpx));

	// Execute FWD_FFT
	kiss_fftr(fft_fwd, x, X);
	kiss_fftr(fft_fwd, y, Y);

	// Multiply in freq-domain
	for (i = 0; i < freq_len; i++)
    {
		if (mode == KISS_XCORR)
		{
			// take conjugate of Y
			Y[i].i = -Y[i].i;
		}
		C_MUL(Z[i], X[i], Y[i]);
	}

	double modulus;
	unsigned int j;
    for(j=0; j< freq_len; j++)
    {
        modulus = hypot(Z[j].r, Z[j].i);
        Z[j].r = Z[j].r/modulus;
        Z[j].i = Z[j].i/modulus;
    }

	// Execute BWD_FFT
	kiss_fftri(fft_bwd, Z, z);

	free(fft_fwd);
	free(fft_bwd);

	free(X);
	free(Y);
	free(Z);
}

/*
 * calculate frequency domain length when doing real-fft
 */
size_t calc_freq_comain_len(const int *dims, int ndims) {
	int dimReal = dims[ndims - 1];
	int dimOther = prod(dims, ndims - 1);
	int freq_len = (dimReal / 2 + 1) * dimOther;
	return freq_len;
}

/*
 * compute nD cross-correlation function with pre-allocated buffer
 */
void rfft_xcorrnd(size_t freq_len, const kiss_fft_scalar *x,
		const kiss_fft_scalar *y, kiss_fft_scalar *z, int mode,
		kiss_fftndr_cfg fft_fwd, kiss_fftndr_cfg fft_bwd, kiss_fft_cpx *X,
		kiss_fft_cpx *Y, kiss_fft_cpx *Z) {
	int i;

	// Execute FWD_FFT
	kiss_fftndr(fft_fwd, x, X);
	kiss_fftndr(fft_fwd, y, Y);

	// Multiply in freq-domain
	for (i = 0; i < freq_len; i++) {
		if (mode == KISS_XCORR) {
			// take conjugate of Y
			Y[i].i = -Y[i].i;
		}

		C_MUL(Z[i], X[i], Y[i]);
	}

	// Execute BWD_FFT
	kiss_fftndri(fft_bwd, Z, z);

}

/*
 * compute nD cross-correlation function with automatic buffer allocation
 */
void rfft_xcorrnd2(const int *dims, int ndims, const kiss_fft_scalar *x,
		const kiss_fft_scalar *y, kiss_fft_scalar *z, int mode) {
	int i;
	int freq_len = calc_freq_comain_len(dims, ndims);

	// FFT configs
	kiss_fftndr_cfg fft_fwd = kiss_fftndr_alloc(dims, ndims, 0, 0, 0);
	kiss_fftndr_cfg fft_bwd = kiss_fftndr_alloc(dims, ndims, 1, 0, 0);

	// Freq-domain data buffer
	kiss_fft_cpx *X = calloc(freq_len, sizeof(kiss_fft_cpx));
	kiss_fft_cpx *Y = calloc(freq_len, sizeof(kiss_fft_cpx));
	kiss_fft_cpx *Z = calloc(freq_len, sizeof(kiss_fft_cpx));

	// Execute cross-correlation calculation
	rfft_xcorrnd(freq_len, x, y, z, mode, fft_fwd, fft_bwd, X, Y, Z);

	// free memory
	free(fft_fwd);
	free(fft_bwd);
	free(X);
	free(Y);
	free(Z);
}

/*
 * compute 2D cross-correlation function with pre-allocated buffer
 */
void rfft_xcorr2d(size_t h, size_t w, const kiss_fft_scalar *x,
		const kiss_fft_scalar *y, kiss_fft_scalar *z, int mode,
		kiss_fftndr_cfg fft_fwd, kiss_fftndr_cfg fft_bwd, kiss_fft_cpx *X,
		kiss_fft_cpx *Y, kiss_fft_cpx *Z) {
	int freq_len = (h / 2 + 1) * w;

	// Execute cross-correlation calculation
	rfft_xcorrnd(freq_len, x, y, z, mode, fft_fwd, fft_bwd, X, Y, Z);

}

/*
 * compute 2D cross-correlation function with automatic buffer allocation
 */
void rfft_xcorr2d2(size_t h, size_t w, const kiss_fft_scalar *x,
		const kiss_fft_scalar *y, kiss_fft_scalar *z, int mode) {
	int ndims = 2;
	int dims[2] = { h, w };

	// Execute cross-correlation calculation
	rfft_xcorrnd2(dims, ndims, x, y, z, mode);
}
