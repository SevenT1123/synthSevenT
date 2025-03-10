/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/ADSRComponent.h"
#include "UI/OSCComponent.h"
#include "UI/FilterComponent.h"

//==============================================================================
/**
*/
class SynthSevenTAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthSevenTAudioProcessorEditor (SynthSevenTAudioProcessor&);
    ~SynthSevenTAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SynthSevenTAudioProcessor& audioProcessor;
    OSCComponent osc;
    ADSRComponent adsr;
    FilterComponent filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthSevenTAudioProcessorEditor)
};
