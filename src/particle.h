#pragma once

#include "ofMain.h"
//#include "link.h"

class Link;

class Particle
{
public:
    Particle(ofVec3f pos);

    void updatePhysics (float timeStep);
    void updateInteractions(bool mousePressed, string mouseString, int pmouseX, int pmouseY);
    void draw();
    void solveConstraints();
    void attachTo(Particle * O, Particle * P, float restingDist, float stiff);
    void removeLink(Link * lnk);
    void applyForce(ofVec3f f);
    void pinTo(ofVec3f location);

    float distPointToSegmentSquared (float lineX1, float lineY1, float lineX2, float lineY2, float pointX, float pointY);

    ofVec3f lastPosition;
    ofVec3f position;
    ofVec3f acceleration;

    float gravity = 392;
    float mass = 1;
    float damping = 100;

    float mouseInfluenceSize = 15;
// minimum distance for tearing when user is right clicking
    float mouseTearSize = 3;
    float mouseInfluenceScalar = 1;

    // An ArrayList for links, so we can have as many links as we want to this particle :)
    vector<Link *> links;

    bool pinned;
    ofVec3f pinLocation;
};

