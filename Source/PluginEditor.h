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
class SevenTAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SevenTAudioProcessorEditor (SevenTAudioProcessor&);
    ~SevenTAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SevenTAudioProcessor& audioProcessor;
    OSCComponent osc;
    ADSRComponent adsr;
    FilterComponent filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SevenTAudioProcessorEditor)
};
