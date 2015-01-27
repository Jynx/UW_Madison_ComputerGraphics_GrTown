#pragma once
#include <GL/glew.h>
#include <time.h>
#include <stdlib.h> 
#include "GrObject.H"
#include "DrawingState.H"
#include "Utilities/Texture.H"

#define MAX_PARTICLES 100000

class ParticleSystem : public GrObject
{
public:
	ParticleSystem();
	~ParticleSystem();

	void InitializeParticles(bool isSnow);
	void ActivateParticles(int index);
	void AdjustParticles(int index);
	void RenderParticles(DrawingState* dr);
	virtual void draw(DrawingState* drst);
	bool m_isSnow;

	struct PARTICLE {
		float _x, _y, _z; // Current position
		float _speedX, _speedY, _speedZ; // Current Speed/Movement
		float _targetSpeedX, _targetSpeedY, _targetSpeedZ; // Target Speed/Movement
		float _r, _g, _b; // Particle Colour
		bool _active; // Is particle Active
		float _radius;
	} mParticles[MAX_PARTICLES];
};
