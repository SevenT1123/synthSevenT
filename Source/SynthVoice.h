/*
  ==============================================================================

    SynthVoice.h
    Created: 8 Jan 2025 6:20:01pm
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/ADSRData.h"
#include "Data/OSCData.h"
#include "Data/FilterData.h"

class SynthVoice : public juce::SynthesiserVoice {
    public:
        bool canPlaySound(juce::SynthesiserSound* sound) override;
        void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
        void stopNote(float velocity, bool allowTailOff) override;
        void controllerMoved(int controllerNumber, int newControllerValue) override;
        void pitchWheelMoved(int newPitchWheelValue) override;
        void prepareToPlay(double sampleRate, int samplePerBlock, int outputChannels);
        void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
        void update(const float attack, const float decay, const float sustain, const float release);
        OSCData& getOscillator() { return osc; }
        ADSRData& getAdsr() { return adsr; }
        FilterData& getFilter() { return filter; }

    private:
        juce::AudioBuffer<float> synthBuffer;

        OSCData osc;
        ADSRData adsr;
        FilterData filter;
        // juce::dsp::Oscillator<float> osc{ [](float x) {return x / juce::MathConstants<float>::pi;}, 200 };
        // return std::sin(x) // Sine wave
        // return x / juce::MathConstants<float>::pi; // Saw wave
        // return x < 0.0f ? -1.0f : 1.0f; // Square wave;
        juce::dsp::Gain<float> gain;

        bool isPrepared{ false };


};