#pragma once
/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - Behaviors don't complete in a cycle
// updated Fall, 2008 - turn linked lists into vectors
// updated Fall, 2010 - better comments to document how to use it
//
// Behaviors move objects around. Each object has a list of
// behaviors. The behaviors can change the internals of the objects.
// This is one of the classes that you will need to subclass for the project.
//
// periodically, every object is told to "simulate" - to do this, it
// calls all of its behaviors. the behavior is told how long to simulate
// for (or, to be more precise, what time to simulate up until)
// time is measured in milliseconds
//
// the "simulateUntil" is the key element of the class. it gets called every
// frame. it has the job of updating some object to make it move. 
// basically, what simulateUntil needs to do is to change the object from
// where it currently is to where it should be at the specified time. 
// inside the behavior, the last time the object was updated by the behavior is stored
// so you know how far the object should be moved.
//
// it is important to pay attention to the amount of time passed so that
// objects move at a consistent speed, independent of frame rate
//
// some simple examples are given in the "SimpleBehaviors.H" file
//
class GrObject;

// an attempt to decouple how we decide where something is with what
// the thing is
class Behavior {
public:
  Behavior(GrObject* owner);

  GrObject* owner;				// who to make go

  // what time did the last step end - after you do something, be sure to set
  // this to the time at the end of the step!
  unsigned long lastV;

  // make it go!
  // be sure to update lastV! (that's the job of simulateUntil)
  virtual void simulateUntil(unsigned long);
};



// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/Behavior.H,v 1.4 2010/11/18 15:31:30 gleicher Exp $






