#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

class FaoxiDriveAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    FaoxiDriveAudioProcessorEditor(FaoxiDriveAudioProcessor&);
    ~FaoxiDriveAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    FaoxiDriveAudioProcessor& audioProcessor;
    juce::Slider sliderDrive; // : le composant graphique
    juce::Label labelDrive;
   /*Une attachment : l'objet qui relie le slider au paramètre, dans les deux sens.
    *Tu tournes le potard → le paramètre change.
    */
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachementDrive;

    juce::ComboBox comboMode;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> attachementMode;


    juce::Slider sliderVolume; // : le composant graphique
    juce::Label labelVolume;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachementVolume;

    juce::Slider sliderTone; // : le composant graphique
    juce::Label labelTone;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachementTone;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaoxiDriveAudioProcessorEditor)
};