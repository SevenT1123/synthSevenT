/*
  ==============================================================================

    OSCData.h
    Created: 16 Jan 2025 1:56:01pm
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OSCData : juce::dsp::Oscillator<float> {
    public:
        void prepareToPlay(juce::dsp::ProcessSpec& spec);
        void setWaveType(const int choice);
        void setWaveFrequency(const int midiNoteNumber);
        void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
        void updateFMParams(const float freq, const float depth);
    private:
        void processFMOSC(juce::dsp::AudioBlock<float>& block);
        juce::dsp::Oscillator<float> fmOSC{ [](float x) {return std::sin(x);} };
        float fmMod{ 0.0f };
        float fmDepth{ 0.0f };
        int lastMidiNote{ 0 };
};