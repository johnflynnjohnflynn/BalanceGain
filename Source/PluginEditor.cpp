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
      metalBackground {ImageCache::getFromMemory (BinaryData::brushmetalbigexporttinypng_png,
                                                  BinaryData::brushmetalbigexporttinypng_pngSize)},
      knobStyleImage  {ImageCache::getFromMemory (BinaryData::knob05LargeForeground4fs8_png,
                                                  BinaryData::knob05LargeForeground4fs8_pngSize)},
      filmstripImage  {ImageCache::getFromMemory (BinaryData::knob05LargeMarkerTextureFilmstripfs8_png,
                                                  BinaryData::knob05LargeMarkerTextureFilmstripfs8_pngSize)},
      knob {filmstripImage, knobStyleImage},
      processor (p)
{
    buttonAB.setColour (TextButton::textColourOffId, Colour (0xff404040));
    buttonAB.setColour (TextButton::buttonColourId, Colour (0xffa0a0a0));
    addAndMakeVisible (buttonAB);
    buttonAB.addListener (this);

    buttonCopyAB.setColour (TextButton::textColourOffId, Colour (0xff404040));
    buttonCopyAB.setColour (TextButton::buttonColourId, Colour (0xffa0a0a0));
    addAndMakeVisible (buttonCopyAB);
    buttonCopyAB.addListener (this);

    // const AudioParameterFloatStepped& param = processor.getParam(0);            // magic! 0
    Slider* stepSizeSlider = new Slider (processor.getParam(0).name);           // magic! 0
    jassert (stepSizeSlider);
    sliders.add (stepSizeSlider);

    addAndMakeVisible (stepSizeSlider);
    stepSizeSlider->addListener (this);

    Slider* gainSlider = new Slider (processor.getParam(1).name);           // magic! 1
    jassert (gainSlider);
    sliders.add (gainSlider);

    addAndMakeVisible (gainSlider);
    gainSlider->addListener (this);


/*
    // Add GUI slider/label for every AudioProcessorParameter
    for (int i = 0; i < processor.numParams(); ++i)
    {
        const AudioParameterFloatStepped& param = processor.getParam(i);

        Slider* newSlider = new Slider (param.name);
        jassert (newSlider);
        sliders.add (newSlider);

        newSlider->setSliderStyle (Slider::LinearHorizontal);
        newSlider->setTextBoxStyle (Slider::TextBoxRight,false,unit*8,unit*2);
        newSlider->setColour (Slider::thumbColourId, Colours::silver);
        newSlider->setColour (Slider::textBoxTextColourId, Colour (0xff404040));
        newSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0xff909090));
        
        addAndMakeVisible (newSlider);

        newSlider->addListener (this);

        const String name = param.name;
        Label* aLabel = new Label (name, name);
        jassert (aLabel);
        labels.add (aLabel);

        aLabel->setJustificationType (Justification::centredLeft);
        aLabel->setEditable (false, false, false);
        aLabel->setColour (Label::textColourId, Colour (0xff404040));
        aLabel->setColour (TextEditor::textColourId, Colours::black);
        aLabel->setColour (TextEditor::backgroundColourId,Colour (0x00000000));

        addAndMakeVisible (aLabel);
    }
    jassert (sliders.size() == labels.size());
*/

    addAndMakeVisible (knob);

    updateSlidersFromProcParams();  // set slider values and ranges

    /*const int numRows = sliders.size();
    const int height = margin
                     + heightButtonsAB
                     + numRows * 2 * heightComponent
                     + margin / 2;*/
    setSize (600, 400);        // must be set before xtor finished                  // magic

    startTimerHz (30);
}

PluginEditor::~PluginEditor()
{
}

//==============================================================================
void PluginEditor::paint (Graphics& g)
{
    g.drawImage(metalBackground, 0, 0, 640, 400, 0, 0, 1280, 800);                      // magic
}

void PluginEditor::resized()
{
    buttonAB    .setBounds (  4,  4, 60, 20);                                           // magic!
    buttonCopyAB.setBounds ( 64,  4, 60, 20);
    sliders[0] ->setBounds (128,  4, 60, 20);
    sliders[1] ->setBounds (128, 40, 60, 20);

    knob.setBounds (0, 150, knob.getSize() / 2, knob.getSize() / 2); // halved for retina

    /*for (int i = 0; i < sliders.size(); ++i)
    {
        jassert (sliders[i]);
        sliders[i]->setBounds(margin,
                              margin + heightButtonsAB + i * 2 * heightComponent + 16,
                              widthComponent,
                              heightComponent);
    }
    for (int i = 0; i < labels.size(); ++i)
    {
        jassert (labels[i]);
        labels[i]->setBounds (margin,
                              margin + heightButtonsAB + i * 2 * heightComponent,
                              widthComponent,
                              heightComponent);
    }*/
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
