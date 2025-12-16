/*
  ==============================================================================

    OSCComponent.cpp
    Created: 16 Jan 2025 2:32:55pm
    Author:  Seven T

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OSCComponent.h"

//==============================================================================
OSCComponent::OSCComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID, juce::String fmFreqID, juce::String fmDepthID, 
    juce::String oscVolumeId,
    juce::String oscPhaseOffsetId,
    juce::String oscDetuneId,
    juce::String oscStereoId,
    juce::String oscPanId)
{
    juce::StringArray choices{ "Sine", "Saw", "Triangle", "Pulse", "H-Pulse", "Q-Pulse", "TriSaw"};
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);

    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorID, oscWaveSelector);    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    waveSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    waveSelectorLabel.setFont(15.0f);
    waveSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(waveSelectorLabel);

    // setSliderWithLabel(fmFreqSlider, fmFreqLabel, apvts, fmFreqID, fmFreqAttachment);
    // setSliderWithLabel(fmDepthSlider, fmDepthLabel, apvts, fmDepthID, fmDepthAttachment);
	setSliderWithLabel(oscVolumeSlider, oscVolumeLabel, apvts, oscVolumeId, oscVolumeAttachment);
	setSliderWithLabel(oscPhaseOffsetSlider, oscPhaseOffsetLabel, apvts, oscPhaseOffsetId, oscPhaseOffsetAttachment);
	setSliderWithLabel(oscDetuneSlider, oscDetuneLabel, apvts, oscDetuneId, oscDetuneAttachment);
	setSliderWithLabel(oscStereoSlider, oscStereoLabel, apvts, oscStereoId, oscStereoAttachment);
	setSliderWithLabel(oscPanSlider, oscPanLabel, apvts, oscPanId, oscPanAttachment);

}

OSCComponent::~OSCComponent()
{
}

void OSCComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Oscillator", labelSpace.withX(5), juce::Justification::left);
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void OSCComponent::resized()
{
    const auto startY = 60;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    oscWaveSelector.setBounds(10, startY + 10, 90, 30);
    waveSelectorLabel.setBounds(10, startY - labelYOffset, 90, labelHeight);

    // fmFreqSlider.setBounds(oscWaveSelector.getRight(), startY, sliderWidth, sliderHeight);
    // fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY() - labelYOffset, fmFreqSlider.getWidth(), labelHeight);

    // fmDepthSlider.setBounds(fmFreqSlider.getRight(), startY, sliderWidth, sliderHeight);
    // fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() - labelYOffset, fmDepthSlider.getWidth(), labelHeight);

	oscVolumeSlider.setBounds(oscWaveSelector.getRight(), startY, sliderWidth, sliderHeight);
	oscVolumeLabel.setBounds(oscVolumeSlider.getX(), oscVolumeSlider.getY() - labelYOffset, oscVolumeSlider.getWidth(), labelHeight);

	oscPhaseOffsetSlider.setBounds(oscVolumeSlider.getRight(), startY, sliderWidth, sliderHeight);
	oscPhaseOffsetLabel.setBounds(oscPhaseOffsetSlider.getX(), oscPhaseOffsetSlider.getY() - labelYOffset, oscPhaseOffsetSlider.getWidth(), labelHeight);

    const auto secondRowParam = oscVolumeSlider.getBottom() + labelYOffset + 10;


	oscDetuneSlider.setBounds(3, secondRowParam, sliderWidth, sliderHeight);
	oscDetuneLabel.setBounds(oscDetuneSlider.getX(), oscDetuneSlider.getY() - labelYOffset, oscDetuneSlider.getWidth(), labelHeight);

	oscStereoSlider.setBounds(oscDetuneSlider.getRight(), secondRowParam, sliderWidth, sliderHeight);
	oscStereoLabel.setBounds(oscStereoSlider.getX(), oscStereoSlider.getY() - labelYOffset, oscStereoSlider.getWidth(), labelHeight);

	oscPanSlider.setBounds(oscStereoSlider.getRight(), secondRowParam, sliderWidth, sliderHeight);
	oscPanLabel.setBounds(oscPanSlider.getX(), oscPanSlider.getY() - labelYOffset, oscPanSlider.getWidth(), labelHeight);
}

void OSCComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment) {
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
    