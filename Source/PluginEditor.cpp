/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
enum
{
    sliderWidth = 100,
    sliderY = 50,

    labelY = 35,
    labelHeight = 20,

    visualizerY = 160,
};


ChowAudioProcessorEditor::ChowAudioProcessorEditor (ChowAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), visualizer (*p.vis)
{
    auto setupSlider = [this] (Slider& slide, AudioParameterFloat* param, String textSuffix = String (" dB"), float step = 0.1f)
    {
        slide.setName (param->name);
        slide.setRange (param->range.start, param->range.end, step);
        
        slide.setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
        slide.setColour (Slider::rotarySliderFillColourId, Colours::darkorange);
        slide.setColour (Slider::textBoxOutlineColourId, Colours::transparentBlack);
        slide.setColour (Slider::thumbColourId, Colours::red);
        slide.setColour (Slider::textBoxTextColourId, Colours::darkorange);
        slide.setColour (Slider::textBoxHighlightColourId, Colours::grey);

        slide.setTextBoxStyle (Slider::TextBoxBelow, false, 80, 15);
        slide.setTextValueSuffix (textSuffix);

        slide.setValue (param->get());
        slide.addListener (this);
        addAndMakeVisible (slide);
    };

    setupSlider (threshSlide, processor.thresh);
    setupSlider (ratioSlide, processor.ratio, String (" : 1"));
    setupSlider (inGainSlide, processor.inGaindB);
    setupSlider (outGainSlide, processor.outGaindB);

    auto setupLabel = [this] (Label& label, AudioParameterFloat* param)
    {
        label.setText (param->name, dontSendNotification);
        label.setJustificationType (Justification::centred);
        label.setColour (Label::textColourId, Colours::red);
        label.setFont (17.0f);
        addAndMakeVisible (label);
    };

    setupLabel (threshLabel, processor.thresh);
    setupLabel (ratioLabel, processor.ratio);
    setupLabel (inGainLabel, processor.inGaindB);
    setupLabel (outGainLabel, processor.outGaindB);

    visualizer.setRepaintRate (1000);
    visualizer.setBufferSize (128);
    visualizer.setSamplesPerBlock (processor.getBlockSize());
    visualizer.setColours (Colours::black, Colours::darkorange);
    addAndMakeVisible (visualizer);

    setSize (400, 300);
}

ChowAudioProcessorEditor::~ChowAudioProcessorEditor()
{
}

//==============================================================================
void ChowAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::darkorange);
    g.setFont (Font (30.0f, Font::bold));
    g.drawFittedText ("CHOW", 0, 0, getWidth(), 30, Justification::centred, 1);

    g.drawLine (0, visualizerY, getWidth(), visualizerY, 5.0f);
}

void ChowAudioProcessorEditor::resized()
{
    inGainLabel.setBounds (0, labelY, sliderWidth, labelHeight);
    threshLabel.setBounds (100, labelY, sliderWidth, labelHeight);
    ratioLabel.setBounds (200, labelY, sliderWidth, labelHeight);
    outGainLabel.setBounds (300, labelY, sliderWidth, labelHeight);


    inGainSlide.setBounds (0, sliderY, sliderWidth, sliderWidth);
    threshSlide.setBounds (100, sliderY, sliderWidth, sliderWidth);
    ratioSlide.setBounds (200, sliderY, sliderWidth, sliderWidth);
    outGainSlide.setBounds (300, sliderY, sliderWidth, sliderWidth);

    auto bounds = getLocalBounds();
    bounds.removeFromTop (visualizerY);
    visualizer.setBounds (bounds);
}
