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
}

// Add phase offset support to your waveform initialiser!
void OSCData::setWaveType(const int choice) {
    switch (choice) {
    case 0:
        // sine wave
        initialise([this](float x) { return std::sin(x + phaseOffset); });
        break;
    case 1:
		// saw wave
        initialise([this](float x) { return (x + phaseOffset) / juce::MathConstants<float>::pi; });
        break;
    case 2:
		// triangle wave
        initialise([this](float x) { return (2.0f / juce::MathConstants<float>::pi) * std::asin(std::sin(x + phaseOffset)); });
        break;
    case 3:
		// pulse wave
        initialise([this](float x) { return (x + phaseOffset) < 0.0f ? -1.0f : 1.0f; });
        break;
    case 4:
		// half pulse wave
        initialise([this](float x) { return (x + phaseOffset) < -juce::MathConstants<float>::pi * 0.5f ? -1.0f : 1.0f; });
        break;
    case 5:
		// quarter pulse wave
        initialise([this](float x) { return (x + phaseOffset) < -juce::MathConstants<float>::pi * 0.25f ? -1.0f : 1.0f; });
        break;
    case 6:
		// triangle wave (custom)
        initialise([this](float x) {
            float phase = ((x + phaseOffset) + juce::MathConstants<float>::pi) / (2.0f * juce::MathConstants<float>::pi);
            if (phase < 0.5f)
                return 4.0f * phase - 1.0f;
            else
                return 3.0f - 4.0f * phase;
            });
        break;
    default:
        jassertfalse;
        break;
    }
}

void OSCData::setWaveFrequency(const int midiNoteNumber) {
    // Detune in cents: 1 cent = 1/100 of a semitone
    float freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    if (cents != 0.0f)
        freq *= std::pow(2.0f, cents / 1200.0f);
    setFrequency(freq);
    lastMidiNote = midiNoteNumber;
}

void OSCData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    process(juce::dsp::ProcessContextReplacing<float>(block));

    auto numChannels = block.getNumChannels();
    auto numSamples = block.getNumSamples();

    // Volume, stereo spread, pan
    for (int channel = 0; channel < numChannels; ++channel) {
        float panGain = 1.0f;
        if (numChannels == 2) {
            // Stereo algorithm: pan and spread
            float basePan = pan;
            float spread = stereo * (channel == 0 ? -1.0f : 1.0f); // L = -spread, R = +spread
            float finalPan = juce::jlimit(-1.0f, 1.0f, basePan + spread);

            // Equal-power pan law
            panGain = std::cos(juce::MathConstants<float>::halfPi * (finalPan + 1.0f) / 2.0f);
            if (channel == 1) // right
                panGain = std::sin(juce::MathConstants<float>::halfPi * (finalPan + 1.0f) / 2.0f);
        }

        auto* data = block.getChannelPointer((size_t)channel);
        for (int i = 0; i < numSamples; ++i)
            data[i] *= (volume * panGain);
    }
}

void OSCData::setVolume(float volume) {
	this->volume = volume;
}

void OSCData::setPhaseOffset(float phaseOffset) {
    this->phaseOffset = phaseOffset;
}

void OSCData::setDetune(float cents) {
    this->cents = cents;
}
void OSCData::setStereo(float stereo) {
    this->stereo = stereo;
}

void OSCData::setPan(float pan) {
    this->pan = pan;
}

float OSCData::getVolume() const {
	return volume;
}

float OSCData::getPhaseOffset() const {
	return phaseOffset;
}

float OSCData::getDetune() const {
	return cents;
}

float OSCData::getStereo() const {
	return stereo;
}

float OSCData::getPan() const {
	return pan;
}