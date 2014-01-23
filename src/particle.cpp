// the Particle class.
#include "particle.h"
#include "link.h"

Particle::Particle(ofVec3f pos)
{
    pinLocation = ofVec3f(0,0,0);
    // Particle constructor
    position = pos;
    lastPosition = pos;
    acceleration = ofVec3f(0,0,0);
    gravity = 392;
    pinned = false;

    mouseInfluenceSize *= mouseInfluenceSize;
    mouseTearSize *= mouseTearSize;
}

// The update function is used to update the physics of the particle.
// motion is applied, and links are drawn here
void Particle::updatePhysics(float timeStep)   // timeStep should be in elapsed seconds (deltaTime)
{
    // gravity:
    // f(gravity) = m * g

    ofVec3f fg = ofVec3f(0, mass * gravity, 0 );
    this->applyForce(fg);

     //Verlet Integration, WAS using http://archive.gamedev.net/reference/programming/features/verlet/
     //  however, we're using the tradition Velocity Verlet integration, because our timestep is now constant.
    // velocity = position - lastPosition
    ofVec3f velocity = position - lastPosition;
    // apply damping: acceleration -= velocity * (damping/mass)
    acceleration -= velocity * (damping/mass);
    // newPosition = position + velocity + 0.5 * acceleration * deltaTime * deltaTime
    ofVec3f nextPos = position + velocity + (acceleration * 0.5) * (timeStep * timeStep);
    // reset variables
    lastPosition = position;
    position = nextPos;
    acceleration = ofVec3f(0,0,0);

}

void Particle::updateInteractions(bool mousePressed, string mouseString, int pmouseX, int pmouseY)
{
    // this is where our interaction comes in.
    if (mousePressed)
    {
        int mouseX = ofGetAppPtr()->mouseX;
        int mouseY = ofGetAppPtr()->mouseY;

        float distanceSquared = distPointToSegmentSquared(pmouseX,pmouseY,mouseX,mouseY,position.x,position.y);
        if (mouseString == "LEFT")
        {
            if (distanceSquared < mouseInfluenceSize)   // remember mouseInfluenceSize was squared in setup()
            {
                // To change the velocity of our particle, we subtract that change from the lastPosition.
                // When the physics gets integrated (see updatePhysics()), the change is calculated
                // Here, the velocity is set equal to the cursor's velocity
                lastPosition = position - ofVec3f((mouseX-pmouseX)*mouseInfluenceScalar, (mouseY-pmouseY)*mouseInfluenceScalar);
            }
        }
        else   // if the right mouse button is clicking, we tear the cloth by removing links
        {
            if (distanceSquared < mouseTearSize)
                links.clear();
        }
    }

}

void Particle::draw()
{
    // draw the links and points
       ofSetColor(0,0,0,255);
    ofRect(position.x-1, position.y-1, position.z, 2,2);

    if (links.size() > 0)
    {
        for (int i = 0; i < links.size(); i++)
        {
            Link * currentLink = (Link * ) links.at(i);
            currentLink->draw();
        }
    }
    else {

    }
}
/* Constraints */
void Particle::solveConstraints()
{
    /* Link Constraints */
    // Links make sure particles connected to this one is at a set distance away


    for (int i = 0; i < links.size(); i++)
    {
        Link * currentLink = (Link *) links.at(i);
        currentLink->constraintSolve();
    }

    /* Boundary Constraints */
    // These if statements keep the particles within the screen
    if (position.y < 1)
        position.y = 2 * (1) - position.y;
    if (position.y > ofGetHeight()-1)
        position.y = 2 * (ofGetHeight() - 1) - position.y;
    if (position.x > ofGetWidth()-1)
        position.x = 2 * (ofGetWidth() - 1) - position.x;
    if (position.x < 1)
        position.x = 2 * (1) - position.x;

    /* Other Constraints */
    // make sure the particle stays in its place if it's pinned
    if (pinned)
        position = pinLocation;

}

// attachTo can be used to create links between this particle and other particles
void Particle::attachTo(Particle * O, Particle * P, float restingDist, float stiff)
{
    Link * lnk = new Link(O, P, restingDist, stiff);
    //cout << "link position defined within particle class" <<endl;
    //cout << lnk->p2->position << endl;
    links.push_back(lnk);
    //cout << "links vector back within particle class" <<endl;
    //cout << links.back()->p2->position << endl;
}
void Particle::removeLink(Link * lnk)
{

std::vector<Link *>::iterator it;
it = find (links.begin(), links.end(), lnk);
if (it != links.end()) links.erase(it);
    //vector<class>::iterator it = find(links.begin(), links.end(), lnk);
    //
}

void Particle::applyForce (ofVec3f f)
{
    // acceleration = (1/mass) * force
    // or
    // acceleration = force / mass
    acceleration = f * (1.0/mass);
}

void Particle::pinTo (ofVec3f location)
{
    pinned = true;
    pinLocation = location;
}

float Particle::distPointToSegmentSquared (float lineX1, float lineY1, float lineX2, float lineY2, float pointX, float pointY)
{
    float vx = lineX1 - pointX;
    float vy = lineY1 - pointY;
    float ux = lineX2 - lineX1;
    float uy = lineY2 - lineY1;

    float len = ux*ux + uy*uy;
    float det = (-vx * ux) + (-vy * uy);
    if ((det < 0) || (det > len))
    {
        ux = lineX2 - pointX;
        uy = lineY2 - pointY;
        return min(vx*vx+vy*vy, ux*ux+uy*uy);
    }

    det = ux*vy - uy*vx;
    return (det*det) / len;
}
