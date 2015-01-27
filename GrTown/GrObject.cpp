/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006
//
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "GrTown_PCH.H"
#include "Examples/Objects.H"
#include "GrObject.H"
#include "FlyCamera.H"
#include "Behavior.H"
#include "GrWorld.H"
#include "DrawingState.H"

using std::vector;
using namespace glm;
int noNameCtr=1;
GrObject::GrObject(char* nb, int& nn, char* xn) :
  ridable(0), interesting(0),
  parent(0)
{
	// since its easiest to just use C sprintf to build the name
	char buf[255];
	if (!nb)
		_snprintf(buf,255,"NoName-%d%c",nn++,0);
	else if (xn)
		_snprintf(buf,255,"%s-%s-%d%c",nb,xn,nn++,0);
	else
		_snprintf(buf,255,"%s-%d%c",nb,nn++,0);
	name = buf;

	idMatrix(transform);
	rideAbove = 0;
}
GrObject::~GrObject()
{
}

// null functions - not abstract since not all objects will override
// them
void GrObject::draw(DrawingState* drst)
{
}
void GrObject::drawAfter(DrawingState* drst)
{
}


// for most objects, the way to use it as a camera is to take its
// transformation and invert it - potentially using "rideabove"
void GrObject::getCamera(Matrix camera)
{
  copyMatrix(transform,camera);
  invertMatrix(camera);
  camera[3][1] -= rideAbove;
}

// the user interface for the object may or may not do anything
bool GrObject::uiStep()
{
  return false;
}
int GrObject::handle(int)
{
	return 0;
}


// note - we might not really get to the time in one shot
// so let it take up to 10 tries
void GrObject::simulateUntil(unsigned long t)
{
	for(vector<Behavior*>::iterator i = behaviors.begin(); i != behaviors.end(); ++i) {
	  Behavior *b = *i;
	  int ct=0;
	  while ((b->lastV < t) && (ct<10)) {
		b->simulateUntil(t);
		ct++;
	  }
	  /* if (ct>1) 
		  printf("needed %d simulation substeps\n",ct); */
	  // this is a common bug - you'll get caught here if you don't update lastV
	  if (ct>8)
		  printf("Warning! stuck behavior!\n");
  }
}

GrObject* GrObject::add(GrObject* g, float x, float y, float z, float ry)
{
	children.push_back(g);
	g->parent = this;
	Matrix t1,t2;
	rotMatrix(t1,'Y',ry);
	multMatrix(t1,g->transform,t2);
	copyMatrix(t2,g->transform);
	g->transform[3][0] += x;
	g->transform[3][1] += y;
	g->transform[3][2] += z;
	return g;
}


GrObject* add(GrObject* g, float x, float y, float z, float ry)
{
	theObjects.push_back(g);
	Matrix t1,t2;
	rotMatrix(t1,'Y',ry);
	multMatrix(t1,g->transform,t2);
	copyMatrix(t2,g->transform);
	g->transform[3][0] += x;
	g->transform[3][1] += y;
	g->transform[3][2] += z;
	return g;
}

// draw a list of objects
void drawObList(vector<GrObject*>& objs, DrawingState* drst)
{
  glPushMatrix();
  for(vector<GrObject*>::iterator i = objs.begin(); i != objs.end(); ++i) {
	  GrObject* g = (*i);
	  if (g != drst->camera) {
		  glPushMatrix();
		  glMultMatrixf(&g->transform[0][0]);
		  g->draw(drst);
		  glBindTexture(GL_TEXTURE_2D,0);
		  if (g->children.size()) {
			  drawObList(g->children,drst);
		  }
		  glPopMatrix();
	  }
  }
  glPopMatrix();
}
void drawAfterObList(vector<GrObject*>& objs, DrawingState* drst)
{
  glPushMatrix();
  for(std::vector<GrObject*>::iterator i = objs.begin(); i != objs.end(); ++i) {
	  GrObject* g = (*i);
	  if (g != drst->camera) {
		  glPushMatrix();
		  glMultMatrixf(&g->transform[0][0]);
		  g->drawAfter(drst);
		  glBindTexture(GL_TEXTURE_2D,0);
		  if (g->children.size()) {
			  drawAfterObList(g->children,drst);
		  }
		  glPopMatrix();
	  }
  }
  glPopMatrix();
}
// added by Steven Volocyk
void GrObject::realTimeBillBoardRender(DrawingState* dr) 
{
	Matrix cam;
	dr->camera->getCamera(cam);
    realTimeBillBoardRender(cam, dr);
}
// added by Steven Volocyk
void GrObject::realTimeBillBoardRender(Matrix& cam, DrawingState* dr) 
{
	vec3 look, upVector, rightVector, objectUpVector, pointPosition;

	//camera position
	float camX = cam[0][2];
	float camY = cam[1][2];
	float camZ = cam[2][2];

	//object point position.
	pointPosition.x = laX;
	pointPosition.y = laY;
	pointPosition.z = laZ;

	//look vector, camera position - point position.
	look.x = camX - pointPosition.x;
	look.y = 0;
	look.z = camZ - pointPosition.z;

	look = normalize(look);

	upVector = vec3(0.0, 1.0, 0.0); // default up vector for camera/world
	rightVector = cross(upVector, look);  // right vector, cross product of UpVector and Look Vector.

	GLfloat mat[16];
	mat[0] = rightVector.x;
	mat[1] = rightVector.y;
	mat[2] = rightVector.z;
	mat[3] = 0;
	mat[4] = upVector.x;
	mat[5] = upVector.y;
	mat[6] = upVector.z;
	mat[7] = 0;
	mat[8] = look.x;
	mat[9] = look.y;
	mat[10] = look.z;
	mat[11] = 0;
	mat[12] = lfX;
	mat[13] = lfY;
	mat[14] = lfZ;
	mat[15] = 1.0;

	glMultMatrixf(mat);
}




// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/GrObject.cpp,v 1.4 2008/11/14 19:53:30 gleicher Exp $
