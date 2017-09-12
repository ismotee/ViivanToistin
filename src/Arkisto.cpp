#include "Arkisto.h"

bool Arkisto::lataaViivatHakemistosta(std::string polku) {
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
        if (viiva.vari.getHue() > ofWrap(hue - range,0, 256) && viiva.vari.getHue() < ofWrap(hue + range,0,256))
            valikoidut.push_back(viiva);
    }
}
