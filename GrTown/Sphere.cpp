//Author Steven Volocyk with help from the openGl redbook.
#include "Sphere.h"
#include "DrawingState.H"


#define X .525731112119133606f
#define Z .850650808352039932f



static GLfloat vdata[12][3] = {
		{ -X, 0.0f, Z }, { X, 0.0f, Z }, { -X, 0.0f, -Z }, { X, 0.0f, -Z },
		{ 0.0f, Z, X }, { 0.0f, Z, -X }, { 0.0f, -Z, X }, { 0.0f, -Z, -X },
		{ Z, X, 0.0f }, { -Z, X, 0.0f }, { Z, -X, 0.0f }, { -Z, -X, 0.0f }
};

static int tindices[20][3] = {
		{ 0, 4, 1 }, { 0, 9, 4 }, { 9, 5, 4 }, { 4, 5, 8 }, { 4, 8, 1 },
		{ 8, 10, 1 }, { 8, 3, 10 }, { 5, 3, 8 }, { 5, 2, 3 }, { 2, 7, 3 },
		{ 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 }, { 11, 0, 6 }, { 0, 1, 6 },
		{ 6, 1, 10 }, { 9, 0, 11 }, { 9, 11, 2 }, { 9, 2, 5 }, { 7, 2, 11 } };

Sphere::Sphere(float x, float y, float z, float scale, int passes, float r, float b, float g, int useText, int shaderType)
{
	transMatrix(transform, x, y, z);
	mScale = scale;
	mPasses = passes;
	_r = r;
	_b = b;
	_g = g;
	_useText = useText;
	_shaderType = shaderType;
}


Sphere::~Sphere()
{
}

void Sphere::normalizeVec(GLfloat v[3]) {
	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= d; v[1] /= d; v[2] /= d;
}


void Sphere::drawTriangle(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]){
	if (_useText == 0) {
		fetchTexture("snow.jpg");
	}
	if (_useText == 1) {
		fetchTexture("coal.jpg");
	}
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0, 0); glNormal3fv(v1); glVertex3fv(v1);
	glTexCoord2f(1, 0); glNormal3fv(v2); glVertex3fv(v2);
	glTexCoord2f(0.5, 1); glNormal3fv(v3); glVertex3fv(v3);
	glEnd();


}
void Sphere::subDivide(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int passes){
	GLfloat v12[3], v23[3], v31[3];
	GLint i;

	if (passes == 0) {
		drawTriangle(v1, v2, v3);
		return;
	}
	for (i = 0; i < 3; i++) {
		v12[i] = v1[i] + v2[i];
		v23[i] = v2[i] + v3[i];
		v31[i] = v3[i] + v1[i];
	}
	normalize(v12);
	normalize(v23);
	normalize(v31);
	subDivide(v1, v12, v31, passes - 1);
	subDivide(v2, v23, v12, passes - 1);
	subDivide(v3, v31, v23, passes - 1);
	subDivide(v12, v23, v31, passes - 1);
}

void Sphere::draw(DrawingState* drst){
	int passes = 0;

	float lightDir[4] = { 0, 0, 0, 0 };

	if ((drst->timeOfDay < 7) || (drst->timeOfDay > 18)){
		passes = 1;
	}
	
	if ((drst->timeOfDay < 3) || (drst->timeOfDay > 22)){
		passes = 2;
	}

	char* error;

	if ((drst->timeOfDay >= 5) && (drst->timeOfDay <= 19)) {
		float angle = (((float)(drst->timeOfDay - 5)) / 7.f) * (3.14159f / 2.f);
		lightDir[0] = (float)cos(angle);
		lightDir[1]= (float)sin(angle);
	}
	normalize(lightDir);

	//goofy cartoon.
	static unsigned int shader;
	if (_shaderType == 0) {
		static unsigned int shader = loadShader("toonShader.vert", "toonShader.frag", error);
		GLint loc = glGetUniformLocation(shader, "lightDir");
		glUniform3f(loc, lightDir[0], lightDir[1], lightDir[2]);
		glUseProgram(shader);

	
	}
	//directional
	if (_shaderType == 1) {
		static unsigned int shader = loadShader("directional.vert", "directional.frag", error);
		GLint loc = glGetUniformLocation(shader, "lightDir");
		glUniform3f(loc, lightDir[0], lightDir[1], lightDir[2]);
		glUseProgram(shader);

	}
	//phong
	if (_shaderType == 2) {
		static unsigned int shader = loadShader("phong.vert", "phong.frag", error);
		/*GLint loc = glGetUniformLocation(shader, "lightDir");
		glUniform3f(loc, lightDir[0], lightDir[1], lightDir[2]);

		//calculate the eye vector.
		float pointPosition[3] = { 0, 0, 0,};
		float eye[3] = { 0, 0, 0, };

		Matrix cam;
		drst->camera->getCamera(cam);

		float camX = cam[0][2];
		float camY = cam[1][2];
		float camZ = cam[2][2];

		//object point position.
		pointPosition[0] = laX;
		pointPosition[1] = laY;
		pointPosition[2] = laZ;

		//look vector, camera position - point position.
		eye[0] = camX - pointPosition[0];
		eye[1] = camY - pointPosition[1];
		eye[2] = camZ - pointPosition[2];

		normalize(eye);

		GLint loc1 = glGetUniformLocation(shader, "eyeVec");
		glUniform3f(loc1, eye[0], eye[1],eye[2]);
		*/
		glUseProgram(shader);
		
	}
	glScaled(mScale, mScale, mScale);
	for (int i = 0; i < 20; i++) {
		subDivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], passes);
	}

	glUseProgram(0);
}
