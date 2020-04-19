#ifndef FIRFILTER_H_INCLUDED
#define FIRFILTER_H_INCLUDED

#include "JuceHeader.h"

class FIRFilter
{
public:
    FIRFilter (int order) :
        order (order)
    {
        h = new float[order];
        z = new float[order];
    }

    ~FIRFilter()
    {
        delete[] h;
        delete[] z;
    }

    void reset()
    {
        zPtr = 0;
        for (int n = 0; n < order; ++n)
            z[n] = 0.0f;
    }

    void setCoefs (float* coefs)
    {
        for (int n = 0; n < order; ++n)
            h[n] = coefs[n];
    }

    inline void process (float* buffer, int numSamples)
    {
        float y = 0.0f;
        for (int n = 0; n < numSamples; ++n)
        {
            y = 0.0f;
            z[zPtr] = buffer[n];

            for (int m = 0; m < order; ++m)
            {
                int idx = zPtr - m;
                idx = (idx < 0) ? idx + order : idx;

                y += h[m] * z[idx];
            }

            buffer[n] = y;
            zPtr = (zPtr + 1) % order;
        }
    }

    inline void processBypassed (float* buffer, int numSamples)
    {
        for (int n = 0; n < numSamples; ++n)
        {
            z[zPtr] = buffer[n];
            zPtr = (zPtr + 1) % order;
        }
    }

protected:
    float* h;
    const int order;

private:
    float* z;
    int zPtr = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FIRFilter)
};

#endif //FIRFILTER_H_INCLUDED