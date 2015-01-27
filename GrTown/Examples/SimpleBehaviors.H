#pragma once
/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - new texture manager, improvements for behaviors
// updated Fall, 2010 - better comments to document how to use it

// simple example behaviors - something to learn from
// not that these all work by updating the transformations. the amount is
// based on how much time has gone by.

#include "../Behavior.H"

////////////////////////////////////////////////////////////////////////
// a simple behavior: spin around the center point
// velocity (roughly) is radians per timestep (millisecond)
//
// the blasts away the rotation part of the object's transform matrix,
// but doesn't effect the translation part
class Spin : public Behavior {
public:
  Spin(GrObject* owner, float vel = .005);
  
  float vel;					// we need to remember this

  virtual void simulateUntil(unsigned long);
};

////////////////////////////////////////////////////////////////////////
// these examples directly update the object's matrix
// - its a bit harder to do anything complicated this way
//
// always makes things go forward - you probably want to change its
// direction using another behavior
class ForwardAlways : public Behavior {
public:
  ForwardAlways(GrObject* owner, float vel = 10);
  float vel;
  virtual void simulateUntil(unsigned long);
};
class TurnAlways : public Behavior {
public:
  TurnAlways(GrObject* owner, float vel = .1);
  float vel;
  virtual void simulateUntil(unsigned long);
};

// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/Examples/SimpleBehaviors.H,v 1.3 2010/11/18 15:31:30 gleicher Exp $
