// Author: Steven Volocyk. 

#include <GL/glew.h>
#include "ParticleSystem.h"

#define MAX_SNOW_DRAWING_DISTANCE 1000
#define MAX_RAIN_DRAWING_DISTANCE 1250

ParticleSystem::ParticleSystem() : m_isSnow(false)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::InitializeParticles(bool isSnow) {

	srand((int)time(NULL));
	m_isSnow = isSnow;

	for (int i = 0; i < MAX_PARTICLES; i++) {
		mParticles[i]._active = false;
		mParticles[i]._targetSpeedX = 0.0f;
		if (isSnow)
		{
			mParticles[i]._targetSpeedY = (rand() % 26) / -100.0f - 0.125f;
			mParticles[i]._radius = (rand() % 25) / 100.0f + 0.15f;
		}
		else
		{
			mParticles[i]._targetSpeedY = (rand() % 35) / -100.0f - 0.3f;
			mParticles[i]._radius = (rand() % 15) / 100.0f + 0.05f;
		}
		mParticles[i]._targetSpeedZ = 0.0f;
	}
}
/*
activate and birth particles at the origin with random speed values and a maximum age to die upon.
Also determines if we want the particle to bounce when it collides with the ground plane.
*/
void ParticleSystem::ActivateParticles(int index) {
	if (!mParticles[index]._active){
		mParticles[index]._x = (float)(rand() % 3000 - 500);
		mParticles[index]._y = (float)(rand() % 300 + 25.0f);
		mParticles[index]._z = (float)(rand() % 3000 - 500);
		mParticles[index]._speedX = (((float)((rand() % 100) + 1)) / 1000.0f) - 0.05f;
		mParticles[index]._speedY = (((float)((rand() % 100) + 50)) / 250.0f);
		mParticles[index]._speedZ = (((float)((rand() % 100) + 1)) / 1000.0f) - 0.05f;
		mParticles[index]._active = true;
	}
}
/*
define the rate at which we increase the speed(%) towards the target speed and adjust the particle accordingly
then, if bouncing, set the amount we're bouncing.
*/
void ParticleSystem::AdjustParticles(int index) {
	// increase speed 
	mParticles[index]._speedX += (mParticles[index]._targetSpeedX - mParticles[index]._speedX) / 2.0f;
	mParticles[index]._speedY += (mParticles[index]._targetSpeedY - mParticles[index]._speedY) / 2.0f;
	mParticles[index]._speedZ += (mParticles[index]._targetSpeedZ - mParticles[index]._speedZ) / 2.0f;
	// adjust particle position relative to new speed.

	mParticles[index]._x += mParticles[index]._speedX;
	mParticles[index]._y += mParticles[index]._speedY;
	mParticles[index]._z += mParticles[index]._speedZ;

	if (mParticles[index]._y < 0.0f){
		mParticles[index]._y = 0.0f;
		mParticles[index]._active = false;
	}
}

float thetaX[] = {
	cosf(2.0f * 3.1415926f),
	cosf(2.0f * 3.1415926f * 0.1f),
	cosf(2.0f * 3.1415926f * 0.2f),
	cosf(2.0f * 3.1415926f * 0.3f),
	cosf(2.0f * 3.1415926f * 0.4f),
	cosf(2.0f * 3.1415926f * 0.5f),
	cosf(2.0f * 3.1415926f * 0.6f),
	cosf(2.0f * 3.1415926f * 0.7f),
	cosf(2.0f * 3.1415926f * 0.8f),
	cosf(2.0f * 3.1415926f * 0.9f)
};

float thetaY[] = {
	sinf(2.0f * 3.1415926f),
	sinf(2.0f * 3.1415926f * 0.1f),
	sinf(2.0f * 3.1415926f * 0.2f),
	sinf(2.0f * 3.1415926f * 0.3f),
	sinf(2.0f * 3.1415926f * 0.4f),
	sinf(2.0f * 3.1415926f * 0.5f),
	sinf(2.0f * 3.1415926f * 0.6f),
	sinf(2.0f * 3.1415926f * 0.7f),
	sinf(2.0f * 3.1415926f * 0.8f),
	sinf(2.0f * 3.1415926f * 0.9f)
};

void ParticleSystem::RenderParticles(DrawingState* dr){
	double size = 2.0;
	double height = 2.0;

	Matrix cam;
	dr->camera->getCamera(cam);
	bool doSnow = dr->timeOfDay < 5 || dr->timeOfDay > 14;
	if ((doSnow && !m_isSnow) ||
		(!doSnow && m_isSnow))
	{
		InitializeParticles(doSnow);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		if (!mParticles[i]._active)
		{
			ActivateParticles(i);
		}
		AdjustParticles(i);

		float deltaX = dr->camera->lfX - mParticles[i]._x;
		float deltaY = dr->camera->lfY - mParticles[i]._y;
		float deltaZ = dr->camera->lfZ - mParticles[i]._z;
		float distance = sqrt((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ));
		if (distance > (doSnow ? MAX_SNOW_DRAWING_DISTANCE : MAX_RAIN_DRAWING_DISTANCE))
		{
			// Don't draw if we are past the max distance
			continue;
		}

		glPushMatrix();
		glTranslatef(mParticles[i]._x, mParticles[i]._y, mParticles[i]._z);
		realTimeBillBoardRender(cam, dr);
		if (doSnow)
		{
			glBegin(GL_TRIANGLE_FAN);
			for (int ii = 0; ii < 10; ii++)
			{
				float x = mParticles[i]._radius * thetaX[ii]; //calculate the x component
				float y = mParticles[i]._radius * thetaY[ii]; //calculate the y component

				glVertex2f(x, y); //output vertex
			}
			glEnd();
		}
		else
		{
			// rain baby rain
			glColor4f(0.388f, 0.55686f, 1.0f, 0.8f);
			glBegin(GL_TRIANGLE_FAN);
			for (int ii = 0; ii < 10; ii++)
			{
				float x = mParticles[i]._radius * thetaX[ii] * 0.5; //calculate the x component
				float y = mParticles[i]._radius * thetaY[ii] * 1.75; //calculate the y component, slightly elliptical

				glVertex2f(x, y); //output vertex
			}
			glEnd();

		}

		glPopMatrix();
	}
	glDisable(GL_BLEND);
}


void ParticleSystem::draw(DrawingState* dr) {
	RenderParticles(dr);
}