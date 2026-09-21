//
// Created by Bourguignon Mathis on 16/09/2026.
//
#pragma once

class Distorsion {
public:
    float traiterEchantillon(float echantillon) const;
    void setDrive(float nouveauDrive);
    void setModeDoux(bool doux);

private:
    float niveauSortie = 0.5f;
    float drive = 10.0f;
    bool modeDoux = true;

};


