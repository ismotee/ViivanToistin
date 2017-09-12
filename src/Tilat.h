#pragma once
#include "ofMain.h"

enum Tila {Selaa, Tallenna};

struct Tilat {
    Tila tila;
    
    Tilat();
    
    void update();
    void keyPressed(int key);
    
    
    virtual void selaa() = 0;
    virtual void tallenna() = 0;
    
    void vaihdaTilaa();
    
    void debugDraw(int x, int y);
    
    
};
