/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioParameterFloatStepped.h"


//==============================================================================
/**
    Simple stepped gain plugin
    --------------------------

    - Gain control stepped
    - Step size variable
    - Very precise step size values allowed (i.e. many decimal places)
    - Double precision enabled
    - A/B compare function

*/
class PluginProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    PluginProcessor();
    ~PluginProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool supportsDoublePrecisionProcessing() const { return true; }

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
    {
        jassert (! isUsingDoublePrecision());
        process (buffer, midiMessages);
    }
    void processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override
    {
        jassert (isUsingDoublePrecision());
        process (buffer, midiMessages);
    }

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                   { return true; }

    //==============================================================================
    const String getName() const override             { return JucePlugin_Name; }

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override      { return 0.0; }

    //==============================================================================
    int getNumPrograms() override                            { return 1; }
    int getCurrentProgram() override                         { return 0; }
    void setCurrentProgram (int /*index*/) override          {}
    const String getProgramName (int /*index*/) override     { return String(); }
    void changeProgramName (int /*index*/, const String& /*newName*/) override {}

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    int numParams() const { return (int) params.size(); }

    const AudioParameterFloatStepped& getParam (int index) const;
    void setParam (int index, float newValue);

    void setParamRange (int index, float min, float max, float step);
    void updateStepSlaveRanges();
    
    void copyABState();
    void toggleABState();

private:
    enum ParameterNames {                               // List symbolic names here
        stepSize,                                       // (for parameter indices
        gain,                                           // in process())
    };

    enum StepInfo {
        stepsPerParam = 25, // odd number
        stepsPerSidePerParam = (stepsPerParam - 1) / 2
    };

    std::vector<AudioParameterFloatStepped*> params;    // (Eliminate downcasting by
                                                        // holding view of
                                                        // AudioParameterFloatStepped
                                                        // params directly.
                                                        // Processor OwnedArray
                                                        // still owns.)

    AudioParameterFloatStepped* stepMaster;             // Keep pointer to stepMaster

    XmlElement xmlABState;                              // A/B state holder

    void writeParamsToXml (XmlElement& xml);
    void setParamsFromXml (const XmlElement& xml);
    
    template <typename FloatType>                       // Double precision enabled
    void process (AudioBuffer<FloatType>& buffer,       // processBlock() template
                  MidiBuffer& midiMessages);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


namespace NonMember // Some helper functions that don't need to be inside the processor
{

template <typename Element>
bool indexInVector (int index, const std::vector<Element>& container);

bool valueInParamRange (float testValue, const AudioParameterFloatStepped& param);
void printParams(const PluginProcessor& processor);

} // namespace NonMember

#endif  // PLUGINPROCESSOR_H_INCLUDED
