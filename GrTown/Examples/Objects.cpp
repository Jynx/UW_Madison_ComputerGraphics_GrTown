/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - new texture manager, improvements for behaviors

#include "../GrTown_PCH.H"
#include "Objects.H"
#include "../DrawUtils.H"
#include "Utilities/Texture.H"
#include "../MMatrix.H"
#include <FL/fl_ask.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../math_3d.h"


#include "Utilities/ShaderTools.H"

using namespace glm;

// some very basic classes of objects...
// mainly for debuggingS
Cube::Cube(float x, float y, float z, float s, 
		   float r, float g, float b)
  : color(r,g,b), size(s)
{
  transMatrix(transform,x,y,z);
}
void Cube::draw(DrawingState*)
{
  glColor4fv(&color.r);
  cube(0,0,0,size);
}


/***********************************************************************/
TestCubes::TestCubes()
{
}

void TestCubes::draw(DrawingState*)
{
  glColor3f(1,0,0);
  cube( 20,0,0, 3);
  cube(-20,0,0, 3);
  cube(0, 20,0, 3);
  cube(0,-20,0, 3);
  cube(0,0, 20, 3);
  cube(0,0,-20, 3);

  glColor3f(1,1,0);
  cube(20,10,20,3);
  glColor3f(0,1,1);
  cube(-20,10,20,3);
  glColor3f(1,0,1);
  cube(-20,10,-20,3);
  glColor3f(1,1,1);
  cube(20,10,-20,3);
}

// some very basic classes of objects...
// mainly for debugging
ShadedCube::ShadedCube(float x, float y, float z, float s, 
					   float r, float g, float b)
  : color(r,g,b), size(s)
{
  transMatrix(transform,x,y,z);
}

// load the shader in the first time the cube draws (since that way we
// know we have a GL context)
// we only try to load it once
static unsigned int shadedCubeShader = 0;
static bool triedCubeShader=false;

void ShadedCube::draw(DrawingState*)
{
	if (!shadedCubeShader && !triedCubeShader) {
		triedCubeShader = true;
		char* error;
		if (!(shadedCubeShader=loadShader("ShadedCubeTest.vert","ShadedCubeTest.frag",error))) {
			std::string s = "Can't Load Cube Shader:";
			s += error;
			fl_alert(s.c_str());
		}
	}

	glUseProgram(shadedCubeShader);
	glColor4fv(&color.r);
	cube(0,0,0,size);
	glUseProgram(0);
}
//Written by Steven Volocyk
GLuint VBO;
Triangle::Triangle(float x, float y, float z, float s, float r, float g, float b)
	: color(r, g, b), size(s)
{
	transMatrix(transform, x, y, z);
}
//Written by Steven Volocyk
void Triangle::draw(DrawingState*) {
	glColor4fv(&color.r);
	char path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	std::string filePath = path;

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "error, '%s'\n", glewGetErrorString(res));
	}

	const float vertices[] = {
		0.0f, 1.0f, 0.0f, 2.0f, // position
		1.0f, -0.732f, 0.0f, 2.0f,
		-1.5f, -0.732f, 0.0f, 2.0f,
		1.0f, 0.0f, 0.0f, 1.0f,  // colors
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
	};


	//GLuint shader = LoadShaders( (filePath + "/Shaders/ShadedCubeTest.vert").c_str(), (filePath + "/Shaders/ShadedCubeTest.frag").c_str() );
	char* error;
	static unsigned int shader = loadShader("Shader1Test.vert", "Shader1Test.frag", error);
	

	
	//Initialize the vertex buffer.
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	glUseProgram(shader);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}
//Written by Steven Volocyk
BillboardTree::BillboardTree(float x, float y, float z, int type){
	transMatrix(transform, x, y, z); 
	_type = type;
}
//Written by Steven Volocyk
void BillboardTree::draw(DrawingState* dr) {

	_type = _type % 3;
	glPushMatrix();
	realTimeBillBoardRender(dr);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1);
	if (_type == 0) {
		fetchTexture("pine1tga.tga");
	}
	if (_type == 1) {
		fetchTexture("pine2tga.tga");
	}
	else {
		fetchTexture("pinestga.tga");
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double size = 25;
	double height = size * 2;

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3d(-size, 0, 0);
	glTexCoord2f(1, 0); glVertex3d(size, 0, 0);
	glTexCoord2f(1, 1); glVertex3d(size, height, 0);
	glTexCoord2f(0, 1); glVertex3d(-size, height, 0);

	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}
//Written by Steven Volocyk
SimpleLogHouse1::SimpleLogHouse1(float x, float y, float z, float degrees, float rotX, float rotY, float rotZ){
	_x = x;
	_y = y;
	_z = z;
	_degrees = degrees;
	_rotX = rotX;
	_rotY = rotY;
	_rotZ = rotZ;
}
//Written by Steven Volocyk
void SimpleLogHouse1::draw(DrawingState* dr) {
	glPushMatrix();

	glTranslated(_x, _y, _z);
	glRotated(_degrees, _rotX, _rotY, _rotZ);

	glPushMatrix();
	{
		double height = 5;
		double length = 65;
		double width = 2.5;
		glTranslated(-20.0, 1.5, -32.0);
		generateLogWall(length, width, height, false, true, 1);
	}
	glPopMatrix();
	//wall 2....
	glPushMatrix();
	{
		double height = 5;
		double length = 65;
		double width = 2.5;
		glTranslated(20.0, 1.5, -32.0);
		generateLogWall(length, width, height, false, true, 1);
	}
	glPopMatrix();

	// wall 3

	glPushMatrix();
	{
		double height = 5;
		double length = 47;
		double width = 2.5;
		glTranslated(-23.0, 1.5, -28.0);
		glRotated(90, 0.0, 1.0, 0.0);
		generateLogWall(length, width, height, true, true, 1);
	}
	glPopMatrix();
	// wall 4
	glPushMatrix();
	{
		double height = 5;
		double length = 47;
		double width = 2.5;
		glTranslated(-23.0, 1.5, 30.0);
		glRotated(90, 0.0, 1.0, 0.0);
		generateLogWall(length, width, height, true, true, 1);
	}
	glPopMatrix();

	//roof side 1
	glPushMatrix();
	{
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("snowroofhires.jpg");

		glTranslated(-13.0, 36.0, -5.0);
		glRotated(125, 0, 0, 1);

		glBegin(GL_QUADS);
		{
			drawRectangle(2, 16, 41);
		}
		glEnd();
	}
	glPopMatrix();

	//roof side 2
	glPushMatrix();
	{
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("snowroofhires.jpg");

		glTranslated(13.0, 36.0, -5.0);
		glRotated(235, 0, 0, 1.0);

		glBegin(GL_QUADS);
		{
			drawRectangle(2, 16, 41);
		}
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	{
		//side log
		glTranslated(0.0, 0.5, 0.0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("mediumBark.jpg", false, false);
		glTranslated(15.0, 1.0, -45.0);
		drawCylinder(1.5, 1.5, 20, 5, 1);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("stump.jpg", false, false);
		drawDisk(0.0, 1.5, 5, 1);

		//middle log
		glRotated(-90, 0, 1, 0);
		glTranslated(2.0, 0.0, -2.0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("mediumBark.jpg", false, false);
		drawCylinder(1.5, 1.5, 33, 5, 1);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("stump.jpg", false, false);
		drawDisk(0.0, 1.5, 5, 1);
		glTranslated(0.0, 0.0, 33.0);
		drawDisk(0.0, 1.5, 5, 1);

		//other side log
		glRotated(90, 0.0, 1.0, 0.0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("mediumBark.jpg", false, false);
		glTranslated(2.0, 0.0, -2.0);
		drawCylinder(1.5, 1.5, 20, 5, 1);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("stump.jpg", false, false);
		drawDisk(0.0, 1.5, 5, 1);

		//deck itself
		glRotated(90, 0.0, 1.0, 0);
		glTranslated(-8.0, 2.0, 14.5);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("deck1.jpg", false, false);
		glBegin(GL_QUADS);
		{
			drawRectangle(15.0, .5, 13.0);
		}
		glEnd();

		// vertical beam
		glRotated(90, 1.0, 0.0, 0.0);
		glTranslated(6.0, 14.5, -31.0);
		//	glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("mediumBark.jpg", false, false);
		drawCylinder(1.5, 1.5, 36, 5, 1);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("stump.jpg", false, false);
		drawDisk(0.0, 1.5, 5, 1);

		//other vertical deck beam
		glTranslated(0.0, -29.0, 0.0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("mediumBark.jpg", false, false);
		drawCylinder(1.5, 1.5, 36, 5, 1);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("stump.jpg", false, false);
		drawDisk(0.0, 1.5, 5, 1);

		//roof beam over deck 1
		glRotated(270, 0.0, 1.0, 0.0);
		glTranslated(2.0, 0.0, -5.0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("mediumBark.jpg", false, false);
		drawCylinder(1.5, 1.5, 20, 5, 1);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("stump.jpg", false, false);
		drawDisk(0.0, 1.5, 5, 1);

		//roof beam over deck 2

		glTranslated(0.0, 29.0, 0.0);
		//	glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("mediumBark.jpg", false, false);
		drawCylinder(1.5, 1.5, 20, 5, 5);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("stump.jpg", false, false);
		drawDisk(0.0, 1.5, 5, 1);

		//roof beam on the diaganol.
		glTranslated(-4.5, -7.0, 0.0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("mediumBark.jpg", false, false);
		drawCylinder(1.5, 1.5, 20, 5, 1);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("stump.jpg", false, false);
		drawDisk(0.0, 1.5, 5, 1);

		//roof beam on the diaganol otherside.
		glTranslated(0.0, -16.5, 0.0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("mediumBark.jpg", false, false);
		drawCylinder(1.5, 1.5, 20, 5, 1);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("stump.jpg", false, false);
		drawDisk(0.0, 1.5, 5, 1);

		//roof beam center
		glTranslated(-5.5, 8.5, 0.0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("mediumBark.jpg", false, false);
		drawCylinder(1.5, 1.5, 20, 5, 1);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("stump.jpg", false, false);
		drawDisk(0.0, 1.5, 5, 1);

	}
	glPopMatrix();

	//chimney
	glPushMatrix();
	{
		glRotated(90, 1.0, 0.0, 0.0);
		glTranslated(0.0, 33.0, -8.0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("brick004.png", false, false);
		glBegin(GL_QUADS);
		{
			drawRectangle(15.0, 2.5, 8.0);
		}
		glEnd();
		glTranslated(0.0, -3.0, -25.0);
		glBegin(GL_QUADS);
		{
			drawRectangle(10.0, 5.0, 18.0);
		}
		glEnd();
		glTranslated(0.0, 0.0, -19.5);
		fetchTexture("largebrick.png", false, false);
		glBegin(GL_QUADS);
		{
			drawRectangle(10.0, 6.0, 1.5);
		}
		glEnd();
	}
	glPopMatrix();

	//windows and door
	glPushMatrix();
	{

		glRotated(-90, 0.0, 1.0, 0.0);
		glTranslated(16.0, 12.0, -12.5);

		char* error;
		static unsigned int glassShader = loadShader("glass.vert", "glass.frag", error);
		int day;
		if ((dr->timeOfDay < 5) || (dr->timeOfDay > 18)){
			day = 0;
		}
		else {
			day = 1;
		}
		GLint loc = glGetUniformLocation(glassShader, "day");
		glProgramUniform1i(glassShader, loc, day);

		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1);
		fetchTexture("windowFrame.tga");
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_POLYGON);

		//pane	
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-7, 0, -10);
		glTexCoord2f(0, 1); glVertex3i(-7, 16, -10);
		glTexCoord2f(1, 1); glVertex3i(7, 16, -10);
		glTexCoord2f(1, 0); glVertex3i(7, 0, -10);
		glEnd();


		//glass
		glUseProgram(glassShader);
		glTranslated(0.0, 0.0, 0.1);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-6, 0, -10);
		glTexCoord2f(0, 1); glVertex3i(-6, 16, -10);
		glTexCoord2f(1, 1); glVertex3i(6, 16, -10);
		glTexCoord2f(1, 0); glVertex3i(6, 0, -10);
		glEnd();
		glTranslated(0.0, 0.0, -0.1);
		glUseProgram(0);

		//pane
		glTranslated(-30.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-7, 0, -10);
		glTexCoord2f(0, 1); glVertex3i(-7, 16, -10);
		glTexCoord2f(1, 1); glVertex3i(7, 16, -10);
		glTexCoord2f(1, 0); glVertex3i(7, 0, -10);
		glEnd();

		//glass
		glUseProgram(glassShader);
		glTranslated(0.0, 0.0, 0.1);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-6, 0, -10);
		glTexCoord2f(0, 1); glVertex3i(-6, 16, -10);
		glTexCoord2f(1, 1); glVertex3i(6, 16, -10);
		glTexCoord2f(1, 0); glVertex3i(6, 0, -10);
		glEnd();
		glTranslated(0.0, 0.0, -0.1);
		glUseProgram(0);

		//pane
		glTranslated(0.0, 0.0, 45.0);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-7, 0, -10);
		glTexCoord2f(0, 1); glVertex3i(-7, 16, -10);
		glTexCoord2f(1, 1); glVertex3i(7, 16, -10);
		glTexCoord2f(1, 0); glVertex3i(7, 0, -10);
		glEnd();

		//glass
		glUseProgram(glassShader);
		glTranslated(0.0, 0.0, -0.1);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-6, 0, -10);
		glTexCoord2f(0, 1); glVertex3i(-6, 16, -10);
		glTexCoord2f(1, 1); glVertex3i(6, 16, -10);
		glTexCoord2f(1, 0); glVertex3i(6, 0, -10);
		glEnd();
		glTranslated(0.0, 0.0, 0.1);
		glUseProgram(0);

		//pane
		glTranslated(30.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-7, 0, -10);
		glTexCoord2f(0, 1); glVertex3i(-7, 16, -10);
		glTexCoord2f(1, 1); glVertex3i(7, 16, -10);
		glTexCoord2f(1, 0); glVertex3i(7, 0, -10);
		glEnd();

		//glass
		glUseProgram(glassShader);
		glTranslated(0.0, 0.0, -0.1);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-6, 0, -10);
		glTexCoord2f(0, 1); glVertex3i(-6, 16, -10);
		glTexCoord2f(1, 1); glVertex3i(6, 16, -10);
		glTexCoord2f(1, 0); glVertex3i(6, 0, -10);
		glEnd();
		glTranslated(0.0, 0.0, 0.1);
		glUseProgram(0);

		//front window
		glTranslated(-56.5, 4.0, -42.0);
		glRotated(-90, 0.0, 1.0, 0.0);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-5, 0, -10);
		glTexCoord2f(0, 1); glVertex3i(-5, 12, -10);
		glTexCoord2f(1, 1); glVertex3i(5, 12, -10);
		glTexCoord2f(1, 0); glVertex3i(5, 0, -10);
		glEnd();

		//glass
		glUseProgram(glassShader);
		glTranslated(0.0, 0.0, -0.1);

		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-4, 0, -10);
		glTexCoord2f(0, 1); glVertex3i(-4, 12, -10);
		glTexCoord2f(1, 1); glVertex3i(4, 12, -10);
		glTexCoord2f(1, 0); glVertex3i(4, 0, -10);
		glEnd();
		glTranslated(0.0, 0.0, 0.1);
		glUseProgram(0);

		//door
		fetchTexture("door.jpg");
		glTranslated(16.0, -12.0, 0.0);
		glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-5, 0, -10);
		glTexCoord2f(0, 1); glVertex3i(-5, 17, -10);
		glTexCoord2f(1, 1); glVertex3i(5, 17, -10);
		glTexCoord2f(1, 0); glVertex3i(5, 0, -10);
		glEnd();

	}

	glPopMatrix();
	glPopMatrix();
}
//Written by Steven Volocyk
SimpleLogHouse2::SimpleLogHouse2(float x, float y, float z, float degrees, float rotX, float rotY, float rotZ)
{
	_x = x;
	_y = y;
	_z = z;
	_degrees = degrees;
	_rotX = rotX;
	_rotY = rotY;
	_rotZ = rotZ;
}
//Written by Steven Volocyk
void SimpleLogHouse2::draw(DrawingState* dr)
{
	glPushMatrix();

	glTranslated(_x, _y, _z);
	glRotated(_degrees, _rotX, _rotY, _rotZ);

	glPushMatrix();
	{

		double height = 5;
		double length = 65;
		double width = 2.5;
		glTranslated(-20.0, 1.5, -32.0);
		generateLogWall(length, width, height, false, true, 2);
	}
	glPopMatrix();

	//wall2
	glPushMatrix();
	{

		double height = 5;
		double length = 65;
		double width = 2.5;
		glTranslated(20.0, 1.5, -32.0);
		generateLogWall(length, width, height, false, true, 2);
	}
	glPopMatrix();


	glPushMatrix();
	{
		double height = 5;
		double length = 47;
		double width = 2.5;
		glTranslated(-23.0, 1.5, -28.0);
		glRotated(90, 0.0, 1.0, 0.0);
		generateLogWall(length, width, height, true, true, 2);
	}
	glPopMatrix();
	// wall 4
	glPushMatrix();
	{
		double height = 5;
		double length = 47;
		double width = 2.5;
		glTranslated(-23.0, 1.5, 30.0);
		glRotated(90, 0.0, 1.0, 0.0);
		generateLogWall(length, width, height, true, true, 2);
	}
	glPopMatrix();

	glPushMatrix();
	{
		fetchTexture("thatcher.jpg");
		glTranslated(-13.0, 36.0, 0.0);
		glRotated(125, 0, 0, 1);

		glBegin(GL_QUADS);
		{
			drawRectangle(2, 16, 35);
		}
		glEnd();
	}
	glPopMatrix();


	//roof side 2
	glPushMatrix();
	{
		fetchTexture("thatcher.jpg");
		glTranslated(13.0, 36.0, 0.0);
		glRotated(235, 0, 0, 1.0);

		glBegin(GL_QUADS);
		{
			drawRectangle(2, 16, 35);
		}
		glEnd();
	}
	glPopMatrix();
	//roof part 3
	glPushMatrix();
	{
		fetchTexture("thatcher.jpg");
		glTranslated(35.0, 25.0, 0.0);
		glRotated(70, 0, 0, 1.0);

		glBegin(GL_QUADS);
		{
			drawRectangle(2, 16, 35);
		}
		glEnd();
	}
	glPopMatrix();
	//deck
	glPushMatrix();
	{
		glTranslated(33.0, 1.0, 0.0);
		glRotated(90, 0, 0, 1);
		fetchTexture("deck2.jpg", false, false);
		glBegin(GL_QUADS);
		{
			drawRectangle(3, 17, 32);
		}
		glEnd();
	}
	//deck pieces
	glPopMatrix();
	glTranslated(33.0, 17.0, 8.0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 73, 5, 1);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("stump.jpg", false, false);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, 73.0);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, -73.0);

	glPushMatrix();
	//hanging roof short
	glTranslated(16.0, -7.0, 22.0);
	glRotated(-20, 0.0, 0.0, 1.0);
	glRotated(90, 0.0, 1.0, 0.0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 30, 5, 1);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("stump.jpg", false, false);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, 30.0);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, -30.0);
	glTranslated(-15.0, 1.0, 45.0);


	glTranslated(-13.0, -2.0, 0.0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 30, 5, 1);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("stump.jpg", false, false);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, 30.0);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, -30.0);
	glTranslated(-15.0, 1.0, 45.0);

	glTranslated(-11.0, -2.0, 0.0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 30, 5, 1);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("stump.jpg", false, false);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, 30.0);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, -30.0);
	glTranslated(-15.0, 1.0, 45.0);

	glTranslated(-11.0, -2.0, 0.0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 30, 5, 1);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("stump.jpg", false, false);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, 30.0);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, -30.0);
	glTranslated(-15.0, 1.0, 45.0);

	glTranslated(-11.0, -2.0, 0.0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 30, 5, 1);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("stump.jpg", false, false);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, 30.0);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, -30.0);
	glTranslated(-15.0, 1.0, 45.0);

	glTranslated(-11.0, -2.0, 0.0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 30, 5, 1);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("stump.jpg", false, false);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, 30.0);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, -30.0);
	glTranslated(-15.0, 1.0, 45.0);

	glPopMatrix();

	glTranslated(-15.0, -45.0, 6.0);
	glRotated(90, 1.0, 0.0, 0.0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 17, 5, 1);

	fetchTexture("stump.jpg", false, false);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, 15.0);
	drawDisk(0.0, 1.5, 5, 1);
	glTranslated(0.0, 0.0, -15.0);
	glTranslated(-15.0, 1.0, 45.0);

	glTranslated(0.0, 11.0, 0.0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 17, 5, 1);
	glTranslated(-15.0, 1.0, 45.0);

	glTranslated(0.0, 31.0, 0.0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 17, 5, 1);
	glTranslated(-15.0, 1.0, 45.0);

	glTranslated(0.0, 12.0, 0.0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 17, 5, 1);
	glTranslated(-15.0, 1.0, 45.0);

	//log railings.
	glPushMatrix();
	{
		double length = 5.0;
		double width = .50;
		double height = 2.0;
		glTranslated(15.0, 0.0, -30.0);
		glRotated(90, 1.0, 0.0, 0.0);

		generateLogFence(length, width, height, 2);
	}
	glPopMatrix();

	glPushMatrix();
	{
		double length = 5.0;
		double width = .50;
		double height = 2.0;
		glTranslated(15.0, -47.0, -30.0);
		glRotated(90, 1.0, 0.0, 0.0);

		generateLogFence(length, width, height, 2);
	}
	glPopMatrix();

	glPushMatrix();
	{
		double length = 5.0;
		double width = .50;
		double height = 2.0;
		glTranslated(0.0, -61.0, -30.0);
		glRotated(90.0, 0.0, 0.0, 1.0);
		glRotated(90, 1.0, 0.0, 0.0);

		generateLogFence(length, width, height, 2);
	}
	glPopMatrix();

	glPushMatrix();
	{
		double length = 5.0;
		double width = .50;
		double height = 2.0;
		glTranslated(-8.0, -61.0, -30.0);
		glRotated(90.0, 0.0, 0.0, 1.0);
		glRotated(90, 1.0, 0.0, 0.0);

		generateLogFence(length, width, height, 2);
	}
	glPopMatrix();


	glPushMatrix();
	{
		double length = 5.0;
		double width = .50;
		double height = 2.0;
		glTranslated(-9.0, -1.0, -30.0);
		glRotated(90.0, 0.0, 0.0, 1.0);
		glRotated(90, 1.0, 0.0, 0.0);

		generateLogFence(length, width, height, 2);
	}
	glPopMatrix();

	glPushMatrix();
	{
		double length = 5.0;
		double width = .50;
		double height = 2.0;
		glTranslated(0.0, -1.0, -30.0);
		glRotated(90.0, 0.0, 0.0, 1.0);
		glRotated(90, 1.0, 0.0, 0.0);

		generateLogFence(length, width, height, 2);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, -27.0, 0.0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 18, 5, 1);
	glTranslated(-15.0, 1.0, 45.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, -38.0, 0.0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	fetchTexture("bark2.jpg", false, false);
	glTranslated(15.0, 1.0, -45.0);
	drawCylinder(1.5, 1.5, 18, 5, 1);
	glTranslated(-15.0, 1.0, 45.0);
	glPopMatrix();

	glPushMatrix();
	{
		double length = 5.0;
		double width = .50;
		double height = 2.0;
		glTranslated(15.0, -10.0, -30.0);
		glRotated(90, 1.0, 0.0, 0.0);

		generateLogFence(length, width, height, 2);
	}
	glPopMatrix();

	glPushMatrix();
	{
		double length = 5.0;
		double width = .50;
		double height = 2.0;
		glTranslated(15.0, -37.0, -30.0);
		glRotated(90, 1.0, 0.0, 0.0);

		generateLogFence(length, width, height, 2);
	}
	glPopMatrix();

	//chimney
	glPushMatrix();
	{
		//glRotated(90, 1.0, 0.0, 0.0);
		glTranslated(-34.0, 0.0, -34.0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		fetchTexture("rock055.png", false, false);
		glBegin(GL_QUADS);
		{
			drawRectangle(15.0, 2.5, 8.0);
		}
		glEnd();
		glTranslated(0.0, -3.0, -25.0);
		glBegin(GL_QUADS);
		{
			drawRectangle(10.0, 5.0, 18.0);
		}
		glEnd();
		glTranslated(0.0, 0.0, -19.5);
		fetchTexture("largebrick.png", false, false);
		glBegin(GL_QUADS);
		{
			drawRectangle(10.0, 6.0, 1.5);
		}
		glEnd();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(-33.0, -52.0, -40.0);
	glRotated(270, 1.0, 0.0, 0.0);
	char* error;
	static unsigned int glassShader = loadShader("glass.vert", "glass.frag", error);
	int day;
	if ((dr->timeOfDay < 5) || (dr->timeOfDay > 18)){
		day = 0;
	}
	else {
		day = 1;
	}
	GLint loc = glGetUniformLocation(glassShader, "day");
	glProgramUniform1i(glassShader, loc, day);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1);
	fetchTexture("windowFrame.tga");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_POLYGON);

	//pane	
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3i(-7, 0, -10);
	glTexCoord2f(0, 1); glVertex3i(-7, 16, -10);
	glTexCoord2f(1, 1); glVertex3i(7, 16, -10);
	glTexCoord2f(1, 0); glVertex3i(7, 0, -10);
	glEnd();


	//glass
	glUseProgram(glassShader);
	glTranslated(0.0, 0.0, 0.1);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3i(-6, 0, -10);
	glTexCoord2f(0, 1); glVertex3i(-6, 16, -10);
	glTexCoord2f(1, 1); glVertex3i(6, 16, -10);
	glTexCoord2f(1, 0); glVertex3i(6, 0, -10);
	glEnd();
	glTranslated(0.0, 0.0, -0.1);
	glUseProgram(0);


	glTranslated(-13.0, -4.0, 7.0);
	glRotated(90, 0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	//pane	
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3i(-7, 0, -10);
	glTexCoord2f(0, 1); glVertex3i(-7, 16, -10);
	glTexCoord2f(1, 1); glVertex3i(7, 16, -10);
	glTexCoord2f(1, 0); glVertex3i(7, 0, -10);
	glEnd();


	//glass
	glUseProgram(glassShader);
	glTranslated(0.0, 0.0, 0.1);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3i(-6, 0, -10);
	glTexCoord2f(0, 1); glVertex3i(-6, 16, -10);
	glTexCoord2f(1, 1); glVertex3i(6, 16, -10);
	glTexCoord2f(1, 0); glVertex3i(6, 0, -10);
	glEnd();
	glTranslated(0.0, 0.0, -0.1);
	glUseProgram(0);

	glTranslated(-29.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	//pane	
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3i(-7, 0, -10);
	glTexCoord2f(0, 1); glVertex3i(-7, 16, -10);
	glTexCoord2f(1, 1); glVertex3i(7, 16, -10);
	glTexCoord2f(1, 0); glVertex3i(7, 0, -10);
	glEnd();


	//glass
	glUseProgram(glassShader);
	glTranslated(0.0, 0.0, 0.1);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3i(-6, 0, -10);
	glTexCoord2f(0, 1); glVertex3i(-6, 16, -10);
	glTexCoord2f(1, 1); glVertex3i(6, 16, -10);
	glTexCoord2f(1, 0); glVertex3i(6, 0, -10);
	glEnd();
	glTranslated(0.0, 0.0, -0.1);
	glUseProgram(0);

	glTranslated(-3.0, 0.0, 26.0);
	glRotated(180, 0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	//pane	
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3i(-7, 0, -10);
	glTexCoord2f(0, 1); glVertex3i(-7, 16, -10);
	glTexCoord2f(1, 1); glVertex3i(7, 16, -10);
	glTexCoord2f(1, 0); glVertex3i(7, 0, -10);
	glEnd();


	//glass
	glUseProgram(glassShader);
	glTranslated(0.0, 0.0, 0.1);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3i(-6, 0, -10);
	glTexCoord2f(0, 1); glVertex3i(-6, 16, -10);
	glTexCoord2f(1, 1); glVertex3i(6, 16, -10);
	glTexCoord2f(1, 0); glVertex3i(6, 0, -10);
	glEnd();
	glTranslated(0.0, 0.0, -0.1);
	glUseProgram(0);

	glTranslated(-34.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	//pane	
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3i(-7, 0, -10);
	glTexCoord2f(0, 1); glVertex3i(-7, 16, -10);
	glTexCoord2f(1, 1); glVertex3i(7, 16, -10);
	glTexCoord2f(1, 0); glVertex3i(7, 0, -10);
	glEnd();


	//glass
	glUseProgram(glassShader);
	glTranslated(0.0, 0.0, 0.1);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3i(-6, 0, -10);
	glTexCoord2f(0, 1); glVertex3i(-6, 16, -10);
	glTexCoord2f(1, 1); glVertex3i(6, 16, -10);
	glTexCoord2f(1, 0); glVertex3i(6, 0, -10);
	glEnd();
	glTranslated(0.0, 0.0, -0.1);
	glUseProgram(0);

	//door
	fetchTexture("door.jpg");
	glTranslated(16.0, -7.0, 0.0);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0); glVertex3i(-5, 0, -10);
	glTexCoord2f(0, 1); glVertex3i(-5, 17, -10);
	glTexCoord2f(1, 1); glVertex3i(5, 17, -10);
	glTexCoord2f(1, 0); glVertex3i(5, 0, -10);
	glEnd();


	glPopMatrix();
	glPopMatrix();


}

// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/Examples/Objects.cpp,v 1.6 2009/11/10 22:40:03 gleicher Exp $
