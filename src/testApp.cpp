#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0,0,0);
	ofSetFrameRate(60);
	ofEnableSmoothing();

    pMouseX = 0;
    pMouseY = 0;

	fabric.setup();
    cout << "done setup" << endl;
}
void testApp::update(){
pMouseX = mouseX;
pMouseY = mouseY;
}
//--------------------------------------------------------------
void testApp::draw() {
ofBackground(255,255,255,255);
ofSetColor(0,0,0,255);
//ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
fabric.draw(pMousePressed, pMouseString, pMouseX, pMouseY);
}

void testApp::mouseDragged(int x, int y, int button)
{

}

void testApp::mousePressed(int x, int y, int button)
{
pMousePressed = true;
if  (button == 0){
pMouseString = "LEFT";
}
if (button == 2){
pMouseString = "RIGHT";
}
}

void testApp::keyPressed(int key) {
fabric.createCurtain();
}
void testApp::mouseReleased(int x, int y, int button)
{
pMousePressed = false;
}

void testApp::windowResized(int w, int h)
{

}
