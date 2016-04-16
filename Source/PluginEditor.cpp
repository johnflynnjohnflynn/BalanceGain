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
      backgroundImage {ImageCache::getFromMemory (BinaryData::BalanceGain_01fs8_png,
                                                  BinaryData::BalanceGain_01fs8_pngSize)},
      knobStyleImage  {ImageCache::getFromMemory (BinaryData::knob05LargeForeground4fs8_png,
                                                  BinaryData::knob05LargeForeground4fs8_pngSize)},
      //knobTexture     {ImageCache::getFromMemory (BinaryData::knob05LargeTextureOnlyRotatedfs8_png,
        //                                          BinaryData::knob05LargeTextureOnlyRotatedfs8_pngSize)},
      filmstripImage  {ImageCache::getFromMemory (BinaryData::markerFilmstripfs8_png,
                                                  BinaryData::markerFilmstripfs8_pngSize)},
      processor (p)
{
    buttonAB.setColour (TextButton::textColourOffId, Colour (0xff404040));
    buttonAB.setColour (TextButton::buttonColourId, Colour (0xff808080));
    addAndMakeVisible (buttonAB);
    buttonAB.addListener (this);

    buttonCopyAB.setColour (TextButton::textColourOffId, Colour (0xff404040));
    buttonCopyAB.setColour (TextButton::buttonColourId, Colour (0xff808080));
    addAndMakeVisible (buttonCopyAB);
    buttonCopyAB.addListener (this);

    String stepSizeParamName = processor.getParam(0).name;                     // magic!

    Slider* stepSizeSlider = new Slider (stepSizeParamName);
    jassert (stepSizeSlider);
    sliders.add (stepSizeSlider);

    stepSizeSlider->setSliderStyle (Slider::LinearHorizontal);
    stepSizeSlider->setTextBoxStyle (Slider::TextBoxLeft,false,60,15);         // magic!
    stepSizeSlider->setColour (Slider::textBoxTextColourId,       Colour (0xff404040));
    stepSizeSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0xff707070));
    stepSizeSlider->setColour (Slider::textBoxHighlightColourId,  Colour (0xffffffff));
    stepSizeSlider->setColour (Slider::textBoxOutlineColourId,    Colour (0x00000000));
    /*stepSizeSlider->setColour (Slider::thumbColourId, Colours::silver);
    stepSizeSlider->setColour (Slider::textBoxTextColourId, Colour (0xff404040));
    stepSizeSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0xff909090));

        setTextBoxStyle (TextBoxBelow, false, 80, 15);
        //setColour (Slider::backgroundColourId,        Colour (0x00000000));

        setColour (Slider::textBoxTextColourId,       Colour (0xff606060));
        setColour (Slider::textBoxBackgroundColourId, Colour (0x80c0c0c0));
        setColour (Slider::textBoxHighlightColourId,  Colour (0xffffffff));
        setColour (Slider::textBoxOutlineColourId,    Colour (0x00000000));*/

    addAndMakeVisible (stepSizeSlider);
    stepSizeSlider->addListener (this);

    Label* stepSizeLabel = new Label (stepSizeParamName, stepSizeParamName);
    jassert (stepSizeLabel);
    labels.add (stepSizeLabel);

    stepSizeLabel->setJustificationType (Justification::centredLeft);
    stepSizeLabel->setEditable (false, false, false);
    stepSizeLabel->setColour (Label::textColourId, Colour (0xff404040));/*
    stepSizeLabel->setColour (TextEditor::textColourId, Colours::black);
    stepSizeLabel->setColour (TextEditor::backgroundColourId,Colour (0x00000000));*/

    addAndMakeVisible (stepSizeLabel);
/*
    Slider* gainSlider = new Slider (processor.getParam(1).name);               // magic! 1
    jassert (gainSlider);
    sliders.add (gainSlider);

    addAndMakeVisible (gainSlider);
    gainSlider->addListener (this);*/

    knob = new FilmStrip {filmstripImage, knobStyleImage};

    sliders.add (knob);

    addAndMakeVisible (knob);
    knob->addListener (this);

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

    updateSlidersFromProcParams();  // set slider values and ranges

    /*const int numRows = sliders.size();
    const int height = margin
                     + heightButtonsAB
                     + numRows * 2 * heightComponent
                     + margin / 2;*/
    setSize (400, 400);        // must be set before xtor finished                  // magic

    startTimerHz (30);
}

PluginEditor::~PluginEditor()
{
}

//==============================================================================
void PluginEditor::paint (Graphics& g)
{
    g.fillAll (Colours::grey);
    g.drawImage (backgroundImage, 0, 0, 400, 400, 0, 0, 800, 800);                      // magic
}

void PluginEditor::resized()
{
    buttonAB    .setBounds (  3,  5,  52, 20);                                           // magic!
    buttonCopyAB.setBounds ( 56,  5,  52, 20);
    labels[0]  ->setBounds (278,  8,  60, 15);
    sliders[0] ->setBounds (336,  6, 240, 20);
    //sliders[1] ->setBounds (128, 40, 240, 20);
    knob       ->setBounds (83, 83, knob->getSize() / 2, knob->getSize() / 2); // halved for retina

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
