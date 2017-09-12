#pragma once
#include "ofMain.h"

enum Tila {Selaa, Tallenna};

struct Tilat {
    Tila tila;
    
    void update() {switch(tila){case Selaa:selaa();break;case Tallenna:tallenna();break;}}
    
    virtual void selaa() = 0;
    virtual void tallenna() = 0;
    
};
