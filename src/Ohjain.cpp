#include "Ohjain.h"


std::string getFrameFilename() {
    //hae uuden framen kuvatiedoston nimi. huom: ei tarkisteta mitenkään meneekö vanhan päälle
    static int n = 0;
    string result = "frames/frame"+ofToString(n)+".png";
    n++;
    return result;
}


Ohjain::Ohjain() : hue(ofRandom(256) ), hueRange(60), saturation(128),saturationRange(128),brightness(0), brightnessRange(30) {

}


void Ohjain::nextFrame() {
    frame_n++;
    if(frame_n >= Arkisto::valikoitujenMaksimiKoko()) {
        frame_n = 0;
        if(Tilat::tila == Tallenna)
            vaihdaTilaa();
        reset();
    }
}


void Ohjain::reset() {
    Arkisto::valikoiVarinMukaan(hue, hueRange,saturation,saturationRange,brightness,brightnessRange);
    frame_n = 0;
    Multimonitori::lopetaViivat();
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
    //Multimonitori::piirraViivatKohdasta(kopio, frame_n);
        
    nextFrame();
    
    if(shiftHold)
        muutosKerroin = 100;
    else
        muutosKerroin = 1;
    
}

void Ohjain::tallenna() {

    //haetaan viivat ja piirretään n pistettä alusta:
    vector<Viiva> kopio = Arkisto::haeValikoidut();
    Multimonitori::piirraViivatKohdasta(kopio, frame_n);
    
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
    ofDrawBitmapString("frame: " + ofToString(frame_n) + " / " + ofToString(Arkisto::valikoitujenMaksimiKoko()), x, y + 220);
    
    ofColor col;
    col = ofColor::white;
    col.setHsb(ofWrap(hue-hueRange,0,256),ofClamp(saturation-saturationRange,0,255),ofClamp(brightness-brightnessRange,0,255));
    ofSetColor(col);
    ofDrawRectangle(0,ofGetHeight()-50,50,50);
    
    col.setHsb(ofWrap(hue+hueRange,0,256),ofClamp(saturation+saturationRange,0,255),ofClamp(brightness+brightnessRange,0,255));
    ofSetColor(col);
    ofDrawRectangle(50,ofGetHeight()-50,50,50);
    
    ofDrawBitmapString(ofToString(shiftHold),x,y+240);
}

void Ohjain::keyPressed(int key) {
    Tilat::keyPressed(key);

    if (key == OF_KEY_TAB) {
        Tilat::vaihdaTilaa();
        reset();
        //Multimonitori::tyhjenna();
    }
    
    if( key == OF_KEY_SHIFT)
        shiftHold = !shiftHold;
    
    if (Tilat::tila == Selaa) {
        
        //hue wasd
        if(key =='w')
            hueRange = ofClamp(hueRange + muutosKerroin,0,128);
        else if(key =='s')
            hueRange = ofClamp(hueRange - muutosKerroin,0,128);
        else if(key =='a')
            hue = ofWrap(hue-muutosKerroin, 0, 256);
        else if(key =='d')
            hue = ofWrap(hue + muutosKerroin, 0, 256);

        //saturation tfgh
        else if(key =='t')
            saturationRange = ofClamp(saturationRange + muutosKerroin,0,128);
        else if(key =='g')
            saturationRange = ofClamp(saturationRange - muutosKerroin,0,128);
        else if(key =='f')
            saturation = ofClamp(saturation-muutosKerroin, 0, 255);
        else if(key =='h')
            saturation = ofClamp(saturation+muutosKerroin, 0, 255);

        //brightness ijkl
        else if(key =='i')
            brightnessRange = ofClamp(brightnessRange+ muutosKerroin,0,129);
        else if(key =='k')
            brightnessRange = ofClamp(brightnessRange- muutosKerroin,0,129);
        else if(key =='j')
            brightness = ofClamp(brightness-muutosKerroin, 0, 255);
        else if(key =='l')
            brightness = ofClamp(brightness+muutosKerroin, 0, 255);
        
        //r: tyhjennä
        //1: päivitä
        else if (key == 'r')
            Multimonitori::tyhjenna();
        else if(key == '1')
            ;
        
        //muut näppäimet: ei tarvitse päivittää
        else return;
        
        //päivitä valikoima ja aloita piirto alusta, jos jotain näppäimistä painettiin:
        reset();
    }
}

void Ohjain::keyReleased(int key) {
        
}
