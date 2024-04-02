#pragma once

#include "PluginProcessor.h"
#include "BinaryData.h"

#define DEG2RADS 0.0174533f

//==============================================================================
/**
*/
class PunkEQEditor  : public juce::AudioProcessorEditor
{
public:
    PunkEQEditor (PunkEQProcessor&);
    ~PunkEQEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //=================== PARAMETER MANIPULATION ===================================
    void setSliderComponent(juce::Slider& slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment, juce::String paramName, bool style);
    void setToggleComponent(juce::ToggleButton& button, std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>& buttonAttachment, juce::String paramName);
    juce::AffineTransform knobRotation(float radians, float posX, float posY, float scaleFactor);
    
private:
    // Parameters
    juce::Slider lowFKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowFKnobAttachment;
    
    juce::Slider lowGKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowGKnobAttachment;
    
    juce::Slider lowMidFKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowMidFKnobAttachment;
    
    juce::Slider lowMidGKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowMidGKnobAttachment;
    
    juce::Slider midFKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midFKnobAttachment;
    
    juce::Slider midGKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midGKnobAttachment;
    
    juce::Slider hiMidFKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hiMidFKnobAttachment;
    
    juce::Slider hiMidGKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hiMidGKnobAttachment;
    
    juce::Slider hiFKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hiFKnobAttachment;
    
    juce::Slider hiGKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hiGKnobAttachment;
    
    juce::ToggleButton onToggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> onToggleAttachment;
    
    // Assets
    juce::Image background;
    juce::Image lightOff;
    juce::Image knobImage;
    juce::Image switchTopImage;
        
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PunkEQProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PunkEQEditor)
};
