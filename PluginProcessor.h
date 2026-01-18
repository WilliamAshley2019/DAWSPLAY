#pragma once
#include <JuceHeader.h>

class DAWInfoDisplayProcessor : public juce::AudioProcessor
{
public:
    DAWInfoDisplayProcessor();
    ~DAWInfoDisplayProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "DAW Info Display"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const juce::String getProgramName(int index) override { return {}; }
    void changeProgramName(int index, const juce::String& newName) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Parameters
    juce::AudioProcessorValueTreeState parameters;

    // Current DAW info
    struct DAWInfo
    {
        double tempo = 120.0;
        double samplePosition = 0.0;
        double ppqPosition = 0.0;
        int timeSigNumerator = 4;
        int timeSigDenominator = 4;
        bool isPlaying = false;
        bool isRecording = false;
        bool isLooping = false;
    };

    DAWInfo getCurrentDAWInfo() const { return currentInfo; }

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    DAWInfo currentInfo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DAWInfoDisplayProcessor)
};