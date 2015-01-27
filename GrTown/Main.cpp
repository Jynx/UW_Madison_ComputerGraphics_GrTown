/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
//
// updated Fall 2006 - more testing code
// updated Fall 2008 - switched some internal stuff (no more linked lists)

#include "GrTown_PCH.H"
#include "GraphicsTownUI.H"
#include "FlyCamera.H"
#include "GrWorld.H"
#include "DrawUtils.H"
#include "DrawingState.H"
#include "Examples/Objects.H"
#include "Examples/SimpleBehaviors.H"
#include "Examples/Cars.H"
#include "ParticleSystem.h"
#include "Sphere.h"
#include "SkyBox.h"

// for setting up shader paths and textures
#include "Utilities/ShaderTools.H"
#include "Utilities/Texture.H"


// Example code (objects and behaviors)
#include "Examples/Suburbs.H"

#include "Roads/Roads.H"
#include "Roads/Drive.H"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <glm/glm.hpp>


// define this to get 2 cars that always turn
// #define TESTCARS

const int numCars = 100;
const int nGrids = 5;


int main(int /*argc*/, char** /*argv*/)
{ 

  // put in some texture paths - look all over the place
	texturePaths.push_back(".");
	texturePaths.push_back("..");
	texturePaths.push_back("Textures/signs");
	texturePaths.push_back("../Textures/signs");
	texturePaths.push_back("Textures/Textures");
	texturePaths.push_back("../Textures/Textures");
	texturePaths.push_back("Textures/Objects");
	texturePaths.push_back("../Textures/Objects");

	shaderPaths.push_back("Shaders");
	shaderPaths.push_back("../Shaders");


  // *****************************************************************
  //  Make your town here

  // added by Steven Volocyk lines 64 to 217
  SkyBox* skybox = new SkyBox();
  add(skybox);

  ParticleSystem* particle = new ParticleSystem();
  particle->InitializeParticles(false);
  add(particle);

  GrObject* lawn = new Lawn(0, 0, 400, 400);
  GrObject* lawn2 = new Lawn(0, 0, -400, 400);
  GrObject* lawn3 = new Lawn(0, 0, -400, -400);
  GrObject* lawn4 = new Lawn(0, 0, 400, -400);
  add(lawn);  add(lawn2);  add(lawn3);  add(lawn4);

  GrObject* loghouse = new SimpleLogHouse1(-210, 0, 190, 300, 0.0, 1.0, 0.0);
  add(loghouse);
  loghouse = new SimpleLogHouse2(210, 0, 190, 90, 0.0, 1.0, 0.0);
  add(loghouse);

  loghouse = new SimpleLogHouse1(350, 0, -100, 180, 0.0, 1.0, 0.0);
  add(loghouse);
  loghouse = new SimpleLogHouse2(230, 0, -100, 270, 0.0, 1.0, 0.0);
  add(loghouse);
  loghouse = new SimpleLogHouse1(130, 0, -100, 180, 0.0, 1.0, 0.0);
  loghouse->interesting = true;
  loghouse->laX = 130; loghouse->laY = 0;   loghouse->laZ = -100;
  loghouse->lfX = 40; loghouse->lfY = 30; loghouse->lfZ = -0;
  loghouse->name = "Loghouse1";
  add(loghouse);

  loghouse = new SimpleLogHouse2(130, 0, -220, 180, 0.0, 1.0, 0.0);
  loghouse->interesting = true;
  loghouse->laX = 130; loghouse->laY = 0;   loghouse->laZ = -220;
  loghouse->lfX = -0; loghouse->lfY = 30; loghouse->lfZ = -250;
  loghouse->name = "Loghouse2";
  add(loghouse);

  loghouse = new SimpleLogHouse1(0, 0, -320, 180, 0.0, 1.0, 0.0);
  add(loghouse);
  loghouse = new SimpleLogHouse2(-110, 0, -320, 270, 0.0, 1.0, 0.0);
  add(loghouse);
  loghouse = new SimpleLogHouse1(-210, 0, -320, 180, 0.0, 1.0, 0.0);
  add(loghouse);
  loghouse = new SimpleLogHouse2(-320, 0, -320, 270, 0.0, 1.0, 0.0);
  add(loghouse);

  //distribute trees around plane

  for (int i = 0; i < 200; i++){
	GrObject* tree = new BillboardTree(rand() % 400, -10, rand() % 400 , rand());
	add(tree); 
	tree = new BillboardTree(-(rand() % 400), -10, rand() % 400, rand());
	add(tree);
	tree = new BillboardTree(-(rand() % 400), -10, -rand() % 200, rand());
	add(tree);
  }

  GrObject* tree = new BillboardTree(-(rand() % 400), -10, -rand() % 200, rand());
  tree->interesting = true;
  tree->name = "Trees & Particles";
  tree->laX = 200; tree->laY = 0; tree->laZ = 150;
  tree->lfX = 0; tree->lfY = 400; tree->lfZ = -300;
  add(tree);
 
  GrObject* road = new StraightRoad(30, -30, 400, -30);
  add(road);
  road = new StraightRoad(45, -45, 45, -265);
  add(road);
  road = new StraightRoad(45, -250, -400, -250);
  add(road);

  //draw snowman!
  Sphere* sphere = new Sphere(300.0, 10.0, -300.0, 20.0, 0, 1.0, 1.0, 1.0, 0, 0);
  add(sphere);
  sphere = new Sphere(300, 40, -300, 15, 0, 1.0, 1.0, 1.0, 0, 0);
  add(sphere);
  sphere = new Sphere(300, 63, -300, 10, 0, 1.0, 1.0, 1.0, 0,0);
  add(sphere);
  //eye
  sphere = new Sphere(304, 66, -291, 1.5, 0, 1.0, 1.0, 1.0, 1,0);
  add(sphere);
  //eye
  sphere = new Sphere(293, 66, -291, 1.5, 0, 1.0, 1.0, 1.0, 1,0);
  add(sphere);
  //mouth left
  sphere = new Sphere(294, 60, -291, .90, 0, 1.0, 1.0, 1.0, 1,0);
  add(sphere);
  sphere = new Sphere(296, 58, -291, .90, 0, 1.0, 1.0, 1.0, 1,0);
  add(sphere);
  sphere = new Sphere(298.5, 57, -291, .90, 0, 1.0, 1.0, 1.0, 1,0);
  add(sphere);
  //mouth right
  sphere = new Sphere(301, 58, -291, .90, 0, 1.0, 1.0, 1.0, 1,0);
  add(sphere);
  sphere = new Sphere(303, 60, -291, .90, 0, 1.0, 1.0, 1.0, 1,0);
  add(sphere);

  //snowman2
  sphere = new Sphere(350.0, 10.0, -350.0, 20.0, 0, 1.0, 1.0, 1.0, 0, 1);
  add(sphere);
  sphere = new Sphere(350, 40, -350, 15, 0, 1.0, 1.0, 1.0, 0, 1);
  add(sphere);
  sphere = new Sphere(350, 63, -350, 10, 0, 1.0, 1.0, 1.0, 0, 1);
  add(sphere);
  //eye
  sphere = new Sphere(354, 66, -341, 1.5, 0, 1.0, 1.0, 1.0, 1, 1);
  add(sphere);
  //eye
  sphere = new Sphere(343, 66, -341, 1.5, 0, 1.0, 1.0, 1.0, 1, 1);
  add(sphere);
  //mouth left
  sphere = new Sphere(344, 60, -341, .90, 0, 1.0, 1.0, 1.0, 1, 1);
  add(sphere);
  sphere = new Sphere(346, 58, -341, .90, 0, 1.0, 1.0, 1.0, 1, 1);
  add(sphere);
  sphere = new Sphere(348.5, 57, -341, .90, 0, 1.0, 1.0, 1.0, 1, 1);
  add(sphere);
  //mouth right
  sphere = new Sphere(351, 58, -341, .90, 0, 1.0, 1.0, 1.0, 1,1);
  add(sphere);
  sphere = new Sphere(353, 60, -341, .90, 0, 1.0, 1.0, 1.0, 1,1 );
  add(sphere);
  
  //snowman 3
  sphere = new Sphere(250.0, 10.0, -250.0, 20.0, 0, 1.0, 1.0, 1.0, 0, 2);
  add(sphere);
  sphere = new Sphere(250, 40, -250, 15, 0, 1.0, 1.0, 1.0, 0, 2);
  add(sphere);
  sphere = new Sphere(250, 63, -250, 10, 0, 1.0, 1.0, 1.0, 0, 2);
  add(sphere);
  //eye
  sphere = new Sphere(254, 66, -241, 1.5, 0, 1.0, 1.0, 1.0, 1, 2);
  add(sphere);
  //eye
  sphere = new Sphere(243, 66, -241, 1.5, 0, 1.0, 1.0, 1.0, 1, 2);
  add(sphere);
  //mouth left
  sphere = new Sphere(244, 60, -241, .90, 0, 1.0, 1.0, 1.0, 1, 2);
  add(sphere);
  sphere = new Sphere(246, 58, -241, .90, 0, 1.0, 1.0, 1.0, 1, 2);
  add(sphere);
  sphere = new Sphere(248.5, 57, -241, .90, 0, 1.0, 1.0, 1.0, 1, 2);
  add(sphere);
  //mouth right
  sphere = new Sphere(251, 58, -241, .90, 0, 1.0, 1.0, 1.0, 1, 2);
  add(sphere);
  sphere = new Sphere(253, 60, -241, .90, 0, 1.0, 1.0, 1.0, 1, 2);
  add(sphere);
  sphere->interesting = true;
  sphere->name = "Snowmen!";
  sphere->laX = 250; sphere->laY = 0; sphere->laZ = -240;
  sphere->lfX = -200; sphere->lfY = 10; sphere->lfZ = 100;

  //end Steven Volocyk Additions

  // now make a UI
  FlyCamera* fc = new FlyCamera;
  Map* m = new Map;

  add(fc);
  add(m);

  GraphicsTownUI grTown;
  grTown.make_window();
  grTown.townView->defaultCamera = fc;
  grTown.window->show();

  Fl::run();
  return 1;
}

// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/Main.cpp,v 1.8 2009/11/10 22:40:03 gleicher Exp $
