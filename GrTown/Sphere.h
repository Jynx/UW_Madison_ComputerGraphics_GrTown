#pragma once
#include <GL/glew.h>
#include <stdlib.h> 
#include "GrObject.H"
#include "Utilities/texture.h"
#include "Utilities/ShaderTools.H"



class Sphere : public GrObject
{
public:
	Sphere(float x, float y, float z, float scale, int passes, float r, float g, float b, int useText, int shaderType);
	~Sphere();

	double _r;
	double _g;
	double _b;
	int _useText;
	int _shaderType;
	int mPasses;
    float mScale;
	void drawTriangle(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]);
	void subDivide(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int passes);
	void normalizeVec(GLfloat v[3]);
	virtual void draw(DrawingState* drst);

};

