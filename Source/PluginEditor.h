/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "FilmStrip.h"


//==============================================================================
/**
    Simple generic editor
    ---------------------
    
    - Generates controls automatically from parameters in plugin processor
    - A/B comparisons
*/
class PluginEditor  : public AudioProcessorEditor,
                      public Button::Listener,
                      public Slider::Listener,
                      private Timer
{
public:
    PluginEditor (PluginProcessor&);
    ~PluginEditor();

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

    Image bgTexture {ImageCache::getFromMemory (BinaryData::brushmetalbigexporttinypng_png,
                                                BinaryData::brushmetalbigexporttinypng_pngSize)};
    Image knob      {ImageCache::getFromMemory (BinaryData::knob05LargeForeground4fs8_png,
                                                BinaryData::knob05LargeForeground4fs8_pngSize)};
    Image fsImage   {ImageCache::getFromMemory (BinaryData::knob05LargeMarkerTextureFilmstripfs8_png,
                                                BinaryData::knob05LargeMarkerTextureFilmstripfs8_pngSize)};

    FilmStrip filmstripKnob {fsImage, knob};
    
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

    PluginProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};


namespace NonMember // Some helper functions that don't need to be inside the processor
{

void printSlidersParams(const PluginProcessor& processor,
                        const OwnedArray<Slider>& sliders);

}

#endif  // PLUGINEDITOR_H_INCLUDED
