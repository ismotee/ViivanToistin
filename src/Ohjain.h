#pragma once
#include "ofMain.h"
#include "Tilat.h"
#include "Arkisto.h"
#include "Monitori.h"

struct Ohjain:public Tilat, public Arkisto {
    
    float hue, hueRange,saturation,saturationRange,brightness,brightnessRange;
    int frame_n = 0;
    float muutosKerroin;
    bool shiftHold;
    
    Multimonitori selausMonitori, tallennusMonitori;
    
    Ohjain();
    void nextFrame();
    void reset(); //päivittää valikoiman huen ja rangen mukaan sekä aloittaa piirron alusta
    
    void setup();
    void update();
    void keyPressed(int key);
    void keyReleased(int key);
    void draw();
    void debugDraw(int x, int y);
    
    void selaa();
    void tallenna();
    
};
