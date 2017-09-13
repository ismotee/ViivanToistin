#include "Arkisto.h"
#include <algorithm>

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

    for (int i = 0; i < kaikki.size(); i++) {
        Viiva& viiva = kaikki[i];
        if (abs(viiva.vari.getHue() - hue) < range || abs(255 + viiva.vari.getHue() - hue) < range || abs(255 - viiva.vari.getHue() - hue) < range)
            valikoidut.push_back(viiva);
    }

}

void Arkisto::valikoiVarinMukaan(float hue, float hueRange, float saturation, float satRange, float brightness, float brightRange) {
    valikoidut.clear();
    vector<int> valikoidutId;

    //hue
    for (int i = 0; i < kaikki.size(); i++) {
        Viiva viiva = kaikki[i];

        if (hue > hueRange && hue < 255 - hueRange) {
            if (hue - hueRange < viiva.vari.getHue() && viiva.vari.getHue() < hue + hueRange) {
                valikoidut.push_back(viiva);
            }
        } else if (hue <= hueRange) {
            if (hue - hueRange + 255 < viiva.vari.getHue()|| viiva.vari.getHue() < hue + hueRange) {
                valikoidut.push_back(viiva);
            }
        } else if (hue >= 255 - hueRange) {
            if (hue - hueRange < viiva.vari.getHue() || viiva.vari.getHue() < hue + hueRange - 255)
              //  cout << viiva.vari.getHue() << '\n';
                valikoidut.push_back(viiva);
        }
    }
    //saturaatio
    for (int i = 0; i < valikoidut.size(); i++) {

            if ( valikoidut[i].vari.getSaturation() < (saturation - satRange) || valikoidut[i].vari.getSaturation() >= (satRange+ saturation) ) {
                cout << saturation - satRange << '\n';
                valikoidut.erase(valikoidut.begin() + i);
                // koska poistettiin niin iteroidaan sama indeksi
                i--;
            }
    }
    //brightness
    for (int i = 0; i < valikoidut.size(); i++) {

            if (valikoidut[i].vari.getBrightness() < (brightness -brightRange) || valikoidut[i].vari.getBrightness() > (brightness + brightRange)) {
                valikoidut.erase(valikoidut.begin()+i);
                // koska poistettiin niin iteroidaan sama indeksi
                i--;
        }
    }

    for (int i : valikoidutId) {
        valikoidut.push_back(kaikki[i]);
    }


}

int Arkisto::valikoitujenMaksimiKoko() {
    int max = 0;
    for (Viiva& v : valikoidut) {
        if (max < v.pisteet.size())max = v.pisteet.size();
    }
    return max;
}
