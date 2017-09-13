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
    Arkisto::valikoiVarinMukaan(hue, range, saturation, range, brightness, range);
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
    ofDrawBitmapString("saturation: " + ofToString(saturation), x, y + 100);
    ofDrawBitmapString("brightness: " + ofToString(brightness), x, y + 120);
    ofDrawBitmapString("range: " + ofToString(range), x, y + 140);
        
    ofDrawBitmapString("frame: " + ofToString(frame_n), x, y + 160);
}

void Ohjain::keyPressed(int key) {
    Tilat::keyPressed(key);

    if (key == OF_KEY_TAB)
        Tilat::vaihdaTilaa();

    if (Tilat::tila == Selaa) {
        //nuolinäppäimet muuttavat saturaatiota ja kirkkautta
        //r arpoo huen
        //tab vaihtaa moodia
        if (key == OF_KEY_DOWN)
            brightness = ofClamp(brightness - 1, 0, 255);
        else if (key == OF_KEY_UP)
            brightness = ofClamp(brightness + 1, 0, 255);
        else if (key == OF_KEY_RIGHT)
            saturation = ofClamp(saturation + 1, 0, 255);
        else if (key == OF_KEY_LEFT)
            saturation = ofClamp(saturation - 1, 0, 255);
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
