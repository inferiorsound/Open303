#pragma once
#include "rosic_OnePoleFilter.h"

namespace rosic
{

class AccentEnvelope
{
public:

    AccentEnvelope()
    {
        vcaFilter.setCutoff(100.);
    }

    void reset()
    {
        cutoffEnv.reset();
        vcaFilter.reset();
        vcaOut = 0.;
        cutoffShift = 0.;
    }

    void setAccent(double accentNormalized)
    {
        accent = accentNormalized;
    }

    void setResonance(double resoNormalized)
    {
        cutoffEnv.setResonance(resoNormalized);
    }

    void setSampleRate(double newSampleRate)
    {
        sampleRate = newSampleRate;
        vcaFilter.setSampleRate(sampleRate);
        cutoffEnv.setSampleRate(sampleRate);
    }

    void tick(double filterEnvValue, bool accentEn)
    {
        filterEnvValue *= accentEn ? accent : 0.;
        vcaOut = vcaFilter.getSample(filterEnvValue) * maxAccExtraGain;
        cutoffShift = cutoffEnv.getSample(filterEnvValue);
    }

    double getAccentVCABoost() const
    {
        return 1. + vcaOut;
    }

    double getCutoffShift() const
    {
        return exp(accent * cutoffShift);
    }

private:

    struct CutoffEnv
    {
    public:
        void setSampleRate(double newSampleRate)
        {
            sampleRate = newSampleRate;
            updateCoefficients();
        }

        void setResonance(double resoNormalized)
        {
            reso = resoNormalized;
            updateCoefficients();
        }

        void reset()
        {
            value = 0.;
        }

        double getSample(double in)
        {
            value *= c;
            if (in > value)
                value += a * (in - value);

            return value;
        }

    private:
        void updateCoefficients()
        {
            const auto decay = 0.3*(100e3 + reso * 100e3) * 1e-6;
            const auto attack = 0.7 * (100e3 * 1e-6);
            c = exp(-1. / (decay * sampleRate));
            a = 1. - exp(-1. / (attack * sampleRate));
        }
        double sampleRate = 0.;
        double reso = 0.;
        double value = 0.;
        double a = 0.;
        double c = 0.;
    };


    OnePoleFilter vcaFilter;
    CutoffEnv cutoffEnv;

    double accent = 0.;
    double sampleRate = 0.;
    
    double vcaOut = 0.;
    double maxAccExtraGain = 2.4;

    double cutoffShift = 0;


};

}