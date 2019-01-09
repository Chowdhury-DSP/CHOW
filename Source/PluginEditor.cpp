/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
class MyLNF : public LookAndFeel_V4
{
public:
    MyLNF()
    {
        setColour (ComboBox::outlineColourId, Colours::darkorange);
    }

    Font getTextButtonFont (TextButton& button, int buttonHeight) override
    {
        return LookAndFeel_V4::getTextButtonFont (button, buttonHeight).boldened();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyLNF)
};

//==============================================================================
enum
{
    width = 400,
    height = 300,
    titleHeight = 30,

    sliderWidth = 100,
    sliderY = 50,
    labelHeight = 20,
    labelY = 35,

    flipX = width - 60,
    flipY = 7,
    flipWidth = 50,
    flipHeight = 25,

    visualizerY = 160,
};


ChowAudioProcessorEditor::ChowAudioProcessorEditor (ChowAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), visualizer (*p.vis)
{
    myLNF = new MyLNF;

    initSliders();
    initLabels();    
    initVisualizer();   
    initFlipButton();

    setSize (width, height);
}

ChowAudioProcessorEditor::~ChowAudioProcessorEditor()
{
}

void ChowAudioProcessorEditor::initSliders()
{
    auto setupSlider = [this] (Slider& slide, AudioParameterFloat* param,
                               float skewMidPoint = NAN, String textSuffix = String (" dB"), float step = 0.1f)
    {
        //@TODO: Custom slider LNF??
        slide.setName (param->name);
        slide.setRange (param->range.start, param->range.end, step);
        if (slide.getRange().contains (skewMidPoint))
            slide.setSkewFactorFromMidPoint ((double) skewMidPoint);

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

    setupSlider (threshSlide, processor.threshDB, -30.0f);
    setupSlider (ratioSlide, processor.ratio, 15.0f, String (" : 1"));
    setupSlider (inGainSlide, processor.inGainDB);
    setupSlider (outGainSlide, processor.outGainDB);
}

void ChowAudioProcessorEditor::initLabels()
{
    auto setupLabel = [this] (Label& label, AudioParameterFloat* param)
    {
        label.setText (param->name, dontSendNotification);
        label.setJustificationType (Justification::centred);
        label.setColour (Label::textColourId, Colours::red);
        label.setFont (17.0f);
        addAndMakeVisible (label);
    };

    setupLabel (threshLabel, processor.threshDB);
    setupLabel (ratioLabel, processor.ratio);
    setupLabel (inGainLabel, processor.inGainDB);
    setupLabel (outGainLabel, processor.outGainDB);
}

void ChowAudioProcessorEditor::initVisualizer()
{
    //@TODO: fine tune visualizer (override paint function?)
    visualizer.setRepaintRate (1000);
    visualizer.setBufferSize (128);
    visualizer.setSamplesPerBlock (processor.getBlockSize());
    visualizer.setColours (Colours::black, Colours::darkorange);
    addAndMakeVisible (visualizer);
}

void ChowAudioProcessorEditor::initFlipButton()
{
    flipButton.setName (processor.flip->name);
    flipButton.setButtonText (flipButton.getName());

    flipButton.setLookAndFeel (myLNF);
    flipButton.setColour (TextButton::buttonOnColourId, Colours::darkorange);
    flipButton.setColour (TextButton::textColourOnId, Colours::black);
    flipButton.setColour (TextButton::buttonColourId, Colours::black);
    flipButton.setColour (TextButton::textColourOffId, Colours::darkorange);

    flipButton.setToggleState (processor.flip->get(), dontSendNotification);
    flipButton.setClickingTogglesState (true);
    flipButton.onStateChange = [this] { *processor.flip = flipButton.getToggleState(); };

    addAndMakeVisible (flipButton);
}

//==============================================================================
void ChowAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setColour (Colours::darkorange);
    g.setFont (Font ((float) titleHeight, Font::bold));
    g.drawFittedText ("CHOW", 0, 0, width, titleHeight, Justification::centred, 1);

    g.drawLine (0.0f, (float) visualizerY, (float) width, (float) visualizerY, 5.0f);
}

void ChowAudioProcessorEditor::resized()
{
    flipButton.setBounds (flipX, flipY, flipWidth, flipHeight);

    inGainLabel.setBounds (0, labelY, sliderWidth, labelHeight);
    threshLabel.setBounds (inGainLabel.getRight(), labelY, sliderWidth, labelHeight);
    ratioLabel.setBounds (threshLabel.getRight(), labelY, sliderWidth, labelHeight);
    outGainLabel.setBounds (ratioLabel.getRight(), labelY, sliderWidth, labelHeight);


    inGainSlide.setBounds (0, sliderY, sliderWidth, sliderWidth);
    threshSlide.setBounds (inGainSlide.getRight(), sliderY, sliderWidth, sliderWidth);
    ratioSlide.setBounds (threshSlide.getRight(), sliderY, sliderWidth, sliderWidth);
    outGainSlide.setBounds (ratioSlide.getRight(), sliderY, sliderWidth, sliderWidth);

    auto bounds = getLocalBounds();
    bounds.removeFromTop (visualizerY);
    visualizer.setBounds (bounds);
}

AudioParameterFloat* ChowAudioProcessorEditor::getParamForSlider (Slider* slider)
{
    if (processor.threshDB->name == slider->getName())
        return processor.threshDB;
    else if (processor.ratio->name == slider->getName())
        return processor.ratio;
    else if (processor.inGainDB->name == slider->getName())
        return processor.inGainDB;
    else if (processor.outGainDB->name == slider->getName())
        return processor.outGainDB;
    else
        return nullptr;
}

void ChowAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (AudioParameterFloat* param = getParamForSlider(slider)){
        *param = (float) slider->getValue();
    }
}

void ChowAudioProcessorEditor::sliderDragStarted(Slider* slider)
{
    if (AudioParameterFloat* param = getParamForSlider(slider))
        param->beginChangeGesture();
}

void ChowAudioProcessorEditor::sliderDragEnded(Slider* slider)
{
    if (AudioParameterFloat* param = getParamForSlider(slider))
        param->endChangeGesture();

}
