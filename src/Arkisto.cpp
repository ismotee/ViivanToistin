#include "Arkisto.h"

bool Arkisto::lataaViivatHakemistosta(std::string polku) {
    kaikki.clear();
    ofDirectory dir(polku);
    if (!dir.exists()) {
        cout << "yritettiin ladata " << dir.getAbsolutePath() << "\n";
        return false;
    }
    cout << "ladataan " << dir.getAbsolutePath() << "\n";

    dir.listDir();

    for (int i = 0; i < dir.size(); i++)
        kaikki.push_back(tiedosto::lataaViiva(dir[i].getAbsolutePath()));

}

const std::vector<Viiva>& Arkisto::haeValikoidut() const {
    return valikoidut;
}

void Arkisto::valikoiHuenMukaan(float hue, float range) {
    valikoidut.clear();

    for (Viiva& viiva : kaikki) {

        if (abs(viiva.vari.getHue() - hue) < range || abs(255 + viiva.vari.getHue() - hue) < range || abs(255 - viiva.vari.getHue() - hue) < range)
            valikoidut.push_back(viiva);
    }

}

int Arkisto::valikoitujenMaksimiKoko() {
    int max = 0;
    for(Viiva& v : valikoidut) {
        if(max < v.pisteet.size())max = v.pisteet.size();
    }
    return max;
}
