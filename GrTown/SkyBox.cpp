//Author: Steven Volocyk.
#include "SkyBox.h"


SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}

void SkyBox::drawSkybox(double size){

	//sides
	fetchTexture("miramar_rt.tga");
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
	glTexCoord2f(1, 0); glVertex3f(size, -size, -size);
	glTexCoord2f(1, 1); glVertex3f(size, size, -size);
	glTexCoord2f(0, 1); glVertex3f(-size, size, -size);
	glEnd();
	
	fetchTexture("miramar_ft.tga");
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
	glTexCoord2f(1, 0); glVertex3f(size, -size, size);
	glTexCoord2f(1, 1); glVertex3f(size, size, size);
	glTexCoord2f(0, 1); glVertex3f(size, size, -size);
	glEnd();
	
	fetchTexture("miramar_lf.tga");
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(size, -size, size);
	glTexCoord2f(1, 0); glVertex3f(-size, -size, size);
	glTexCoord2f(1, 1); glVertex3f(-size, size, size);
	glTexCoord2f(0, 1); glVertex3f(size, size, size);
	glEnd();
	
	fetchTexture("miramar_bk.tga");
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
	glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
	glTexCoord2f(1, 1); glVertex3f(-size, size, -size);
	glTexCoord2f(0, 1); glVertex3f(-size, size, size);
	glEnd();

	/* Top and Bottom */
	
	fetchTexture("miramar_up.tga");
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
	glTexCoord2f(1, 0); glVertex3f(size, size, -size);
	glTexCoord2f(1, 1); glVertex3f(size, size, size);
	glTexCoord2f(0, 1); glVertex3f(-size, size, size);
	glEnd();
	
	fetchTexture("miramar_dn.tga");
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex3f(size, -size, -size);
	glTexCoord2f(0, 1); glVertex3f(-size, -size, -size);
	glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
	glTexCoord2f(1, 0); glVertex3f(size, -size, size);
	glEnd();

}

void SkyBox::draw(DrawingState* drst) {
	drawSkybox(10000);
}