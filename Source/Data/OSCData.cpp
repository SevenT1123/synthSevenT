/*
  ==============================================================================

    OSCData.cpp
    Created: 16 Jan 2025 1:56:01pm
    Author:  Seven T

  ==============================================================================
*/

#include "OSCData.h"

void OSCData::prepareToPlay(juce::dsp::ProcessSpec& spec) {
    prepare(spec);
    fmOSC.prepare(spec);
}


void OSCData::setWaveType(const int choice) {
    // return std::sin(x) // Sine wave
    // return x / juce::MathConstants<float>::pi; // Saw wave
    // return x < 0.0f ? -1.0f : 1.0f; // Square wave;
    
    switch (choice) {
        case 0:
            initialise([](float x) {return std::sin(x);});
            // sine wave
            break;
        case 1:
            initialise([](float x) {return x / juce::MathConstants<float>::pi;});
            // saw wave
            break;
        case 2:
            initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f;});
            // square wave
            break;
        default:
            jassertfalse;
            break;
    }
}

void OSCData::setWaveFrequency(const int midiNoteNumber) {
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    lastMidiNote = midiNoteNumber;
}

void OSCData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {
    processFMOSC(block);
    process(juce::dsp::ProcessContextReplacing<float>(block));
}

void OSCData::processFMOSC(juce::dsp::AudioBlock<float>& block) {
    for (int channel = 0; channel < block.getNumChannels(); ++channel) {
        for (int sample = 0; sample < block.getNumSamples(); ++sample) {
            fmMod = fmOSC.processSample(block.getSample(channel, sample)) * fmDepth;
        }
    }
}

void OSCData::updateFMParams(const float freq, const float depth) {
    fmOSC.setFrequency(freq);
    fmDepth = depth;
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod;
    setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
}