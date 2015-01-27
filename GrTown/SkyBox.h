#pragma once
#include "GrObject.H"
#include "Utilities/texture.h"
#include "Utilities/ShaderTools.H"
class SkyBox :
	public GrObject
{
public:
	SkyBox();
	~SkyBox();
	void drawSkybox(double size);
	virtual void draw(DrawingState* drst);
};

