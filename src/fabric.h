#pragma once

#include "ofMain.h"
#include "particle.h"
#include "link.h"

class Fabric
{
public:
    void setup();
    void draw(bool mousepressed, string pMouseString, int pMouseX, int pMouseY);
    void createCurtain ();
// Controls. The r key resets the curtain, g toggles gravity
    void keyPressed();
    void drawInstructions ();
    void toggleGravity();
    float distPointToSegmentSquared(float lineX1, float lineY1, float lineX2, float lineY2, float pointX, float pointY);

    vector<Particle *> particles;

// every particle within this many pixels will be influenced by the cursor
    float mouseInfluenceSize = 15;
// minimum distance for tearing when user is right clicking
    float mouseTearSize = 8;
    float mouseInfluenceScalar = 1;

// force of gravity is really 9.8, but because of scaling, we use 9.8 * 40 (392)
// (9.8 is too small for a 1 second timestep)
    float gravity = 392;

// Dimensions for our curtain. These are number of particles for each direction, not actual widths and heights
// the true width and height can be calculated by multiplying restingDistances by the curtain dimensions
    int curtainHeight = 60;
    int curtainWidth = 80;
    int yStart = 25; // where will the curtain start on the y axis?
    float restingDistances = 10;
    float restingDistances2 = 10;
    float stiffnesses = 1; // distance the particles have to go before ripping

// These variables are used to keep track of how much time is elapsed between each frame
// they're used in the physics to maintain a certain level of accuracy and consistency
// this program should run the at the same rate whether it's running at 30 FPS or 300,000 FPS
    long previousTime;
    long currentTime;
// Delta means change. It's actually a triangular symbol, to label variables in equations
// some programmers like to call it elapsedTime, or changeInTime. It's all a matter of preference
// To keep the simulation accurate, we use a fixed time step.
    int fixedDeltaTime = 15;
    float fixedDeltaTimeSeconds = (float)fixedDeltaTime / 1000.0;

// the leftOverDeltaTime carries over change in time that isn't accounted for over to the next frame
    int leftOverDeltaTime = 0;

// How many times are the constraints solved for per frame:
    int constraintAccuracy = 3;



};
