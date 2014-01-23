#pragma once
#include "particle.h"

class Link {
public:
  Link(Particle * which1, Particle * which2, float restingDist, float stiff);
  ~Link(){}

  void draw();
  void constraintSolve();

  float restingDistance;
  float stiffness;
  float curtainTearSensitivity = 10050;

  Particle * p1;
  Particle * p2;

  // the scalars are how much "tug" the particles have on each other
  // this takes into account masses and stiffness, and are set in the Link constructor
  float scalarP1;
  float scalarP2;

  // if you want this link to be invisible, set this to false
  bool drawThis;

};
