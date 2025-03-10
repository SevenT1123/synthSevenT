/*
  ==============================================================================

    OSCComponent.h
    Created: 16 Jan 2025 2:32:55pm
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OSCComponent  : public juce::Component
{
public:
    OSCComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID, juce::String fmFreqID, juce::String fmDepthID);
    ~OSCComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;

    juce::Slider fmFreqSlider;
    juce::Slider fmDepthSlider;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<Attachment> fmFreqAttachment;
    std::unique_ptr<Attachment> fmDepthAttachment;

    juce::Label waveSelectorLabel{ "Wave Type", "Wave Type" };
    juce::Label fmFreqLabel{ "FM Freq", "FM Freq" };
    juce::Label fmDepthLabel{ "FM Depth", "FM Depth" };

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>& attachment);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCComponent)
};
