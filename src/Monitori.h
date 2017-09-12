#include "Viiva.h"

class smooth{
public:
    std::vector<float> values;
    float get();
    void add(float);
    unsigned int max_size = 6;
};


class pensseli{
public:
    const int MAX_KOKO = 300;
    
    bool viivaJatkuu = false;
    
    float koko = 10;
    float blur = 0.1;
    float spacing = 0.4; // suhteena pensselin koosta. huom: vaikuttaa sumennukseen olennaisesti!
    const ofColor clearColor = ofColor(100, 100, 100, 0);
    ofColor vari = ofColor::lightPink;

    ofPoint sijainti;
    
    ofFbo brushFbo; //tähän piirretään yksi pensselin piste
    
    void setup(); // luo fbo:n

    void drawBrush(); // päivittää brushFbo:n
    void strokeTo(ofPoint kohde);
    void moveTo(ofPoint kohde);
    void lopetaViiva();
};


class Monitori : public pensseli {
public:
    ofColor taustaVari = ofColor::grey;
    ofFbo viivaFbo;
    
    float viivanAlfa = 0;
    bool fadeIn = false;
    bool fadeOut = false;
    bool showing = false;
    
    void setup();
    void draw();
    
    void piirraViiva(const Viiva&);
    void piirraKokoViiva(const Viiva&);
    void piirraViivaAlusta(const Viiva&, unsigned int n);
    void piirraViivatAlusta(std::vector<const Viiva&>, unsigned int n);
    void piirraVari(ofColor vari_);
    void tyhjenna();
    
    void paljasta();
    void piilota();
    
    void tallennaKuvana(std::string filename = "kuvat/default.png");
    void tallennaKartta(const std::vector<Viiva>& viivat, std::string filename = "kuvat/kartta.png");
};