#include "Ohjain.h"


std::string getFrameFilename() {
    //hae uuden framen kuvatiedoston nimi. huom: ei tarkisteta mitenkään meneekö vanhan päälle
    static int n = 0;
    string result = "frames/frame"+ofToString(n)+".png";
    n++;
    return result;
}


Ohjain::Ohjain() : hue(ofRandom(256) ), hueRange(128), saturation(128),saturationRange(128),brightness(128), brightnessRange(128) {

}


void Ohjain::nextFrame() {
    frame_n++;
    if(frame_n >= Arkisto::valikoitujenMaksimiKoko()) 
        frame_n = 0;
}


void Ohjain::reset() {
    Arkisto::valikoiVarinMukaan(hue, hueRange,saturation,saturationRange,brightness,brightnessRange);
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
    ofDrawBitmapString("hue range: " + ofToString(hueRange), x, y + 100);

    ofDrawBitmapString("saturation: " + ofToString(saturation), x, y + 120);
    ofDrawBitmapString("saturation range: " + ofToString(saturationRange), x, y + 140);

    ofDrawBitmapString("brightness: " + ofToString(brightness), x, y + 160);
    ofDrawBitmapString("brightness range: " + ofToString(brightnessRange), x, y + 180);
    
    ofDrawBitmapString("pensseleitä: " + ofToString(Multimonitori::pensselit.size() ), x, y + 200);
    ofDrawBitmapString("frame: " + ofToString(frame_n), x, y + 220);
    
    ofColor col;
    col = ofColor::white;
    col.setHsb(ofWrap(hue-hueRange,0,256),ofClamp(saturation-saturationRange,0,255),ofClamp(brightness-brightnessRange,0,255));
    ofSetColor(col);
    ofDrawRectangle(0,ofGetHeight()-50,50,50);
    
    col.setHsb(ofWrap(hue+hueRange,0,256),ofClamp(saturation+saturationRange,0,255),ofClamp(brightness+brightnessRange,0,255));
    ofSetColor(col);
    ofDrawRectangle(50,ofGetHeight()-50,50,50);
    
}

void Ohjain::keyPressed(int key) {
    Tilat::keyPressed(key);

    if (key == OF_KEY_TAB)
        Tilat::vaihdaTilaa();

    if (Tilat::tila == Selaa) {
        
        //hue wasd
        if(key =='w')
            hueRange = ofClamp(hueRange += 0.1,0,128);
        else if(key =='s')
            hueRange = ofClamp(hueRange -= 0.1,0,128);
        else if(key =='a')
            hue = ofWrap(--hue, 0, 256);
        else if(key =='d')
            hue = ofWrap(++hue, 0, 256);

        //saturation tfgh
        else if(key =='t')
            saturationRange = ofClamp(saturationRange += 0.1,0,128);
        else if(key =='g')
            saturationRange = ofClamp(saturationRange -= 0.1,0,128);
        else if(key =='f')
            saturation = ofWrap(--saturation, 0, 256);
        else if(key =='h')
            saturation = ofWrap(++saturation, 0, 256);

        //brightness ijkl
        else if(key =='i')
            brightnessRange = ofClamp(brightnessRange += 0.1,0,129);
        else if(key =='k')
            brightnessRange = ofClamp(brightnessRange -= 0.1,0,129);
        else if(key =='j')
            brightness = ofWrap(--brightness, 0, 256);
        else if(key =='l')
            brightness = ofWrap(++brightness, 0, 256);
        else if(key == '1')
            ;
        else return;
        
        //päivitä valikoima ja aloita piirto alusta, jos jotain näppäimistä painettiin:
        reset();
    }
}
