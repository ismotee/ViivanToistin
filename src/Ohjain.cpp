#include "Ohjain.h"


std::string getFrameFilename() {
    //hae uuden framen kuvatiedoston nimi. huom: ei tarkisteta mitenkään meneekö vanhan päälle
    static int n = 0;
    string result = "frames/frame"+ofToString(n)+".png";
    n++;
    return result;
}


Ohjain::Ohjain() : hue(ofRandom(256) ), range(1) {

}


void Ohjain::nextFrame() {
    frame_n++;
    if(frame_n >= Arkisto::valikoitujenMaksimiKoko()) 
        frame_n = 0;
}


void Ohjain::reset() {
    Arkisto::valikoiHuenMukaan(hue, range);
    frame_n = 0;    
}

void Ohjain::setup() {
    Arkisto::lataaViivatHakemistosta("viivat/");
    Multimonitori::setup();
    
    //päivitä valikoima:
    reset();
}

void Ohjain::update() {
    Tilat::update();
}

void Ohjain::selaa() {
    //haetaan viivat ja piirretään n pistettä alusta:
    vector<Viiva> kopio = Arkisto::haeValikoidut();
    Multimonitori::piirraViivatAlusta(kopio, frame_n);
        
    nextFrame();
}

void Ohjain::tallenna() {
    //haetaan viivat ja piirretään n pistettä alusta:
    vector<Viiva> kopio = Arkisto::haeValikoidut();
    Multimonitori::piirraViivatAlusta(kopio, frame_n);
    
    //tallennetaan kuva:
    Multimonitori::tallennaKuvana(getFrameFilename() );
    
    nextFrame();
}

void Ohjain::debugDraw(int x, int y) {
    Tilat::debugDraw(x, y);
    
    ofDrawBitmapString("viivoja yhteensa: " + ofToString(Arkisto::kaikki.size()), x, y + 30);
    ofDrawBitmapString("valikoituja: " + ofToString(Arkisto::valikoidut.size()), x, y + 50);
    
    ofDrawBitmapString("hue: " + ofToString(hue), x, y + 80);
    ofDrawBitmapString("range: " + ofToString(range), x, y + 100);
    
    ofDrawBitmapString("pensseleitä: " + ofToString(Multimonitori::pensselit.size() ), x, y + 120);
    ofDrawBitmapString("frame: " + ofToString(frame_n), x, y + 140);
}

void Ohjain::keyPressed(int key) {
    Tilat::keyPressed(key);

    if (Tilat::tila == Selaa) {
        if (key == OF_KEY_DOWN)
            range = ofClamp(range -= 0.1, 0, 127);
        else if (key == OF_KEY_UP)
            range = ofClamp(range += 0.1, 0, 127);
        else if (key == OF_KEY_RIGHT)
            hue = ofWrap(++hue, 0, 256);
        else if (key == OF_KEY_LEFT)
            hue = ofWrap(--hue, 0, 256);
        else if (key == 'r')
            hue = ofRandom(256);
        else if (key == '1')
            ;
        else
            return;
        
        //päivitä valikoima ja aloita piirto alusta, jos jotain näppäimistä painettiin:
        reset();
    }
}
