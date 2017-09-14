#include "Tilat.h"

Tilat::Tilat() : tila(Selaa) {
}


void Tilat::update() {
    switch (tila) {
            case Selaa:
                selaa();
            break;
            case Tallenna:
                tallenna();
            break;
    }
}

void Tilat::vaihdaTilaa() {
    if (tila == Selaa) tila = Tallenna;
    else tila = Selaa;
}

void Tilat::debugDraw(int x, int y) {
    ofSetColor(ofColor::white);
    if (tila == Selaa) ofDrawBitmapString("Tila: Selaa", x, y);
    else ofDrawBitmapString("Tila: Tallenna", x, y);
}

void Tilat::keyPressed(int key) {

    
}
