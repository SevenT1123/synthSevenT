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
OSCComponent::OSCComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID, juce::String fmFreqID, juce::String fmDepthID)
{
    juce::StringArray choices{ "Sine", "Saw", "Square" };
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);

    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorID, oscWaveSelector);    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    waveSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    waveSelectorLabel.setFont(15.0f);
    waveSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(waveSelectorLabel);

    setSliderWithLabel(fmFreqSlider, fmFreqLabel, apvts, fmFreqID, fmFreqAttachment);
    setSliderWithLabel(fmDepthSlider, fmDepthLabel, apvts, fmDepthID, fmDepthAttachment);
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
    const auto startY = 55;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    oscWaveSelector.setBounds(10, startY + 5, 90, 30);
    waveSelectorLabel.setBounds(10, startY - labelYOffset, 90, labelHeight);

    fmFreqSlider.setBounds(oscWaveSelector.getRight(), startY, sliderWidth, sliderHeight);
    fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY() - labelYOffset, fmFreqSlider.getWidth(), labelHeight);

    fmDepthSlider.setBounds(fmFreqSlider.getRight(), startY, sliderWidth, sliderHeight);
    fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() - labelYOffset, fmDepthSlider.getWidth(), labelHeight);
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