#pragma once
#include "ofMain.h"
#include "Tilat.h"
#include "Arkisto.h"
#include "Monitori.h"

struct Ohjain:public Tilat, public Arkisto,public Multimonitori {
    
    float hue, range;
    
    Ohjain();
    
    void setup();
    void update();
    void keyPressed(int key);
    
    void debugDraw(int x, int y);
    
    void selaa();
    void tallenna();
    
};
