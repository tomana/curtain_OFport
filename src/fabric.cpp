/*
  curtain_OFport, by Tom Andreas Nærland, tom.naerland@gmail.com 2014

  based on the original by Jared Count:
  Curtain (Fabric Simulator)
  Made by BlueThen on February 5th, 2011; updated February 10th and 11th, 2011 and July 18th and 19th, 2011
  To interact, left click and drag, right click to tear,
               press 'G' to toggle gravity, and press 'R' to reset
  www.bluethen.com
*/


#include "fabric.h"

void Fabric::setup()
{
    // I find that P2D is the fastest renderer for 2D graphics
    // OPENGL may be faster for some people
    // The default renderer is JAVA2D

    // we square the mouseInfluenceSize and mouseTearSize so we don't have to use squareRoot when comparing distances with this.
    mouseInfluenceSize *= mouseInfluenceSize;
    mouseTearSize *= mouseTearSize;

    // create the curtain
    createCurtain();
}

void Fabric::draw(bool pMousePressed, string pMouseString, int pMouseX, int pMouseY)
{
    /******** Physics ********/
    // time related stuff
    currentTime = ofGetElapsedTimeMillis();
    // deltaTimeMS: change in time in milliseconds since last frame
    long deltaTimeMS = currentTime - previousTime;
    previousTime = currentTime; // reset previousTime
    // timeStepAmt will be how many of our fixedDeltaTime's can fit in the physics for this frame.
    int timeStepAmt = (int)((float)(deltaTimeMS + leftOverDeltaTime) / (float)fixedDeltaTime);
    // Here we cap the timeStepAmt to prevent the iteration count from getting too high and exploding
    timeStepAmt = min(timeStepAmt, 5);
    leftOverDeltaTime += (int)deltaTimeMS - (timeStepAmt * fixedDeltaTime); // add to the leftOverDeltaTime.

    // If the mouse is pressing, it's influence will be spread out over every iteration in equal parts.
    // This keeps the program from exploding from user interaction if the timeStepAmt gets too high.
    mouseInfluenceScalar = 1.0 / timeStepAmt;

    // update physics
    for (int iteration = 1; iteration <= timeStepAmt; iteration++)
    {
        //cout << "fabric1" << endl;
        // solve the constraints multiple times
        // the more it's solved, the more accurate.
        for (int x = 0; x < constraintAccuracy; x++)
        {
            for (int i = 0; i < particles.size(); i++)
            {

                Particle * particle = particles.at(i);
                //cout << "particle position" << endl;
                //cout << particle->position << endl;
                particle->solveConstraints();
            }
        }
         //cout << "fabric2" << endl;
        // update each particle's position
        for (int i = 0; i < particles.size(); i++)
        {

            Particle * particle = particles.at(i);
            //cout << "before update" << endl;
            //cout << particle->position << endl;
            particle->updateInteractions(pMousePressed, pMouseString, pMouseX,pMouseY);
            particle->updatePhysics(fixedDeltaTimeSeconds);
            //cout << "after update" << endl;
            //cout << particle->position << endl;
        }

    }
    // draw each particle or its links
    //cout << "fabric3" << endl;
    for (int i = 0; i < particles.size(); i++)
    {
        Particle * particle = particles.at(i);
        ofSetColor(255,0,0,255);
        //ofRect(test.position.x, test.position.y, -2, 2, 2);
        particle->draw();
    }

    //cout << "fabric4" << endl;
}

void Fabric::createCurtain ()
{
    // We use an ArrayList instead of an array so we could add or remove particles at will.
    // not that it isn't possible using an array, it's just more convenient this way

    // midWidth: amount to translate the curtain along x-axis for it to be centered
    // (curtainWidth * restingDistances) = curtain's pixel width
    int midWidth = (int) (ofGetWidth()/2 - (curtainWidth * restingDistances)/2);
    // Since this our fabric is basically a grid of points, we have two loops
    for (int y = 0; y <= curtainHeight; y++)   // due to the way particles are attached, we need the y loop on the outside
    {
        for (int x = 0; x <= curtainWidth; x++)
        {
            Particle * particle = new Particle(ofVec3f(midWidth + x * restingDistances, y * restingDistances + yStart, -ofRandom(0,0)));

            // attach to
            // x - 1  and
            // y - 1
            // particle attachTo parameters: Particle particle, float restingDistance, float stiffness
            // try disabling the next 2 lines (the if statement and attachTo part) to create a hairy effect

            if (x != 0) {

                particle->attachTo(particle, particles.at(particles.size()-1), restingDistances, stiffnesses);
                //cout << "links vector outside, x" <<endl;
                //cout << particle->links.back()->p2->position << endl;
                }
            // the index for the particles are one dimensions,
            // so we convert x,y coordinates to 1 dimension using the formula y*width+x

            if (y != 0) {
                particle->attachTo(particle, (particles.at((y - 1) * (curtainWidth + 1) + x)), restingDistances, stiffnesses);
                //cout << "links vector outside, y" <<endl;
                }



                  // shearing, presumably. Attaching invisible links diagonally between points can give our cloth stiffness.
                  // the stiffer these are, the more our cloth acts like jello.
                  // these are unnecessary for me, so I keep them disabled.
                  /*
                  if ((x != 0) && (y != 0))
                    particle->attachTo(particle, (particles.at((y - 1) * (curtainWidth+1) + (x-1))), restingDistances * sqrt(2), 0.1);
                  if ((x != curtainWidth) && (y != 0))
                    particle->attachTo(particle, (particles.at((y - 1) * (curtainWidth+1) + (x+1))), restingDistances * sqrt(2), 1);
            */

            // we pin the very top particles to where they are
            if (y == 0)
                particle->pinTo(particle->position);

            // add to particle array
            particles.push_back(particle);
            //cout << particles.back()->position << endl;
            if (x != 0) {
            }
        }
    }

    /*
    for (int i = 0; i < particles.size(); i ++){
    if (particles.at(i)->links.size() > 0) cout << particles.at(i)->links.back()->p1->position << endl;
    if (particles.at(i)->links.size() > 0) cout << particles.at(i)->position << endl;
    }
    */
}

// Controls. The r key resets the curtain, g toggles gravity
/*
void Fabric::keyPressed()
{
    if ((key == 'r') || (key == 'R'))
        createCurtain();
    if ((key == 'g') || (key == 'G'))
        toggleGravity();
}
*/
void Fabric::toggleGravity()
{
    if (gravity != 0)
        gravity = 0;
    else
        gravity = 392;
}
// Credit to: http://www.codeguru.com/forum/showpost.php?p=1913101&postcount=16
