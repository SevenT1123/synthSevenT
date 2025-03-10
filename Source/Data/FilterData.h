/*
  ==============================================================================

    FilterData.h
    Created: 23 Jan 2025 7:31:57pm
    Author:  Seven T

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FilterData {
    public:
        void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
        void process(juce::AudioBuffer<float>& buffer);
        void updateParameters(const int filterType, const float frequency, const float resonance);
        void reset();
    private:
        juce::dsp::StateVariableTPTFilter<float> filter;
        bool isPrepared{ false };
};
