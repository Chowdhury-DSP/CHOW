/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ChowAudioProcessorEditor  : public AudioProcessorEditor,
                                  public Slider::Listener
{
public:
    ChowAudioProcessorEditor (ChowAudioProcessor&);
    ~ChowAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void sliderValueChanged (Slider* slider) override {}

private:
    ChowAudioProcessor& processor;

    Slider threshSlide;
    Label threshLabel;

    Slider ratioSlide;
    Label ratioLabel;

    Slider inGainSlide;
    Label inGainLabel;

    Slider outGainSlide;
    Label outGainLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChowAudioProcessorEditor)
};
