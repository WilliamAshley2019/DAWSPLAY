#include "PluginProcessor.h"
#include "PluginEditor.h"

DAWInfoDisplayEditor::DAWInfoDisplayEditor(DAWInfoDisplayProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(500, 350);

    // Display mode combo box
    addAndMakeVisible(displayModeCombo);
    displayModeCombo.addItemList(juce::StringArray{
        "Tempo (BPM)",
        "Sample Position",
        "PPQ Position",
        "Time Signature",
        "Play State",
        "Recording State",
        "Loop State"
        }, 1);
    displayModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processor.parameters, "displayMode", displayModeCombo);

    // Font size slider
    addAndMakeVisible(fontSizeSlider);
    fontSizeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    fontSizeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    fontSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.parameters, "fontSize", fontSizeSlider);

    addAndMakeVisible(fontSizeLabel);
    fontSizeLabel.setText("Font Size:", juce::dontSendNotification);
    fontSizeLabel.attachToComponent(&fontSizeSlider, true);

    // Update display at 30fps
    startTimerHz(30);
}

DAWInfoDisplayEditor::~DAWInfoDisplayEditor()
{
}

void DAWInfoDisplayEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);

    // Draw the main display area
    auto displayArea = getLocalBounds().reduced(10);
    displayArea.removeFromTop(80); // Space for controls

    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(displayArea.toFloat(), 5.0f);

    // Get font size
    int fontSize = static_cast<int>(processor.parameters.getRawParameterValue("fontSize")->load());

    // Draw the value
    g.setColour(juce::Colours::lime);
    g.setFont(juce::FontOptions(static_cast<float>(fontSize)).withStyle("Bold"));
    g.drawText(getDisplayString(), displayArea, juce::Justification::centred);

    // Draw control area background
    auto controlArea = getLocalBounds().reduced(10).removeFromTop(70);
    g.setColour(juce::Colours::darkslategrey);
    g.fillRoundedRectangle(controlArea.toFloat(), 5.0f);
}

void DAWInfoDisplayEditor::resized()
{
    auto area = getLocalBounds().reduced(15);
    auto controlArea = area.removeFromTop(60);

    controlArea.removeFromTop(10);
    displayModeCombo.setBounds(controlArea.removeFromTop(25));
    controlArea.removeFromTop(5);
    fontSizeSlider.setBounds(controlArea.removeFromLeft(controlArea.getWidth()).withTrimmedLeft(80));
}

void DAWInfoDisplayEditor::timerCallback()
{
    repaint();
}

juce::String DAWInfoDisplayEditor::getDisplayString()
{
    auto info = processor.getCurrentDAWInfo();
    int mode = static_cast<int>(processor.parameters.getRawParameterValue("displayMode")->load());

    switch (mode)
    {
    case 0: // Tempo
        return juce::String(info.tempo, 2) + " BPM";

    case 1: // Sample Position
        return juce::String(static_cast<juce::int64>(info.samplePosition));

    case 2: // PPQ Position
        return juce::String(info.ppqPosition, 3);

    case 3: // Time Signature
        return juce::String(info.timeSigNumerator) + "/" + juce::String(info.timeSigDenominator);

    case 4: // Play State
        return info.isPlaying ? "PLAYING" : "STOPPED";

    case 5: // Recording State
        return info.isRecording ? "RECORDING" : "NOT REC";

    case 6: // Loop State
        return info.isLooping ? "LOOPING" : "NO LOOP";

    default:
        return "---";
    }
}