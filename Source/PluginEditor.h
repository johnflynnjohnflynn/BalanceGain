/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include <sstream>                              // for printing debug messages
#include "../JuceLibraryCode/JuceHeader.h"
#include "JFProcessor.h"


//==============================================================================
/**
    Simple generic editor
    ---------------------
    
    - Generates controls automatically from parameters in plugin processor
    - A/B comparisons
*/
class JFEditor  : public AudioProcessorEditor,
                  public Button::Listener,
                  public Slider::Listener,
                  private Timer
{
public:
    JFEditor (JFProcessor&);
    ~JFEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void buttonClicked (Button* clickedButton) override;
    void sliderValueChanged (Slider* changedSlider) override;

private:
    TextButton buttonAB;
    TextButton buttonCopyAB;
    OwnedArray<Slider> sliders;
    OwnedArray<Label> labels;
    
    void timerCallback() override;

    void setProcParamFromSlider (const Slider& slider) const;
    void updateSlidersFromProcParams();
    
    enum relativeLayout {                   // Measurements for relative layout
        unit = 8,
        width = 64 * unit,
        margin = 2 * unit,
        widthComponent = width - margin * 2,
        heightComponent = 3 * unit,
        heightButtonsAB = 5 * unit - 2,
        buttonABWidth = int (widthComponent * 0.75 - unit),
        buttonCopyABWidth = int (widthComponent * 0.25 - unit),
    };

    JFProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JFEditor)
};


//==============================================================================
// Non-member helpers
//==============================================================================
void printSlidersParams(const JFProcessor& processor,
                        const OwnedArray<Slider>& sliders);


#endif  // PLUGINEDITOR_H_INCLUDED
