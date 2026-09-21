#include "PluginProcessor.h"
#include "PluginEditor.h"

FaoxiDriveAudioProcessor::FaoxiDriveAudioProcessor()
    : AudioProcessor(BusesProperties()
    .withInput("Input",juce::AudioChannelSet::stereo(),true)
    .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    parametres(*this, nullptr, "Parametres", creerParametres())
{
}

FaoxiDriveAudioProcessor::~FaoxiDriveAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
FaoxiDriveAudioProcessor::creerParametres()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "drive", 1 }, // drive est l'identifaint interne, et 1 c'est la version
        "Drive", // Drive est le nom affiché dans ableton
        1.0f, 50.0f, 10.0f));
    // min , max, default

    // on ajoute un deuxieme parametres
    layout.add(std::make_unique<juce::AudioParameterChoice>(
    juce::ParameterID { "mode", 1 },
    "Mode",
    juce::StringArray { "Hard", "Soft" }, //liste des options affichées
    1)); // par defaut c'est Soft clipping

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "volume", 1 },
        "Volume", // Volume est le nom affiché dans ableton
        -24.0f, 6.0f, 0.0f));

    // on change la plage
    juce::NormalisableRange<float> plageTone(500.0f, 20000.0f);
    plageTone.setSkewForCentre(3000.0f); // quand le potard est au milieu je veux 3000 Hz;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "tone", 1 },
        "Tone",
        plageTone,
        20000.0f));


    return layout;
}

void FaoxiDriveAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // pour chaque filtre dans filtres,on remet la mémoire des deux filtres à zéro.
    for (auto& filtre : filtres)
        filtre.preparer(static_cast<float>(sampleRate));
    //après cette boucle, chaque filtre connaît le sample rate et repart d'une mémoire vide
    juce::ignoreUnused(samplesPerBlock);



}

void FaoxiDriveAudioProcessor::releaseResources()
{
}

void FaoxiDriveAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                  juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    /* getRawParameterValue rend un pointeur vers le std::atomic<float>
    a flèche -> sert à appeler une méthode à travers un pointeur (le point . est pour un objet direct)
    Et load() est la lecture atomique donc lit la valeur qui est dans la case.
    */
    const float drive = parametres.getRawParameterValue("drive")->load();
    distorsion.setDrive(drive);

    const int mode = static_cast<int>(parametres.getRawParameterValue("mode")->load());
    distorsion.setModeDoux(mode == 1);

    const float volumeDb = parametres.getRawParameterValue("volume")->load();
    const float facteur = juce::Decibels::decibelsToGain(volumeDb);

    const float tone = parametres.getRawParameterValue("tone")->load();

    for (auto& filtre : filtres)
        filtre.setFrequence(tone);






    const int nombreEchantillons = buffer.getNumSamples();
    // return le min entre deux valeurs
    const int nombreCanaux = juce::jmin(buffer.getNumChannels(), static_cast<int>(filtres.size()));
    // boucle externe sur les canaux :
    for (int canal = 0; canal < nombreCanaux; canal++ ) {
        // on recupere le pointeur :
        auto* donnees = buffer.getWritePointer(canal);
        for (int j = 0; j < nombreEchantillons; j++ ) {
            float echantillon = donnees[j];

            echantillon = distorsion.traiterEchantillon(echantillon);
            echantillon = filtres[canal].traiterEchantillon(echantillon);
            echantillon = echantillon * facteur;

            donnees[j] = echantillon;
        }
    }
}

juce::AudioProcessorEditor* FaoxiDriveAudioProcessor::createEditor()
{
    return new FaoxiDriveAudioProcessorEditor(*this);
}

//les DAW appelle cette méthode quand il sauvegarde le projet.
//on copie les valeurs des paramètres, on les transforme en XML
void FaoxiDriveAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto etat = parametres.copyState(); // retourne une copy de l'apvts

    // on convertit en XML
    // on creer un pointeur qui s'appelle xml et  createXml() transforme l'arbre en XML, un format texte
    std::unique_ptr<juce::XmlElement> xml(etat.createXml());

    // cette fonction convertit du xml en binaire et les ecrit dans destData.
    //xml est un pointeur, une adresse. La fonction veut l'objet lui-même, pas son adresse.
    //L'étoile dit « donne-moi ce qui se trouve à cette adresse ».
    copyXmlToBinary(*xml, destData);
}

// Le Daw appelle cette méthode à l'ouverture du projet.
void FaoxiDriveAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // on recupere l'adresse
    //getXmlFromBinary relit les octet et reconstruit l'objet XML
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    //si les données sont illisibles — fichier abîmé, données d'un autre plugin,
    //ancienne version — elle rend nullptr, un pointeur qui ne pointe vers rien.

    //xml != nullptr : le XML a-t-il pu être reconstruit ?
    //xml->hasTagName(parametres.state.getType()) : la balise principale porte-t-elle le bon nom ?
    //parametres.state.getType() rend le nom de ton arbre, "Parametres".
    //Donc on vérifie que le XML commence bien par <Parametres>, et pas par autre chose.
    if (xml != nullptr && xml->hasTagName(parametres.state.getType()))

        //*xml : l'objet XML lui-même, pas son adresse.
        // juce::ValueTree::fromXml(...) : reconvertit le XML en arbre de données.
        // parametres.replaceState(...) : l'APVTS remplace ses valeurs par celles de l'arbre.
        parametres.replaceState(juce::ValueTree::fromXml(*xml));
}


bool FaoxiDriveAudioProcessor::hasEditor() const { return true; }
const juce::String FaoxiDriveAudioProcessor::getName() const { return JucePlugin_Name; }
bool FaoxiDriveAudioProcessor::acceptsMidi() const { return false; }
bool FaoxiDriveAudioProcessor::producesMidi() const { return false; }
double FaoxiDriveAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int FaoxiDriveAudioProcessor::getNumPrograms() { return 1; }
int FaoxiDriveAudioProcessor::getCurrentProgram() { return 0; }
void FaoxiDriveAudioProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }
const juce::String FaoxiDriveAudioProcessor::getProgramName(int index) { juce::ignoreUnused(index); return {}; }
void FaoxiDriveAudioProcessor::changeProgramName(int index, const juce::String& newName) { juce::ignoreUnused(index, newName); }

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FaoxiDriveAudioProcessor();
}