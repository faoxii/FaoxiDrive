#include "PluginProcessor.h"
#include "PluginEditor.h"

FaoxiDriveAudioProcessorEditor::FaoxiDriveAudioProcessorEditor(FaoxiDriveAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    sliderDrive.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sliderDrive.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    //addAndMakeVisible ajoute le slider comme enfant de la fenêtre. Sans cette ligne, il existe mais n'est dessiné nulle part.
    addAndMakeVisible(sliderDrive);

    attachementDrive = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parametres, "drive", sliderDrive);


    // comboMode ne connait pas les options du parametre, il faut lui donner avec addItemList
    comboMode.addItemList(juce::StringArray { "Hard", "Soft" }, 1);
    addAndMakeVisible(comboMode);

    attachementMode = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.parametres, "mode", comboMode);

    sliderVolume.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sliderVolume.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(sliderVolume);

    attachementVolume = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parametres, "volume", sliderVolume);



    sliderTone.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sliderTone.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(sliderTone);

    attachementTone = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parametres, "tone", sliderTone);

    labelTone.setText("Tone", juce::dontSendNotification);
    labelTone.setJustificationType(juce::Justification::centred);
    labelTone.attachToComponent(&sliderTone, false);
    addAndMakeVisible(labelTone);


    labelDrive.setText("Drive", juce::dontSendNotification);
    labelDrive.setJustificationType(juce::Justification::centred);
    labelDrive.attachToComponent(&sliderDrive, false);
    addAndMakeVisible(labelDrive);

    labelVolume.setText("Volume", juce::dontSendNotification);
    labelVolume.setJustificationType(juce::Justification::centred);
    labelVolume.attachToComponent(&sliderVolume, false);
    addAndMakeVisible(labelVolume);



    setSize(560, 300);
}

FaoxiDriveAudioProcessorEditor::~FaoxiDriveAudioProcessorEditor()
{
}

void FaoxiDriveAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawFittedText("Distorsion", 0, 20, getWidth(), 30, juce::Justification::centred, 1);
}

void FaoxiDriveAudioProcessorEditor::resized()
{
    const int taille = 150;
    const int ecart = 20;

    const int largeurGroupe = 3 * taille + 2*ecart;
    const int debutGroupe = (getWidth() - largeurGroupe) / 2;
    const int y = (getHeight() - taille) / 2;

    sliderDrive.setBounds(debutGroupe, y, taille, taille);
    sliderTone.setBounds(debutGroupe + taille + ecart, y, taille, taille);
    sliderVolume.setBounds(debutGroupe + 2* taille + 2* ecart, y, taille, taille);

    comboMode.setBounds((getWidth() - 120) / 2, getHeight() - 20 - 24, 120, 24);
}