/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p),
      buttonAB ("A-B"),
      buttonCopyAB ("Copy"),
      backgroundImage {ImageCache::getFromMemory (BinaryData::BalanceGain_02fs8_png,
                                                  BinaryData::BalanceGain_02fs8_pngSize)},
      knobStyleImage  {ImageCache::getFromMemory (BinaryData::knob05LargeForeground4fs8_png,
                                                  BinaryData::knob05LargeForeground4fs8_pngSize)},
      filmstripImage  {ImageCache::getFromMemory (BinaryData::markerFilmstripfs8_png,
                                                  BinaryData::markerFilmstripfs8_pngSize)},
      processor (p)
{
    // Button AB
    buttonAB.setColour (TextButton::textColourOffId, Colour (0xff373737));
    buttonAB.setColour (TextButton::buttonColourId, Colour (0xff808080));
    addAndMakeVisible (buttonAB);
    buttonAB.addListener (this);

    // Button copy
    buttonCopyAB.setColour (TextButton::textColourOffId, Colour (0xff373737));
    buttonCopyAB.setColour (TextButton::buttonColourId, Colour (0xff808080));
    addAndMakeVisible (buttonCopyAB);
    buttonCopyAB.addListener (this);

    // StepSize slider
    String stepSizeParamName = processor.getParam(0).name;                     // magic constant!

    Slider* stepSizeSlider = new Slider (stepSizeParamName);
    jassert (stepSizeSlider);
    sliders.add (stepSizeSlider);

    stepSizeSlider->setSliderStyle (Slider::LinearHorizontal);
    stepSizeSlider->setTextBoxStyle (Slider::TextBoxLeft,false,60,15);         // magic constants!
    stepSizeSlider->setColour (Slider::textBoxTextColourId,       Colour (0xff373737));
    stepSizeSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0xff707070));
    stepSizeSlider->setColour (Slider::textBoxHighlightColourId,  Colour (0xffffffff));
    stepSizeSlider->setColour (Slider::textBoxOutlineColourId,    Colour (0x00000000));

    addAndMakeVisible (stepSizeSlider);
    stepSizeSlider->addListener (this);

    // StepSize label
    Label* stepSizeLabel = new Label (stepSizeParamName, stepSizeParamName);
    jassert (stepSizeLabel);
    labels.add (stepSizeLabel);

    stepSizeLabel->setJustificationType (Justification::centredLeft);
    stepSizeLabel->setEditable (false, false, false);
    stepSizeLabel->setColour (Label::textColourId, Colour (0xff2b2b2b));

    addAndMakeVisible (stepSizeLabel);

    // Main knob
    knob = new FilmStrip {filmstripImage, knobStyleImage};

    sliders.add (knob);

    knob->setDoubleClickReturnValue (true, sliders[0]->getValue()); // current getValue() is
                                                                    // the default value (0dB)

    addAndMakeVisible (knob);
    knob->addListener (this);

    // Init and size setting
    updateSlidersFromProcParams();  // set slider values and ranges

    setSize (400, 400);             // must be set before xtor finished

    startTimerHz (30);
}

PluginEditor::~PluginEditor()
{
}

//==============================================================================
void PluginEditor::paint (Graphics& g)
{
    g.fillAll (Colours::grey);
    g.drawImage (backgroundImage, 0, 0, 400, 400, 0, 0, 800, 800);
}

void PluginEditor::resized()
{
    buttonAB    .setBounds (  3,  5,  52, 20);
    buttonCopyAB.setBounds ( 56,  5,  52, 20);
    labels[0]  ->setBounds (278,  8,  60, 15);                                          // magic constants [0]
    sliders[0] ->setBounds (336,  6, 240, 20);
    knob       ->setBounds (83, 83, knob->getSize() / 2, knob->getSize() / 2); // halved for retina
}

//==============================================================================
void PluginEditor::buttonClicked (Button* clickedButton)
{
    if (clickedButton == &buttonAB) processor.toggleABState();
    if (clickedButton == &buttonCopyAB) processor.copyABState();
}

//==============================================================================
void PluginEditor::sliderValueChanged (Slider* movedSlider)
{
    jassert (movedSlider);
    setProcParamFromSlider (*movedSlider);
}

//==============================================================================
void PluginEditor::timerCallback()
{
    processor.updateStepSlaveRanges();
    updateSlidersFromProcParams();
}

//==============================================================================
void PluginEditor::setProcParamFromSlider (const Slider& slider) const
{
    int i = sliders.indexOf (&slider);
    const float sliderVal = (float) slider.getValue();

    processor.setParam (i, sliderVal);
}

//==============================================================================
void PluginEditor::updateSlidersFromProcParams()
{
    for (int i = 0; i < processor.numParams(); ++i) {
        const auto& p = processor.getParam(i);

        jassert (i < sliders.size());
        jassert (sliders[i]);
        sliders[i]->setRange (p.range.start,
                              p.range.end,
                              p.range.interval);
        sliders[i]->setValue ((double) p.get(), dontSendNotification);
    }
}


//==============================================================================
//==============================================================================
void NonMember::printSlidersParams(const PluginProcessor& processor,
                        const OwnedArray<Slider>& sliders)
{
    String message;

    for (int i = 0; i < processor.numParams(); ++i) {
        jassert (i < sliders.size());
        jassert (sliders[i]);
        message << "Sliders["<<i<<"] " << sliders[i]->getValue()
                << " params["<<i<<"] " << processor.getParam(i).get() << "\n";
    }

    Logger::outputDebugString (message);
}
