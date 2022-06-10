#include "noise.h"
#include "log.h"


using namespace std;

const double GreyANoise::AFILTER_B[7] = {0.234301792299513,  -0.468603584599026,  -0.234301792299513, 0.937207169198054,  -0.234301792299515,  -0.468603584599025,   0.234301792299513};
const double GreyANoise::AFILTER_A[6] = {-4.113043408775871,   6.553121752655047, -4.990849294163381,1.785737302937573,  -0.246190595319487,   0.011224250033231};

const double GreyNoise::ANFILTER_B[3] = { 0.23614050038447681, -0.3682612669579237, 0.14511161746470033};
const double GreyNoise::ANFILTER_A[2] = {-0.3682612669579237, -0.6187478821508299};


double randn(double mu, double sigma)
{/* Generates additive white Gaussian Noise samples with zero mean and a standard deviation of 1. */

  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;

  if (call == 1)
    {
      call = !call;
      return (mu + sigma * (double) X2);
    }

  do
    {
      U1 = -1 + ((double) rand () / RAND_MAX) * 2;
      U2 = -1 + ((double) rand () / RAND_MAX) * 2;
      W = pow (U1, 2) + pow (U2, 2);
    }
  while (W >= 1 || W == 0);

  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;

  call = !call;

  return (mu + sigma * (double) X1);
}


unsigned long GenerateRandomNumber( void )
{
    static unsigned long randSeed = 22222;
    randSeed = (randSeed * 196314165) + 907633515;
    return randSeed;
}

void Noise::SetAmplitude(float ddBFS)
{
    m_dNoiseAmplitude = pow(10.0, ddBFS/20.0);
}


WhiteNoise::WhiteNoise()
{
    srand(time(0));
}

WhiteNoise::~WhiteNoise()
{

}

float WhiteNoise::ProduceSample()
{
    return randn(0,0.4)*m_dNoiseAmplitude;
}



PinkNoise::PinkNoise()
{
    Initialize(30);
}

PinkNoise::~PinkNoise()
{
}

float PinkNoise::ProduceSample()
{
    long newRandom;
    long sum;
    float output;

    /* Increment and mask index. */
    m_pinkIndex = (m_pinkIndex + 1) & m_pinkIndexMask;
    /* If index is zero, don't update any random values. */
    if( m_pinkIndex != 0 )
    {
        /* Determine how many trailing zeros in PinkIndex. */
        /* This algorithm will hang if n==0 so test first. */
        int numZeros = 0;
        int n = m_pinkIndex;
        while( (n & 1) == 0 )
        {
            n = n >> 1;
            numZeros++;
        }
        /* Replace the indexed ROWS random value.
         * Subtract and add back to RunningSum instead of adding all the random
         * values together. Only one changes each time.
         */
        m_pinkRunningSum -= m_pinkRows[numZeros];
        newRandom = ((long)GenerateRandomNumber()) >> PINK_RANDOM_SHIFT;
        m_pinkRunningSum += newRandom;
        m_pinkRows[numZeros] = newRandom;
    }

    /* Add extra white noise value. */
    newRandom = ((long)GenerateRandomNumber()) >> PINK_RANDOM_SHIFT;
    sum = m_pinkRunningSum + newRandom;
    /* Scale to range of -1.0 to 0.9999. */
    output = m_pinkScalar * sum;

    return output*m_dNoiseAmplitude;

}

void PinkNoise::Initialize(int numRows )
{
    int i;
    long pmax;
    m_pinkIndex = 0;
    m_pinkIndexMask = (1<<numRows) - 1;

    /* Calculate maximum possible signed random value. Extra 1 for white noise always added. */
    pmax = (numRows + 1) * (1<<(PINK_RANDOM_BITS-1));
    m_pinkScalar = 1.0f / pmax;
    /* Initialize rows. */
    for( i=0; i<numRows; i++ )
    {
        m_pinkRows[i] = 0;
    }
    m_pinkRunningSum = 0;
}


GreyNoise::GreyNoise() : m_filter(ANFILTER_A, ANFILTER_B, 2, 3)
{

}

GreyNoise::~GreyNoise()
{

}

float GreyNoise::ProduceSample()
{
    return m_filter.Filter(WhiteNoise::ProduceSample())*2.0;
}


GreyANoise::GreyANoise() : m_filter(AFILTER_A, AFILTER_B, 6, 7)
{

}

GreyANoise::~GreyANoise()
{

}

float GreyANoise::ProduceSample()
{
    return m_filter.Filter(WhiteNoise::ProduceSample());
}


GreyKNoise::GreyKNoise() : m_filter()
{

}

GreyKNoise::~GreyKNoise()
{

}

float GreyKNoise::ProduceSample()
{
    return m_filter.Filter(WhiteNoise::ProduceSample())*2.0;
}



