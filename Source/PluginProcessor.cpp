/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
PluginProcessor::PluginProcessor()
    : stepMaster {nullptr},
      xmlABState {"MYABSETTINGS"}
{
                    // Add parameters here and in header enum
                    // (No spaces in name for XML)    name       min    max   default  stepSize  stepMaster/Slave
    params.push_back (new AudioParameterFloatStepped ("StepSize",0.01f, 3.0f, 0.1875f,   0.0f, AudioParameterFloatStepped::stepMaster));
    params.push_back (new AudioParameterFloatStepped ("Gain",   -12.0f,12.0f,    0.0f,   0.0f, AudioParameterFloatStepped::stepSlave));

    // Only one (or no) step master controls
    int numStepMasters = 0;
    for (const auto p : params)
        if (p->stepSetting() == AudioParameterFloatStepped::stepMaster)
            ++numStepMasters;
    jassert (numStepMasters <= 1);

    // Keep pointer to step master if exists (remain null if none)  // hold view container of step slaves?
    for (const auto p : params)
        if (p->stepSetting() == AudioParameterFloatStepped::stepMaster)
            stepMaster = p;

    for (auto p : params) addParameter(p);                          // const auto& pointer or just auto?

    copyABState();
}

PluginProcessor::~PluginProcessor()
{
    // parameter deletes handled by processor OwnedArray
}

//==============================================================================
bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

//==============================================================================
void PluginProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/)
{
}

void PluginProcessor::releaseResources()
{
}

template <typename FloatType> // float or double precision
void PluginProcessor::process (AudioBuffer<FloatType>& buffer, MidiBuffer& /*midiMessages*/)
{
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());    // clear junk channels

    // per-block basis gain (fine for stepped control)
    const FloatType gndB = getParam(gain).get();
    const FloatType gnLin = Decibels::decibelsToGain (gndB);
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        buffer.applyGain (channel, 0, buffer.getNumSamples(), gnLin);
}

//==============================================================================
AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
// Copy plugin state to memory
void PluginProcessor::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml ("MYPLUGINSETTINGS");

    writeParamsToXml (xml);
    copyXmlToBinary (xml, destData);
}

// Get plugin state from memory
void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState)
        if (xmlState->hasTagName ("MYPLUGINSETTINGS")) setParamsFromXml (*xmlState);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

//==============================================================================
const AudioParameterFloatStepped& PluginProcessor::getParam (int index) const
{
    jassert (NonMember::indexInVector (index, params));
    jassert (params[index]);

    return *params[index];
}

//==============================================================================
void PluginProcessor::setParam (int index, float newValue)
{
    jassert (NonMember::indexInVector (index, params));
    jassert (NonMember::valueInParamRange (newValue, getParam(index)));
    const float sliderVal0to1 = getParam(index).range.convertTo0to1 (newValue);

    jassert (params[index]);
    params[index]->setValueNotifyingHost (sliderVal0to1); // setVal needs 0to1
}

//==============================================================================
void PluginProcessor::updateStepSlaveRanges()
{
    if (stepMaster) {
        const float newStepSize = stepMaster->get();

        // Calculate min, max from num steps per param
        const float newMax = stepsPerSidePerParam * newStepSize;
        const float newMin = -(newMax);

        // Set ranges of step slaves            // hold view of step slaves more efficient?
        for (int i = 0; i < numParams(); ++i)
            if (getParam(i).stepSetting() == AudioParameterFloatStepped::stepSlave)
                setParamRange (i, newMin, newMax, newStepSize);
    }
}

//==============================================================================
void PluginProcessor::setParamRange (int index, float min, float max, float step)
{
    jassert (params[index]);
    params[index]->setRange (min, max, step);
}

//==============================================================================
void PluginProcessor::copyABState()
{
    writeParamsToXml (xmlABState);
}

void PluginProcessor::toggleABState()
{
    XmlElement tempState = XmlElement ("TEMP");

    // curr -> temp
    writeParamsToXml (tempState);
    
    // abState -> curr
    setParamsFromXml (xmlABState);

    // temp -> abState
    xmlABState = tempState;
}

//==============================================================================
void PluginProcessor::writeParamsToXml (XmlElement& xml)
{
    for (int i = 0; i < numParams(); ++i) {
        const float min   = getParam(i).range.start;
        const float max   = getParam(i).range.end;
        const float step  = getParam(i).range.interval;
        const float value = getParam(i).get();
        xml.setAttribute (getParam(i).paramID+"min",  min);
        xml.setAttribute (getParam(i).paramID+"max",  max);
        xml.setAttribute (getParam(i).paramID+"step", step);
        xml.setAttribute (getParam(i).paramID,        value);
    }
}

//==============================================================================
void PluginProcessor::setParamsFromXml (const XmlElement& xml)
{
    for (int i = 0; i < numParams(); ++i) {
        const float min   = float (xml.getDoubleAttribute (getParam(i).paramID+"min"));
        const float max   = float (xml.getDoubleAttribute (getParam(i).paramID+"max"));
        const float step  = float (xml.getDoubleAttribute (getParam(i).paramID+"step"));
        const float value = float (xml.getDoubleAttribute (getParam(i).paramID));
        setParamRange (i, min, max, step);
        setParam (i, value);
    }
}


//==============================================================================
//==============================================================================
template <typename Element>
bool NonMember::indexInVector (int index, const std::vector<Element>& container)
{
    return (0 <= index && index < (int) container.size());
}

//==============================================================================
bool NonMember::valueInParamRange (float testValue, const AudioParameterFloatStepped& param)
{
    const float min = param.range.start;
    const float max = param.range.end;

    return min <= testValue && testValue <= max;
}

//==============================================================================
void NonMember::printParams(const PluginProcessor& processor)
{
    String message;

    for (int i = 0; i < processor.numParams(); ++i) {
        message << "params_["<<i<<"]="
                << processor.getParam(i).get() << " "
                << processor.getParam(i).range.start << " "
                << processor.getParam(i).range.end << " "
                << processor.getParam(i).range.interval << "\n";
    }

    Logger::outputDebugString (message);
}
