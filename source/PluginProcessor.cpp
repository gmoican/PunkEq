#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PunkEQProcessor::PunkEQProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), state(*this, nullptr, "parameters", createParams())
#endif
{
}

PunkEQProcessor::~PunkEQProcessor()
{
}

//==============================================================================
const juce::String PunkEQProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PunkEQProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PunkEQProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PunkEQProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PunkEQProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PunkEQProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PunkEQProcessor::getCurrentProgram()
{
    return 0;
}

void PunkEQProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String PunkEQProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void PunkEQProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

// =========== PARAMETER LAYOUT ====================
juce::AudioProcessorValueTreeState::ParameterLayout PunkEQProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
        
    params.push_back(std::make_unique<juce::AudioParameterBool>("ONOFF", "On/Off", true));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LO_F", "Low frequency", juce::NormalisableRange<float>(30.0f, 100.0f, 0.1f, 0.667f), DEFAULT_LO, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LO_G", "Low gain", juce::NormalisableRange<float>(-10.0f, 10.0f, 0.1f), DEFAULT_GAIN, "dB"));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOMID_F", "Low mids frequency", juce::NormalisableRange<float>(85.0f, 360.0f, 0.1f, 0.62f), DEFAULT_LOMID, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOMID_G", "Low mids gain", juce::NormalisableRange<float>(-10.0f, 10.0f, 0.1f), DEFAULT_GAIN, "dB"));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MID_F", "Mids frequency", juce::NormalisableRange<float>(200.0f, 1300.0f, 0.1f, 0.547f), DEFAULT_MID, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MID_G", "Mids gain", juce::NormalisableRange<float>(-10.0f, 10.0f, 0.1f), DEFAULT_GAIN, "dB"));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIMID_F", "High mids frequency", juce::NormalisableRange<float>(620.0f, 4000.0f, 0.1f, 0.548f), DEFAULT_HIMID, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIMID_G", "High mids gain", juce::NormalisableRange<float>(-10.0f, 10.0f, 0.1f), DEFAULT_GAIN, "dB"));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HI_F", "Highs frequency", juce::NormalisableRange<float>(1500.0f, 14500.0f, 0.1f, 0.49f), DEFAULT_HI, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HI_G", "Highs gain", juce::NormalisableRange<float>(-10.0f, 10.0f, 0.1f), DEFAULT_GAIN, "dB"));
        
    return { params.begin(), params.end() };
}

// ============ VALUE UPDATERS =====================
void PunkEQProcessor::updateOnOff()
{
    auto ONOFF = state.getRawParameterValue("ONOFF");
    on = ONOFF->load();
}

void PunkEQProcessor::updateEq()
{
    auto LO_F = state.getRawParameterValue("LO_F")->load();
    auto LO_G = state.getRawParameterValue("LO_G")->load();
    auto LO_Q = juce::jmap(abs(LO_G), 0.f, 10.f, 1.f, 1.25f);
    
    auto LOMID_F = state.getRawParameterValue("LOMID_F")->load();
    auto LOMID_G = state.getRawParameterValue("LOMID_G")->load();
    auto LOMID_Q = juce::jmap(abs(LOMID_G), 0.f, 10.f, 1.f, 1.2f);
    
    auto MID_F = state.getRawParameterValue("MID_F")->load();
    auto MID_G = state.getRawParameterValue("MID_G")->load();
    auto MID_Q = juce::jmap(abs(MID_G), 0.f, 10.f, 0.707f, 1.f);
    
    auto HIMID_F = state.getRawParameterValue("HIMID_F")->load();
    auto HIMID_G = state.getRawParameterValue("HIMID_G")->load();
    auto HIMID_Q = juce::jmap(abs(HIMID_G), 0.f, 10.f, 0.707f, 1.f);
    
    auto HI_F = state.getRawParameterValue("HI_F")->load();
    auto HI_G = state.getRawParameterValue("HI_G")->load();
    auto HI_Q = juce::jmap(abs(HI_G), 0.f, 10.f, 0.707f, 1.f);
    
    LO_G = juce::Decibels::decibelsToGain(LO_G);
    LOMID_G = juce::Decibels::decibelsToGain(LOMID_G);
    MID_G = juce::Decibels::decibelsToGain(MID_G);
    HIMID_G = juce::Decibels::decibelsToGain(HIMID_G);
    HI_G = juce::Decibels::decibelsToGain(HI_G);
    
    double sampleRate = getSampleRate();
    
    *equalizer.get<0>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, LO_F, LO_Q, LO_G);
    *equalizer.get<1>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, LOMID_F, LOMID_Q, LOMID_G);
    *equalizer.get<2>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, MID_F, MID_Q, MID_G);
    *equalizer.get<3>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, HIMID_F, HIMID_Q, HIMID_G);
    *equalizer.get<4>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, HI_F, HI_Q, HI_G);
}

void PunkEQProcessor::updateState()
{
    updateOnOff();
    updateEq();
}

//==============================================================================
void PunkEQProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;
    
    equalizer.prepare(spec);
    equalizer.reset();
}

void PunkEQProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PunkEQProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PunkEQProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    updateState();
    if(on)
    {
        juce::dsp::AudioBlock<float> audioBlock = juce::dsp::AudioBlock<float>(buffer);
        
        equalizer.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    }
}

//==============================================================================
bool PunkEQProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PunkEQProcessor::createEditor()
{
    return new PunkEQEditor (*this);
}

//==============================================================================
void PunkEQProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    juce::ignoreUnused(destData);
}

void PunkEQProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PunkEQProcessor();
}
