/*
  ==============================================================================

    ADSRData.h
    Created: 14 Jan 2025 12:41:58am
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ADSRData : public juce::ADSR {
    public:
        void updateADSR(const float attack, const float decay, const float sustain, const float release);

    private:
        juce::ADSR::Parameters adsrParameters;
};