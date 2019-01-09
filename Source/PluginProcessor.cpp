/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChowAudioProcessor::ChowAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter (thresh = new AudioParameterFloat (String ("thresh"), String ("Threshold"),
                                                    -100.0f, 0.0f, -27.0f));
    
    addParameter (ratio = new AudioParameterFloat (String ("ratio"), String ("Ratio"),
                                                   0.0f, 20.0f, 10.0f));

    addParameter (inGaindB = new AudioParameterFloat (String ("inGaindB"), String ("Input Gain"),
                                                      -30.0f, 6.0f, 0.0f));

    addParameter (outGaindB = new AudioParameterFloat (String ("outGaindB"), String ("Output Gain"),
                                                       -30.0f, 6.0f, 0.0f));
}

ChowAudioProcessor::~ChowAudioProcessor()
{
}

//==============================================================================
const String ChowAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChowAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChowAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChowAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChowAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChowAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChowAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChowAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String ChowAudioProcessor::getProgramName (int /*index*/)
{
    return {};
}

void ChowAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void ChowAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    setRateAndBufferSizeDetails (sampleRate, samplesPerBlock);
}

void ChowAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChowAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

float ChowAudioProcessor::chow (float x)
{
    float y = x;

    const float threshGain = Decibels::decibelsToGain (thresh->get());
    if (x > threshGain)
    {
        y = threshGain + ((x - threshGain) / ratio->get());
    }

    //set y
    return y;
}

void ChowAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& /*midiMessages*/)
{
    ScopedNoDenormals noDenormals;

    buffer.applyGain (Decibels::decibelsToGain (inGaindB->get()));

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* x = buffer.getWritePointer (channel);
        for (int n = 0; n < buffer.getNumSamples(); n++)
            x[n] = chow (x[n]);
    }

    buffer.applyGain (Decibels::decibelsToGain (outGaindB->get()));
}

//==============================================================================
bool ChowAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ChowAudioProcessor::createEditor()
{
    return new ChowAudioProcessorEditor (*this);
}

//==============================================================================
void ChowAudioProcessor::getStateInformation (MemoryBlock& /*destData*/)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ChowAudioProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChowAudioProcessor();
}
