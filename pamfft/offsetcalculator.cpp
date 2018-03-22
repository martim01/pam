#include "offsetcalculator.h"

#include <wx/log.h>
#include <wx/event.h>
#include "kiss_xcorr.h"

DEFINE_EVENT_TYPE(wxEVT_OFFSET_DONE)

using namespace std;

OffsetCalculator::OffsetCalculator(wxEvtHandler* pHandler, const std::vector<kiss_fft_scalar>& vInLeft, const std::vector<kiss_fft_scalar>& vInRight) :
    m_pHandler(pHandler),
    m_vfft_inL(vInLeft),
    m_vfft_inR(vInRight)
{
}

void* OffsetCalculator::Entry()
{
    WindowData(m_vfft_inL);
    WindowData(m_vfft_inR);

    std::vector<kiss_fft_scalar> vfft_out;
    vfft_out.resize(m_vfft_inL.size());

    rfft_xcorr(m_vfft_inL.size(), m_vfft_inL.data(), m_vfft_inR.data(), vfft_out.data(),KISS_XCORR);



    long pos_peak_pos = 0;
    long neg_peak_pos = 0;
    double biggest = vfft_out[0];
    double smallest = vfft_out[0];

    // now search for positive and negative peaks in correlation function
    for(unsigned int n=0; n<vfft_out.size(); n++)
    {
        if(vfft_out[n] > biggest)
        {
            biggest = vfft_out[n];
            pos_peak_pos = n;
        }
        if(vfft_out[n] < smallest)
        {
            smallest = vfft_out[n];
            neg_peak_pos = n;
        }
    }


    int nBlockSize = m_vfft_inL.size();

    int offset =  ((biggest < fabs(smallest)) ?  neg_peak_pos : pos_peak_pos) ;
    if ((unsigned)offset > nBlockSize/2)
    {
        offset = offset - nBlockSize;
    }


    if(m_pHandler)
    {
        wxCommandEvent event(wxEVT_OFFSET_DONE);
        event.SetInt(offset);
        wxPostEvent(m_pHandler, event);
    }
    return NULL;
}

void OffsetCalculator::WindowData(std::vector<kiss_fft_scalar>& vData)
{
	double angle;
	for(size_t i = 0; i < vData.size(); i++)
    {
        angle = (2*M_PI)*i/(vData.size()-1);
        vData[i] *= 0.5*(1.0 - cos(angle));
    }

}


/*
int i,size = 12;
    int isinverse = 1;
    float buf[size];
    float array[] = {0.1, 0.6, 0.1, 0.4, 0.5, 0, 0.8, 0.7, 0.8, 0.6, 0.1,0};

    kiss_fft_cpx out_cpx[size],out[size],*cpx_buf;

    kiss_fftr_cfg fft = kiss_fftr_alloc(size*2 ,0 ,0,0);
    kiss_fftr_cfg ifft = kiss_fftr_alloc(size*2,isinverse,0,0);

    cpx_buf = copycpx(array,size);
    kiss_fftr(fft,(kiss_fft_scalar*)cpx_buf, out_cpx);
    kiss_fftri(ifft,out_cpx,(kiss_fft_scalar*)out );

    printf("Input:    tOutput:n");
    for(i=0;i<size;i++)
    {
	buf[i] = (out[i].r)/(size*2);
	printf("%ft%fn",array[i],buf[i]);
    }

    kiss_fft_cleanup();
    free(fft);
    free(ifft);


    */
