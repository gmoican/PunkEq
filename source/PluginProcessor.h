#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#if (MSVC)
#include "ipps.h"
#endif

#define DEFAULT_LO 80.0f
#define DEFAULT_LOMID 250.0f
#define DEFAULT_MID 780.0f
#define DEFAULT_HIMID 2400.0f
#define DEFAULT_HI 7000.0f
#define DEFAULT_GAIN 0.0f

//==============================================================================
/**
*/

class PunkEQProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    PunkEQProcessor();
    ~PunkEQProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //=============== MY STUFF =====================================================
    juce::AudioProcessorValueTreeState state;
    
    // Updaters
    void updateOnOff();
    void updateEq();
    void updateState();
    
    void process(float* samples, int numSamples);

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    using FilterBand = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    
    juce::dsp::ProcessorChain<FilterBand, FilterBand, FilterBand, FilterBand, FilterBand> equalizer;
    
    bool on;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PunkEQProcessor)
};
