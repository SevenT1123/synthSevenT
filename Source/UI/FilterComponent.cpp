/*
  ==============================================================================

    FilterComponent.cpp
    Created: 23 Jan 2025 8:15:54pm
    Author:  Seven T

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorID, juce::String filterFreqID, juce::String filterResID)
{
    juce::StringArray choices{ "Low-Pass", "Band-Pass", "High-Pass" };
    filterTypeSelector.addItemList(choices, 1);
    addAndMakeVisible(filterTypeSelector);

    filterTypeSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeSelectorID, filterTypeSelector);
    
    setSliderWithLabel(filterFreqSlider, filterFreqLabel, apvts, filterFreqID, filterFreqAttachment);
    setSliderWithLabel(filterResSlider, filterResLabel, apvts, filterResID, filterResAttachment);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void FilterComponent::resized()
{
    const auto sliderPosY = 55;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    filterTypeSelector.setBounds(10, 0, 90, 30);
    filterFreqSlider.setBounds(10, sliderPosY, 90, sliderHeight);
    filterFreqLabel.setBounds(filterFreqSlider.getX(), filterFreqSlider.getY() - labelYOffset, filterFreqSlider.getWidth(), labelHeight);

    filterResSlider.setBounds(filterFreqSlider.getRight(), sliderPosY, sliderWidth, sliderHeight);
    filterResLabel.setBounds(filterResSlider.getX(), filterResSlider.getY() - labelYOffset, filterResSlider.getWidth(), labelHeight);
}

void FilterComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment) {
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}