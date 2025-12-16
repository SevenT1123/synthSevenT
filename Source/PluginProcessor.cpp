/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SevenTAudioProcessor::SevenTAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    synth.clearVoices();
	synth.clearSounds();

    synth.addSound(new SynthSound());
    for (int i = 0; i < 8; ++i) // Add 8 voices
        synth.addVoice(new SynthVoice());

}

SevenTAudioProcessor::~SevenTAudioProcessor()
{
}

//==============================================================================
const juce::String SevenTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SevenTAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SevenTAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SevenTAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SevenTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SevenTAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SevenTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SevenTAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SevenTAudioProcessor::getProgramName (int index)
{
    return {};
}

void SevenTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SevenTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }

    filter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
}

void SevenTAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SevenTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SevenTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            // Osc control
            auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");
			auto& oscVolume = *apvts.getRawParameterValue("OSC1VOLUME");
			auto& oscPhaseOffset = *apvts.getRawParameterValue("OSC1PHASEOFFSET");
			auto& oscDetune = *apvts.getRawParameterValue("OSC1DETUNE");
			auto& oscStereo = *apvts.getRawParameterValue("OSC1STEREO");
			auto& oscPan = *apvts.getRawParameterValue("OSC1PAN");

            // ADSR
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");

            // LFO
            // auto& fmFreq = *apvts.getRawParameterValue("OSC1FMFREQ");
            // auto& fmDepth = *apvts.getRawParameterValue("OSC1FMDEPTH");

            voice->getOscillator().setWaveType(oscWaveChoice);
			voice->getOscillator().setVolume(oscVolume.load()); // Atomic float
			voice->getOscillator().setPhaseOffset(oscPhaseOffset.load()); // Atomic float
			voice->getOscillator().setDetune(oscDetune.load()); // Atomic float
			voice->getOscillator().setStereo(oscStereo.load()); // Atomic float
			voice->getOscillator().setPan(oscPan.load()); // Atomic float
            voice->update(attack.load(), decay.load(), sustain.load(), release.load());     // Atomic float
        }
    }


    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    auto& filterType = *apvts.getRawParameterValue("FILTERTYPE");
    auto& cutoff = *apvts.getRawParameterValue("FILTERFREQ");
    auto& resonance = *apvts.getRawParameterValue("FILTERRES");

    filter.updateParameters(filterType, cutoff, resonance);

    filter.process(buffer);

}

//==============================================================================
bool SevenTAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SevenTAudioProcessor::createEditor()
{
    return new SevenTAudioProcessorEditor (*this);
}

//==============================================================================
void SevenTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

	juce::ValueTree stateCopy = apvts.copyState();
    std::unique_ptr <juce::XmlElement> xml (stateCopy.createXml());
    
	copyXmlToBinary(*xml, destData);
}

void SevenTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

	std::unique_ptr <juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    juce::XmlElement* apvtsXml = nullptr;

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(apvts.state.getType()))
        {
			apvtsXml = xmlState.get();
        }
        else {
			apvtsXml = xmlState->getChildByName(apvts.state.getType());
        }

        juce::ValueTree v(juce::ValueTree::fromXml(*apvtsXml));
        if (v.isValid())
        {
            // Safely replace the APVTS state (updates parameters & listeners)
            apvts.replaceState(v);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SevenTAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SevenTAudioProcessor::createParameters() {
    /*
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 1.0f, 0.5f));

    return { params.begin(), params.end() };
    */

    // Combobox: switch oscillator
    // Attack - float
    // Sustain - float
    // Decay - float
    // Release - float

    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Oscillator selection
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray{ "Sine", "Saw", "Triangle", "Pulse", "H-Pulse", "Q-Pulse", "TriSaw" }, 0, ""));

	// Oscillator parameters
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1VOLUME", "Osc 1 Volume", juce::NormalisableRange<float> {0.0f, 0.125f, 0.0125f}, 0.125f));  
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1PHASEOFFSET", "Osc 1 Phase Offset", juce::NormalisableRange<float> {-juce::MathConstants<float>::pi, juce::MathConstants<float>::pi, 0.01f}, 0.0f));  
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1DETUNE", "Osc 1 Detune", juce::NormalisableRange<float> {-100.0f, 100.0f, 0.01f}, 0.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1STEREO", "Osc 1 Stereo", juce::NormalisableRange<float> {0.0f, 1.0f, 0.01f}, 0.5f)); // 0.0 mono, 1.0 wide
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1PAN", "Osc 1 Pan", juce::NormalisableRange<float> {-1.0f, 1.0f, 0.01f}, 0.0f)); // -1.0 left, 0.0 center, +1.0 right

    // ADSR 
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> {0.0f, 10.0f, 0.01f}, 0.05f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> {0.0f, 10.0f, 0.01f}, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> {0.0f, 1.0f, 0.01f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> {0.0f, 10.0f, 0.01f}, 0.1f));

    // Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray{ "Bypass", "Low-Pass", "Band-Pass", "High-Pass" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERFREQ", "Filter Freq", juce::NormalisableRange<float> {20.0f, 20000.0f, 0.1f, 0.6f}, 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float> {1.0f, 10.0f, 0.1f}, 1.0f));

    return { params.begin(), params.end() };
}