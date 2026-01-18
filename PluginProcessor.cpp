#include "PluginProcessor.h"
#include "PluginEditor.h"

DAWInfoDisplayProcessor::DAWInfoDisplayProcessor()
    : AudioProcessor(BusesProperties()),
    parameters(*this, nullptr, "Parameters", createParameterLayout())
{
}

DAWInfoDisplayProcessor::~DAWInfoDisplayProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout DAWInfoDisplayProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "displayMode",
        "Display Mode",
        juce::StringArray{ "Tempo", "Sample Position", "PPQ Position",
                           "Time Signature", "Play State", "Recording State", "Loop State" },
        0));

    layout.add(std::make_unique<juce::AudioParameterInt>(
        "fontSize",
        "Font Size",
        12, 200, 72));

    return layout;
}

void DAWInfoDisplayProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
}

void DAWInfoDisplayProcessor::releaseResources()
{
}

bool DAWInfoDisplayProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return true;
}

void DAWInfoDisplayProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    // Get transport info from the DAW
    auto playHead = getPlayHead();
    if (playHead != nullptr)
    {
        if (auto posInfo = playHead->getPosition())
        {
            currentInfo.tempo = posInfo->getBpm().orFallback(120.0);
            currentInfo.timeSigNumerator = posInfo->getTimeSignature().orFallback(juce::AudioPlayHead::TimeSignature()).numerator;
            currentInfo.timeSigDenominator = posInfo->getTimeSignature().orFallback(juce::AudioPlayHead::TimeSignature()).denominator;
            currentInfo.ppqPosition = posInfo->getPpqPosition().orFallback(0.0);
            currentInfo.samplePosition = posInfo->getTimeInSamples().orFallback(0.0);
            currentInfo.isPlaying = posInfo->getIsPlaying();
            currentInfo.isRecording = posInfo->getIsRecording();
            currentInfo.isLooping = posInfo->getIsLooping();
        }
    }
}

juce::AudioProcessorEditor* DAWInfoDisplayProcessor::createEditor()
{
    return new DAWInfoDisplayEditor(*this);
}

void DAWInfoDisplayProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DAWInfoDisplayProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DAWInfoDisplayProcessor();
}