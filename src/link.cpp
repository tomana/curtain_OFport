#include "link.h"

// The Link class is used for handling constraints between particles.
Link::Link(Particle * which1, Particle * which2, float restingDist, float stiff)
{
    // if you want this link to be invisible, set this to false
    p1 = which1; // when you set one object to another, it's pretty much a reference.
    //cout << p1->position << endl;
    p2 = which2; // Anything that'll happen to p1 or p2 in here will happen to the paticles in our array
    //cout << p1->position << endl;
    restingDistance = restingDist;
    stiffness = stiff;
    drawThis = true;

    // although there are no differences in masses for the curtain,
    // this opens up possibilities in the future for if we were to have a fabric with particles of different weights
    float im1 = 1.0 / p1->mass; // inverse mass quantities
    float im2 = 1.0 / p2->mass;
    scalarP1 = (im1 / (im1 + im2)) * stiffness;
    scalarP2 = (im2 / (im1 + im2)) * stiffness;
}

void Link::constraintSolve()
{

    // calculate the distance between the two particles
    //cout << "link position" << endl;
    //cout << p1->position << endl;

    ofVec3f delta = p1->position - p2->position;
    float d = sqrt(delta.x * delta.x + delta.y * delta.y);
    float difference = (restingDistance - d) / d;

    // if the distance is more than curtainTearSensitivity, the cloth tears
    // it would probably be better if force was calculated, but this works
    if (d > curtainTearSensitivity)
        p1->removeLink(this);

    // P1.position += delta * scalarP1 * difference
    // P2.position -= delta * scalarP2 * difference

    p1->position += (delta * scalarP1 * difference);
    p2->position -= (delta * scalarP2 * difference);

}

void Link::draw ()
{
    if (drawThis)
        //ofRect(p1->position.x, p1->position.y, p1->position.z, 2, 2);
        glLineWidth(1);
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(p1->position.x, p1->position.y, 0.0);
        glVertex3f(p2->position.x, p2->position.y, 0);
        glEnd();
        //ofLine(p1->position.x, p1->position.y, p2->position.x, p2->position.y);
}
