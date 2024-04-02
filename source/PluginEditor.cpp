#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PunkEQEditor::PunkEQEditor (PunkEQProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    juce::ignoreUnused(audioProcessor);
    
    // ================= PARAMETERS ====================
    setSliderComponent(lowFKnob, lowFKnobAttachment, "LO_F", false);
    setSliderComponent(lowMidFKnob, lowMidFKnobAttachment, "LOMID_F", false);
    setSliderComponent(midFKnob, midFKnobAttachment, "MID_F", false);
    setSliderComponent(hiMidFKnob, hiMidFKnobAttachment, "HIMID_F", false);
    setSliderComponent(hiFKnob, hiFKnobAttachment, "HI_F", false);
    
    setSliderComponent(lowGKnob, lowGKnobAttachment, "LO_G", true);
    setSliderComponent(lowMidGKnob, lowMidGKnobAttachment, "LOMID_G", true);
    setSliderComponent(midGKnob, midGKnobAttachment, "MID_G", true);
    setSliderComponent(hiMidGKnob, hiMidGKnobAttachment, "HIMID_G", true);
    setSliderComponent(hiGKnob, hiGKnobAttachment, "HI_G", true);

    setToggleComponent(onToggle, onToggleAttachment, "ONOFF");

    // ================= ASSETS =======================
    background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    lightOff = juce::ImageCache::getFromMemory(BinaryData::lightOff_png, BinaryData::lightOff_pngSize);
    knobImage = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
    switchTopImage = juce::ImageCache::getFromMemory(BinaryData::switchTop_png, BinaryData::switchTop_pngSize);
        
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (220, 320);
}

PunkEQEditor::~PunkEQEditor()
{
}

//==============================================================================
void PunkEQEditor::paint (juce::Graphics& g)
{
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
        
    // =========== On/Off state ====================
    if (!onToggle.getToggleState()) {
        juce::AffineTransform t;
        t = t.scaled(0.49f);
        t = t.translated(28.5, 252.5);
        g.drawImageTransformed(lightOff, t);
    }
    
    // ========== Frequency knobs angle in radians ==================
    auto lowFRadians = juce::jmap(lowFKnob.getValue(), 30.0, 100.0, -150.0, 150.0) * DEG2RADS;
    auto lowMidFRadians = juce::jmap(lowMidFKnob.getValue(), 85.0, 360.0, -150.0, 150.0) * DEG2RADS;
    auto midFRadians = juce::jmap(midFKnob.getValue(), 200.0, 1300.0, -150.0, 150.0) * DEG2RADS;
    auto hiMidFRadians = juce::jmap(hiMidFKnob.getValue(), 620.0, 4000.0, -150.0, 150.0) * DEG2RADS;
    auto hiFRadians = juce::jmap(hiFKnob.getValue(), 1500.0, 14500.0, -150.0, 150.0) * DEG2RADS;
    // ============== Draw frequency knobs ==========================
    g.drawImageTransformed(knobImage, knobRotation(lowFRadians, 15.0, 25.0, 0.3261f));
    g.drawImageTransformed(knobImage, knobRotation(lowMidFRadians, 55.0, 25.0, 0.3261f));
    g.drawImageTransformed(knobImage, knobRotation(midFRadians, 95.0, 25.0, 0.3261f));
    g.drawImageTransformed(knobImage, knobRotation(hiMidFRadians, 135.0, 25.0, 0.3261f));
    g.drawImageTransformed(knobImage, knobRotation(hiFRadians, 175.0, 25.0, 0.3261f));
    
    // ========== Gain sliders y position ==================
    auto lowGYPos = juce::jmap(lowGKnob.getValue(), 10.0, -10.0, 98.0, 168.0);
    auto lowMidGYPos = juce::jmap(lowMidGKnob.getValue(), 10.0, -10.0, 98.0, 168.0);
    auto midGYPos = juce::jmap(midGKnob.getValue(), 10.0, -10.0, 98.0, 168.0);
    auto hiMidGYPos = juce::jmap(hiMidGKnob.getValue(), 10.0, -10.0, 98.0, 168.0);
    auto hiGYPos = juce::jmap(hiGKnob.getValue(), 10.0, -10.0, 98.0, 168.0);
    // ============= Draw gain sliders =====================
    g.drawImageTransformed(switchTopImage, knobRotation(0, 43, lowGYPos, 0.5));
    g.drawImageTransformed(switchTopImage, knobRotation(0, 72, lowMidGYPos, 0.5));
    g.drawImageTransformed(switchTopImage, knobRotation(0, 101, midGYPos, 0.5));
    g.drawImageTransformed(switchTopImage, knobRotation(0, 130, hiMidGYPos, 0.5));
    g.drawImageTransformed(switchTopImage, knobRotation(0, 159, hiGYPos, 0.5));
}

void PunkEQEditor::resized()
{
    // Frequency knobs
    lowFKnob.setBounds(15, 25, 32, 32);
    lowMidFKnob.setBounds(55, 25, 32, 32);
    midFKnob.setBounds(95, 25, 32, 32);
    hiMidFKnob.setBounds(135, 25, 32, 32);
    hiFKnob.setBounds(175, 25, 32, 32);
    
    // Gain sliders
    lowGKnob.setBounds(47, 100, 12, 82);
    lowMidGKnob.setBounds(74, 100, 12, 82);
    midGKnob.setBounds(105, 100, 12, 82);
    hiMidGKnob.setBounds(134, 100, 12, 82);
    hiGKnob.setBounds(163, 100, 12, 82);
        
    // OnOff
    onToggle.setBounds(65, 240, 50, 50);
}

void PunkEQEditor::setSliderComponent(juce::Slider &slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> &sliderAttachment, juce::String paramName, bool style){
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.state, paramName, slider);
    if (style)
        slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    else
        slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    addAndMakeVisible(slider);
    // slider.setAlpha(0);
}

void PunkEQEditor::setToggleComponent(juce::ToggleButton& button, std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>& buttonAttachment, juce::String paramName){
    buttonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.state, paramName, button);
    addAndMakeVisible(button);
    button.setAlpha(0);
}

juce::AffineTransform PunkEQEditor::knobRotation(float radians, float posX, float posY, float scaleFactor){
    juce::AffineTransform t;
    t = t.rotated(radians, 46.0f, 46.0f);
    t = t.scaled(scaleFactor);
    t = t.translated(posX, posY);
    return t;
}
