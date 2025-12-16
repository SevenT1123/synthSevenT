/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SevenTAudioProcessorEditor::SevenTAudioProcessorEditor (SevenTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), osc(audioProcessor.apvts, "OSC1WAVETYPE", "OSC1FMFREQ", "OSC1FMDEPTH", "OSC1VOLUME", "OSC1PHASEOFFSET", "OSC1DETUNE", "OSC1STEREO", "OSC1PAN")
    , adsr("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE")
    , filter(audioProcessor.apvts, "FILTERTYPE", "FILTERFREQ", "FILTERRES")
{
    setSize(1024, 768);
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
}

SevenTAudioProcessorEditor::~SevenTAudioProcessorEditor()
{
}

//==============================================================================
void SevenTAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void SevenTAudioProcessorEditor::resized()
{
    const auto paddingX = 5;
    const auto paddingY = 35;
    const auto paddingY2 = 370;

    osc.setBounds(paddingX, paddingY, 300, 300);
    adsr.setBounds(osc.getRight(), paddingY2, 250, 200);
    filter.setBounds(paddingX, paddingY2, 300, 200);
}
