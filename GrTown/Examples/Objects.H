#pragma once

/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - new texture manager, improvements for behaviors

//
// A set of relatively simple objects to get the ideas across
// these are mainly here as examples - real objects should be more
// interesting 

#include "../GrObject.H"
#include "../DrawingState.H"

// mainly for debugging
class Cube : public GrObject {
public:
  Cube(float x, float y, float z, float s, float r=1, float g=.2, float b=.2);
  Color color;
  float size;
  virtual void draw(DrawingState*);
};


class TestCubes : public GrObject {
public:
  TestCubes();
  virtual void draw(DrawingState*);
};

// this object uses a shader!
class ShadedCube : public GrObject {
public:
  ShadedCube(float x, float y, float z, float s, float r=1, float g=.2, float b=.2);
  Color color;
  float size;
  virtual void draw(DrawingState*);
};

class Triangle : public GrObject {
public:
	Triangle(float x, float y, float z, float s, float r, float g, float b);
	Color color;
	float size;
	virtual void draw(DrawingState*);
};

class BillboardTree : public GrObject {
public :
	BillboardTree(float x, float y, float z, int type);
	int _type;
	virtual void draw(DrawingState*);


};

class SimpleLogHouse1 : public GrObject {
public:
	SimpleLogHouse1(float x, float y, float z, float degrees, float rotX, float rotY, float rotZ);
	double _x, _y, _z;
	double _degrees, _rotX, _rotY, _rotZ;
	virtual void draw(DrawingState*);
};

class SimpleLogHouse2 : public GrObject {
public:
	SimpleLogHouse2(float x, float y, float z, float degrees, float rotX, float rotY, float rotZ);
	double _x, _y, _z;
	double _degrees, _rotX, _rotY, _rotZ;
	virtual void draw(DrawingState*);
};


// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/Examples/Objects.H,v 1.3 2008/11/11 04:18:40 gleicher Exp $


