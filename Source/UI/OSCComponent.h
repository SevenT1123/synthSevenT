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
    OSCComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID, juce::String fmFreqID, juce::String fmDepthID, 
        juce::String oscVolumeId, 
        juce::String oscPhaseOffsetId, 
        juce::String oscDetuneId, 
        juce::String oscStereoId, 
        juce::String oscPanId);
    ~OSCComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;

    // juce::Slider fmFreqSlider;
    // juce::Slider fmDepthSlider;
    juce::Slider oscVolumeSlider;
    juce::Slider oscPhaseOffsetSlider;
    juce::Slider oscDetuneSlider;
    juce::Slider oscStereoSlider;
    juce::Slider oscPanSlider;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    // std::unique_ptr<Attachment> fmFreqAttachment;
    // std::unique_ptr<Attachment> fmDepthAttachment;
    std::unique_ptr<Attachment> oscVolumeAttachment;
    std::unique_ptr<Attachment> oscPhaseOffsetAttachment;
    std::unique_ptr<Attachment> oscDetuneAttachment;
    std::unique_ptr<Attachment> oscStereoAttachment;
    std::unique_ptr<Attachment> oscPanAttachment;

    juce::Label waveSelectorLabel{ "Wave Type", "Wave Type" };

    // juce::Label fmFreqLabel{ "FM Freq", "FM Freq" };
    // juce::Label fmDepthLabel{ "FM Depth", "FM Depth" };
    juce::Label oscVolumeLabel{ "Volume", "Volume" };
    juce::Label oscPhaseOffsetLabel{ "Phase Offset", "Phase Offset" };
    juce::Label oscDetuneLabel{ "Detune", "Detune" };
    juce::Label oscStereoLabel{ "Stereo", "Stereo" };
    juce::Label oscPanLabel{ "Pan", "Pan" };

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>& attachment);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCComponent)
};
