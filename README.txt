This is a project I completed in Fall of 2014 while attending the University of Wisconsin, Madison. 
We were provided framework code that was written entirely in Legacy openGL using only immediate mode drawing. 
The basic project consisted of a working, living “Graphics Town” that had very basic houses, roads, some moving cars, and an older styled global lighting system. 

I adapted this town to create a “winter town”; a town existing in an imagined world that was quiet and peaceful. I created a dynamic weather system, a tree generation system, log cabins, and snowmen. My contributions are found in the files listed at the bottom. Also, you will find comments above any functions that I added/created signifying that I wrote them. Some algorithms were adopted from the OpenGL redbook.

Features:

- Trees that always face the user/camera via the technique known as “billboarding”

- A particle system that I used for both rain and snow. The particle speed, shape, and draw distance changes based on the time-of-day. 

- A skymap (created using a pretty simple, giant set of quads)

- Detailed log cabins using legacy OpenGL drawing. These were created using a combination of small algorithms to generate walls (can be found in drawUtils.cpp), as well as some admittedly inefficient drawing for some of the more obscure pieces of the houses (namely the individual roof and deck support pieces). I also used a basic shader that changed the color of the window based on time-of-day as well.

-Snowmen that “melt” and change shape based on the time-of-day. I created the sections of the snowmen using surface subdivision. The snowmen “melt” during the day (0 – 1 subdivision passes), and refreeze during the evening (as many as 3 or 4 passes). I also used these snowmen to demonstrate the usage of very basic shaders. 

Notable Contributions/Files to check for code that I contributed:

DrawUtils.cpp/h (Many drawing utilities for logs, fencing, roofs, etc)
ParticleSystem.cpp/h
Sphere.cpp/h 
Skybox.cpp/h
Main.cpp/h
GrObject.cpp/h (Various objects that were drawn)
Objects.cpp/h (Where I draw the log house, a tiny bit of modern OpenGL, trees, etc.)
