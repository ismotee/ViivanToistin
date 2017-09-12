#include "Monitori.h"
#include "tilastot.h"

void smooth::add(float newValue) {
    static int add_i = 0;
    
    if(values.size() == max_size) {
        values[add_i] = newValue;
        add_i++;
        if(add_i >= max_size) 
            add_i = 0;
    }
    else if (values.size() < max_size) {
        values.push_back(newValue);
    }
    else if (values.size() > max_size)
        values.resize(max_size);
}


float smooth::get() {
    return keskiarvo(values);
}


void pensseli::setup() {
    brushFbo.allocate(MAX_KOKO, MAX_KOKO, GL_RGBA);
    brushFbo.begin();
        ofClear(clearColor);
    brushFbo.end();
    ofEnableAlphaBlending();
}


void pensseli::drawBrush() {       
    const int blur_steps = 10;
    //float koko_step = koko / (2*blur_steps);
    ofPoint P(MAX_KOKO/2, MAX_KOKO/2);
    
    brushFbo.begin();
        ofClear(clearColor);
        
        //piirretään valkoisella brush läpinäkyvälle. Stroken piirrossa käytetään värimodulointia
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        int alpha = 2.6 * 256 / blur_steps;
        ofSetColor(255,255,255, alpha);
        
        for(unsigned int blur_i = 0; blur_i < blur_steps; blur_i++) {
            float suhde = (float)blur_i / (float)blur_steps;
            float r = pow(suhde, blur) * (koko * (1 + blur/3) ); //maksimisäde on (2+1/3) * koko. Maksimihalkaisija on siis (2+1/3) * 2 * koko. Joten: koko <= MAX_KOKO/(4+2/3)
            ofDrawCircle(P, r);
        }
        
    brushFbo.end();
}


void pensseli::strokeTo(ofPoint kohde) {
    //jos viivaa ei ole aloitettu, aloitetaan se nyt eikä vielä piirretä
    if(!viivaJatkuu) {
        sijainti = kohde;
        viivaJatkuu = true;
        return;
    }
    
    if(sijainti == kohde) {
        return;
    }
    
    //päivitä uusi pensseli parametrien mukaan
    drawBrush();
    
    //tee viiva nykyisestä sijainnista kohteeseen
    ofVec2f v = kohde - sijainti;
    float L = v.length();
    
    //tehdään v:stä suuntavektori
    v.normalize();
    
    //piirrä tasavälisesti brushia viivalle:
    if(koko < 1) koko = 1;
    float step = koko * spacing;    
    ofVec2f brushMiddle(-MAX_KOKO/2, -MAX_KOKO/2);
    
    //blendausasetukset fbo:oon piirtämistä varten
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    for(float l = 0; l < L; l += step)  {
        ofSetColor(vari);
        brushFbo.draw(sijainti + v*l + brushMiddle);
    }
    
    //päivitetään sijainti
    sijainti = kohde;
}


void pensseli::moveTo(ofPoint kohde) {
    sijainti = kohde;
}


void pensseli::lopetaViiva() {
    viivaJatkuu = false;
    //nyt jos pyydetään piirtämään, täytyy siirtää ensin alkupää paikalleen
}


void Monitori::setup() {
    pensseli::setup();
    //alusta viivaFbo
    viivaFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    viivaFbo.begin();
        ofClear(pensseli::clearColor);
    viivaFbo.end();
    
}


void Monitori::draw() {
    ofClear(taustaVari);
    
    ofSetColor(255,255,255, pow(viivanAlfa, 0.7) * 255);
    
    viivaFbo.draw(0,0);
    
    if(fadeOut && viivanAlfa > 0) {
        viivanAlfa -= 0.025;
        if(viivanAlfa <0) {
            viivanAlfa = 0;
            fadeOut = false;
        }
    }
    else if(fadeIn && viivanAlfa < 1) {
        viivanAlfa += 0.015;
        if(viivanAlfa > 1) {
            viivanAlfa = 1;
            fadeIn = false;
        }
    }
}


void Monitori::piirraViiva(const Viiva& viiva) {
    if(viiva.pisteet.empty() ) {
        return;
    }    

    //sumeus on 0...1
    float sumeus = viiva.haeViimeisinSumeus().arvo;
    
    //jos sumeus on täysi, ei piirretä mitään
    if(sumeus == 1) {
        pensseli::moveTo(viiva.pisteet.back().sijainti);
        return;
    }    
    
    //paksuus on 0...1
    //pehmennetään ottamalla 6 viimeistä arvoa
    float paksuus = keskiarvo(viiva.haeArvot(&viiva.paksuus, 6) );
    
    // blur: 0...16
    pensseli::blur = ofClamp(pow(sumeus, 2) * 16, 0.1, 16);
    
    // koko: 0 ... MAX_KOKO/(4+2/3)
    pensseli::koko = ofClamp(pow(paksuus, 0.7) * MAX_KOKO / (4+2/3), 1, MAX_KOKO / (4+2/3) );    
    
    viivaFbo.begin();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        pensseli::strokeTo( viiva.pisteet.back().sijainti );
    viivaFbo.end();
}


void Monitori::piirraKokoViiva(const Viiva& viiva) {
    if(viiva.pisteet.empty() ) {
        return;
    }
    
    smooth paksuusSmooth;
    
    tyhjenna();
    piirraVari(viiva.vari);
    std::cout << "väri: " << (int)viiva.vari.r << ", " << (int)viiva.vari.g << ", " << (int)viiva.vari.b << "\n";
    
    std::vector<float> sumeudet = viiva.haeArvot(&viiva.sumeus);
    std::vector<float> paksuudet = viiva.haeArvot(&viiva.paksuus);
    
    if(sumeudet.size() != viiva.pisteet.size() || paksuudet.size() != viiva.pisteet.size() ) {
        std::cerr << "Monitori::piirraKokoViiva: vektorien koko ei täsmää\n";
        return;
    }

    viivaFbo.begin();
    
    for(unsigned int i=0; i<viiva.pisteet.size(); i++) {
        //sumeus on 0...1
        float sumeus = sumeudet[i];

        //jos sumeus on täysi, ei piirretä mitään
        if(sumeus == 1) {
            pensseli::moveTo(viiva.pisteet[i].sijainti);
            continue;
        }

        //paksuus on 0...1
        //todo: pehmennys ottamalla 8 viimeistä arvoa
        paksuusSmooth.add(paksuudet[i]);
        float paksuus = paksuusSmooth.get();

        // blur: 0...16
        pensseli::blur = ofClamp(pow(sumeus, 2) * 16, 0.1, 16);

        // koko: 0 ... MAX_KOKO/(4+2/3)
        pensseli::koko = ofClamp(pow(paksuus, 0.7) * MAX_KOKO / (4+2/3), 1, MAX_KOKO / (4+2/3) );

        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        pensseli::strokeTo( viiva.pisteet[i].sijainti );
    }
    viivaFbo.end();
}


void Monitori::piirraViivaAlusta(const Viiva& viiva, unsigned int n) {
}


void Monitori::piirraVari(ofColor vari_) {
    //taustaa piirretään koko ajan, joten tämä riittää:
    taustaVari = vari_;
    pensseli::vari = vari_;
}


void Monitori::tyhjenna() {
    viivaFbo.begin();
        ofClear(pensseli::clearColor);
    viivaFbo.end();
    pensseli::lopetaViiva();
}


void Monitori::paljasta() {
    fadeOut = false;
    fadeIn = true;
    showing = true;
}


void Monitori::piilota() {
    fadeOut  = true;
    fadeIn = false;
    showing = false;
}


void Monitori::tallennaKuvana(std::string tiedosto) {
    ofPixels px;
    viivaFbo.readToPixels(px);
    ofSaveImage(px, tiedosto);
}


void Monitori::tallennaKartta(const std::vector<Viiva>& viivat, std::string filename) {
    if(viivat.empty() ) {
        std::cerr << "Monitori::tallennaKartta: Ei viivoja!\n";
        return;
    }
    
    //luodaan kartta omaan fbo:oon
    ofFbo karttaFbo;
    int w = ofGetWidth() * 5;
    int h = ofGetHeight() * 5;
    karttaFbo.allocate(w + ofGetWidth(), h + ofGetHeight(), GL_RGBA);
    karttaFbo.begin();
    //taustavärinä sama kuin blurrin reunat
    ofClear(pensseli::clearColor);
    
    std::cerr << "kartan koko: " << w << " x " << h << "\n";
    std::cout << "max: " << GL_MAX_FRAMEBUFFER_WIDTH << " x " << GL_MAX_FRAMEBUFFER_HEIGHT << "\n";
    
    //piirretään viivat karttaan
    for(unsigned int i=0; i<viivat.size(); i++) {
        piirraKokoViiva(viivat[i]);
        float x = viivat[i].haeViimeisinPaksuus().keskiarvo * w;
        float y = (1 - viivat[i].haeViimeisinSumeus().keskiarvo) * h;
        std::cerr << "piirretään karttaan (" << x << ", " << y << ")\n";
        ofPushMatrix();
            ofScale(0.5, 0.5);
            viivaFbo.draw(2*x, 2*y);
        ofPopMatrix();
        //ofDrawBitmapStringHighlight(ofToString(viivat[i].haeViimeisinSumeus().arvo), x, y, ofColor::white, ofColor::black);
        //ofSetColor(viivat[i].vari);
        //ofDrawCircle(x,y,10);
    }
    
    karttaFbo.end();
    
    ofPixels px;
    karttaFbo.readToPixels(px);
    ofSaveImage(px, filename);    
}