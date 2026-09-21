//
// Created by Bourguignon Mathis on 16/09/2026.
//

#include "distorsion.h"
#include <cmath>

void Distorsion::setModeDoux(bool doux) {
    modeDoux = doux;
}

float Distorsion::traiterEchantillon(float echantillon) const {
    echantillon *= drive; // on rehausse le niveau

    if (modeDoux) {
        echantillon = std::tanh(echantillon);
        return echantillon*niveauSortie;
    }

    if (echantillon < -niveauSortie) {
        echantillon = -niveauSortie;
    }
    else if (echantillon > niveauSortie) {
        echantillon = niveauSortie;
    }



    return echantillon;
}


void Distorsion::setDrive(float nouveauDrive) {
    drive = nouveauDrive;
}
