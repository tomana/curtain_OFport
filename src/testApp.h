#pragma once

#include "ofMain.h"
#include "fabric.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

    /*
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    */

	int pMouseX;
    int pMouseY;
    bool pMousePressed;
    string pMouseString;

    Fabric fabric;
};
