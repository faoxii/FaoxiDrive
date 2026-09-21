#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "distorsion.h"
#include "filtre.h"

class FaoxiDriveAudioProcessor : public juce::AudioProcessor
{
public:
    FaoxiDriveAudioProcessor();
    ~FaoxiDriveAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;


    juce::AudioProcessorValueTreeState parametres;
    static juce::AudioProcessorValueTreeState::ParameterLayout creerParametres();

private:
    Distorsion distorsion;
    std::array<FiltrePasseBas, 2> filtres; // tableau contentnat des element de type FIltrePasseBas et  de taille 2

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaoxiDriveAudioProcessor)
};