#include "Ohjain.h"

Ohjain::Ohjain() : hue(0), range(10) {

}

void Ohjain::setup() {
    Arkisto::lataaViivatHakemistosta("viivat/");
    Monitori::setup();
}

void Ohjain::update() {
    Tilat::update();
}

void Ohjain::selaa() {
    vector<Viiva> kopio = Arkisto::haeValikoidut();
    for(const Viiva& viiva : kopio)
        Monitori::piirraViivaAlusta(viiva, 100);
}

void Ohjain::tallenna() {

}

void Ohjain::debugDraw(int x, int y) {
    Tilat::debugDraw(x, y);
    
    ofDrawBitmapString("viivoja yhteensa: " + ofToString(Arkisto::kaikki.size()), x, y + 30);
    ofDrawBitmapString("valikoituja: " + ofToString(Arkisto::valikoidut.size()), x, y + 50);
    
    ofDrawBitmapString("hue: " + ofToString(hue), x, y + 80);
    ofDrawBitmapString("range: " + ofToString(range), x, y + 100);
    
    
    
}

void Ohjain::keyPressed(int key) {
    Tilat::keyPressed(key);

    if (Tilat::tila == Selaa) {
        if (key == '1')
            Arkisto::valikoiHuenMukaan(hue, range);
        if (key == OF_KEY_DOWN)
            range = ofClamp(range -= 0.1, 0, 127);
        if (key == OF_KEY_UP)
            range = ofClamp(range += 0.1, 0, 127);
        if (key == OF_KEY_RIGHT)
            hue = ofWrap(++hue, 0, 256);
        if (key == OF_KEY_LEFT)
            hue = ofWrap(--hue, 0, 256);
        if(key == OF_KEY_TAB)
            Monitori::paljasta();

        Arkisto::valikoiHuenMukaan(hue, range);
    }
}
