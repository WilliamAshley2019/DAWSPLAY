#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class DAWInfoDisplayEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    DAWInfoDisplayEditor(DAWInfoDisplayProcessor&);
    ~DAWInfoDisplayEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    DAWInfoDisplayProcessor& processor;

    juce::ComboBox displayModeCombo;
    juce::Slider fontSizeSlider;
    juce::Label fontSizeLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> displayModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fontSizeAttachment;

    juce::String getDisplayString();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DAWInfoDisplayEditor)
};