#pragma once
#include "ofMain.h"
#include "Viiva.h"
#include "tiedosto.h"

struct Arkisto {
    vector<Viiva> kaikki;
    vector<Viiva> valikoidut;

    bool lataaViivatHakemistosta(std::string polku);
    void valikoiHuenMukaan(float hue, float range);
    const std::vector<Viiva>& haeValikoidut() const;
};
