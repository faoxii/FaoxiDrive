//
// Created by Bourguignon Mathis on 21/09/2026.
//

#include "filtre.h"
#include <cmath>

const float pi = 3.14159265358979323846f;

void FiltrePasseBas::preparer(float nouveauSampleRate) {
    sampleRate = nouveauSampleRate;
    sortiePrecedente = 0.0f;
}

void FiltrePasseBas::setFrequence(float frequenceCoupure) {
    a = 1.0f - std::exp(-2.0f * pi * frequenceCoupure / sampleRate);
}

float FiltrePasseBas::traiterEchantillon(float entree) {
    float sortie = sortiePrecedente + a * (entree- sortiePrecedente);
    sortiePrecedente =sortie;
    return sortie;
}
