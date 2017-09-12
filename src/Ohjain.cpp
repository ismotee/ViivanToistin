#include "Ohjain.h"

Ohjain::Ohjain() : hue(ofRandom(256) ), range(2) {

}

void Ohjain::setup() {
    Arkisto::lataaViivatHakemistosta("viivat/");
    Multimonitori::setup();
}

void Ohjain::update() {
    Tilat::update();
}

void Ohjain::selaa() {
    //rujo automaatio. TODO: tehdään tämä kunnolla
    static int n = 0;
    n++;
    if(n > 300) 
        n = 0;    

    
    //haetaan viivat ja piirretään n pistettä alusta:
    vector<Viiva> kopio = Arkisto::haeValikoidut();
    Multimonitori::piirraViivatAlusta(kopio, n);

}

void Ohjain::tallenna() {

}

void Ohjain::debugDraw(int x, int y) {
    Tilat::debugDraw(x, y);
    
    ofDrawBitmapString("viivoja yhteensa: " + ofToString(Arkisto::kaikki.size()), x, y + 30);
    ofDrawBitmapString("valikoituja: " + ofToString(Arkisto::valikoidut.size()), x, y + 50);
    
    ofDrawBitmapString("hue: " + ofToString(hue), x, y + 80);
    ofDrawBitmapString("range: " + ofToString(range), x, y + 100);
    
    ofDrawBitmapString("pensseleitä: " + ofToString(Multimonitori::pensselit.size() ), x, y + 120);
    
    
    
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
        if (key == 'r')
            hue = ofRandom(256);

        Arkisto::valikoiHuenMukaan(hue, range);
    }
}
