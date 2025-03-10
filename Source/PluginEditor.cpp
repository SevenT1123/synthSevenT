/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthSevenTAudioProcessorEditor::SynthSevenTAudioProcessorEditor (SynthSevenTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
    , osc(audioProcessor.apvts, "OSC1WAVETYPE", "OSC1FMFREQ", "OSC1FMDEPTH")
    , adsr("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE")
    , filter(audioProcessor.apvts, "FILTERTYPE", "FILTERFREQ", "FILTERRES")
{
    setSize(620, 500);
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
}

SynthSevenTAudioProcessorEditor::~SynthSevenTAudioProcessorEditor()
{
}

//==============================================================================
void SynthSevenTAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void SynthSevenTAudioProcessorEditor::resized()
{
    const auto paddingX = 5;
    const auto paddingY = 35;
    const auto paddingY2 = 270;

    osc.setBounds(paddingX, paddingY, 300, 200);
    adsr.setBounds(osc.getRight(), paddingY, 250, 200);
    filter.setBounds(paddingX, paddingY2, 300, 200);
}


