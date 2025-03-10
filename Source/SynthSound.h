/*
  ==============================================================================

    SynthSounds.h
    Created: 8 Jan 2025 6:20:28pm
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound {
    public: 
        bool appliesToNote(int midiToNoteNumber) override {     // virtual function
            return true;
        }

        bool appliesToChannel(int midiChannel) override {       // virtual function
            return true;
        }
}; 