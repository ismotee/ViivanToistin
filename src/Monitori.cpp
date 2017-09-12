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


ofColor pensseli::clearColor = ofColor(100, 100, 100, 0);

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


void Monitori::teeVeto(ofPoint kohde, float paksuus, float sumeus) {
    //jos sumeus on täysi, ei piirretä mitään
    if(sumeus == 1) {
        pensseli::moveTo(kohde);
        return;
    }
    // koko: 0 ... MAX_KOKO/(4+2/3)
    pensseli::koko = ofClamp(pow(paksuus, 0.7) * MAX_KOKO / (4+2/3), 1, MAX_KOKO / (4+2/3) );    
    // blur: 0...16
    pensseli::blur = ofClamp(pow(sumeus, 2) * 16, 0.1, 16);    
    
    viivaFbo.begin();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        pensseli::strokeTo( kohde );
    viivaFbo.end();    
}


void Monitori::piirraViiva(const Viiva& viiva) {
    if(viiva.pisteet.empty() ) {
        return;
    }    

    //paksuus on 0...1
    //pehmennetään ottamalla 6 viimeistä arvoa
    float paksuus = keskiarvo(viiva.haeArvot(&viiva.paksuus, 6) );

    //sumeus on 0...1
    float sumeus = viiva.haeViimeisinSumeus().arvo;
    
    //jos sumeus on täysi, ei piirretä mitään
    if(sumeus == 1) {
        pensseli::moveTo(viiva.pisteet.back().sijainti);
        return;
    }    
    
    teeVeto(viiva.pisteet.back().sijainti, paksuus, sumeus);
}


void Monitori::piirraKokoViiva(const Viiva& viiva) {
    if(viiva.pisteet.empty() ) {
        return;
    }
    
    smooth paksuusSmooth;
    
    tyhjenna();
    piirraVari(viiva.vari);
    std::cout << "väri: " << (int)viiva.vari.r << ", " << (int)viiva.vari.g << ", " << (int)viiva.vari.b << "\n";
    
    std::vector<float> paksuudet = viiva.haeArvot(&viiva.paksuus);
    std::vector<float> sumeudet = viiva.haeArvot(&viiva.sumeus);
    
    if(paksuudet.size() != viiva.pisteet.size() || sumeudet.size() != viiva.pisteet.size() ) {
        std::cerr << "Monitori::piirraKokoViiva: vektorien koko ei täsmää\n";
        return;
    }
    
    for(unsigned int i=0; i<viiva.pisteet.size(); i++) {
        //paksuus on 0...1
        //pehmennys ottamalla 8 viimeistä arvoa
        paksuusSmooth.add(paksuudet[i]);
        float paksuus = paksuusSmooth.get();

        //sumeus on 0...1
        float sumeus = sumeudet[i];

        teeVeto(viiva.pisteet[i].sijainti, paksuus, sumeus);
    }
}


void Monitori::piirraViivaAlusta(const Viiva& viiva, unsigned int n) {
    if(viiva.pisteet.empty() ) {
        return;
    }
    
    if(n >= viiva.pisteet.size() ) {
        piirraKokoViiva(viiva);
        return;
    }
    
    smooth paksuusSmooth;
    
    tyhjenna();
    piirraVari(viiva.vari);
    std::cout << "väri: " << (int)viiva.vari.r << ", " << (int)viiva.vari.g << ", " << (int)viiva.vari.b << "\n";
    
    std::vector<float> sumeudet = viiva.haeArvot(&viiva.sumeus);
    std::vector<float> paksuudet = viiva.haeArvot(&viiva.paksuus);
    
    //todo: jos tämän siirtäisi Viivaan?
    if(paksuudet.size() != viiva.pisteet.size() || sumeudet.size() != viiva.pisteet.size()) {
        std::cerr << "Monitori::piirraKokoViiva: vektorien koko ei täsmää\n";
        return;
    }

    for(unsigned int i=0; i<n; i++) {
        
        float sumeus = sumeudet[i];

        //todo: pehmennys ottamalla 8 viimeistä arvoa
        paksuusSmooth.add(paksuudet[i]);
        float paksuus = paksuusSmooth.get();

        teeVeto( viiva.pisteet[i].sijainti, paksuus, sumeus);
    }
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


void Multimonitori::teeVeto(ofPoint kohde, unsigned int pensseli_i, float paksuus, float sumeus, ofColor vari) {
    if(pensseli_i >= pensselit.size() ) {
        std::cout << "Multimonitori::teeVeto: ei pensseliä " << pensseli_i << " (" << pensselit.size() << ")\n";
        return;
    }
    
    //jos sumeus on täysi, ei piirretä mitään
    if(sumeus == 1) {
        pensselit[pensseli_i].moveTo(kohde);
        return;
    }
    // koko: 0 ... MAX_KOKO/(4+2/3)
    pensselit[pensseli_i].koko = ofClamp(pow(paksuus, 0.7) * pensseli::MAX_KOKO / (4+2/3), 1, pensseli::MAX_KOKO / (4+2/3) );    
    // blur: 0...16
    pensselit[pensseli_i].blur = ofClamp(pow(sumeus, 2) * 16, 0.1, 16);
    
    pensselit[pensseli_i].vari = vari;
    
    viivaFbo.begin();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        pensselit[pensseli_i].strokeTo( kohde );
    viivaFbo.end();
}


void Multimonitori::setup(unsigned int pensseli_n) {

    //jos pensselien määrä on annettu, luodaan ne; muuten jätetään tyhjäksi ja luodaan tarvittaessa
    if(pensseli_n > 0) {
        pensselit.resize(pensseli_n);
        for(pensseli& p : pensselit) {
            p.setup();
        }
    }
    
    //alusta viivaFbo
    viivaFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    viivaFbo.begin();
        ofClear(pensseli::clearColor);
    viivaFbo.end();    
}


void Multimonitori::luoPensselit(unsigned int n) {
    pensselit.resize(n);
    for(pensseli& p : pensselit)
        p.setup();
    
    tyhjenna();
}


void Multimonitori::draw() {
    ofClear(taustaVari);    
    ofSetColor(ofColor::white);    
    viivaFbo.draw(0,0);
}


void Multimonitori::piirraViivatAlusta(const std::vector<Viiva>& viivat, unsigned int n) {
    if(viivat.empty() ) {
        return;
    }
        
    //luodaan pensselit jos niitä ei ole
    if(pensselit.size() < viivat.size() ) {
        luoPensselit(viivat.size() );
    }
    else
        tyhjenna();
    
    //haetaan paksuudet ja sumeudet valmiiksi:
    std::vector< std::vector<float> > paksuudet(viivat.size());
    std::vector< std::vector<float> > sumeudet(viivat.size());
    std::vector<smooth> paksuusSmooths(viivat.size() );
    
    for(unsigned int viiva_i = 0; viiva_i < viivat.size(); viiva_i++) {
        paksuudet[viiva_i] = viivat[viiva_i].haeArvot(&viivat[viiva_i].paksuus);
        sumeudet[viiva_i] = viivat[viiva_i].haeArvot(&viivat[viiva_i].sumeus);        

        if(paksuudet[viiva_i].size() != viivat[viiva_i].pisteet.size() || sumeudet[viiva_i].size() != viivat[viiva_i].pisteet.size()) {
            std::cerr << "Multimonitori::piirraViivatAlusta: vektorien koko ei täsmää\n";
            return;
        }
    }
    
    //piirretään n vetoa:
    for(unsigned int i=0; i<n; i++) {
        //piirretään joka viivasta yksi veto:
        for(unsigned int viiva_i = 0; viiva_i < viivat.size(); viiva_i++) {
            float sumeus = sumeudet[viiva_i][i];

            //pehmennys ottamalla 8 viimeistä arvoa
            paksuusSmooths[viiva_i].add(paksuudet[viiva_i][i]);
            float paksuus = paksuusSmooths[viiva_i].get();

            teeVeto( viivat[viiva_i].pisteet[i].sijainti, viiva_i, paksuus, sumeus, viivat[viiva_i].vari);
        }
    }
}


void Multimonitori::tallennaKuvana(std::string filename) {}


void Multimonitori::tyhjenna() {
    viivaFbo.begin();
        ofClear(pensseli::clearColor);
    viivaFbo.end();
    
    for(pensseli& p : pensselit)
        p.lopetaViiva();
}