//
// Created by Bourguignon Mathis on 21/09/2026.
//
#pragma once


class FiltrePasseBas {

public:
    void preparer(float nouveauSampleRate);
    void setFrequence(float frequenceCoupure);
    float traiterEchantillon(float entree);

private:
    float a = 1.0f;
    float sortiePrecedente = 0.0f;
    float sampleRate = 44100.0f;

};



