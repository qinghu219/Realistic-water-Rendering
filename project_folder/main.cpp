#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "src/sdl_manager.h" //adding SDL frame
#include "src/fft.h"	//adding fft support
#include "src/vec.h"     //adding vector support
#include "src/timer.h"	//adding timer support
#include "src/ocean.h"  //adding ocean
#include "src/cube.h" //adding skybox
#define WIDTH 128*10
#define HEIGHT 72*10

cTimer t;
sdl_manager curSDL("Rendering", WIDTH, HEIGHT);  //initialize SDL frame
ocean curOcean(64, 0.000005f, vec2(0.f,35.0f), 64);    //initialize the ocean
cube curCube; //initialize the skybox

void setupPerspective(int width, int height, double fovy, double aspect, double zNear, double zFar) {
	double xmin, xmax, ymin, ymax;
	ymax = zNear * tan(fovy * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void opengl_init()
{
	setupPerspective(WIDTH, HEIGHT, 100.0f, float(WIDTH)/float(HEIGHT), 0.1f, 2000.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}
int main(int argc, char** argv)
{
	opengl_init();
	curCube.init();
	curSDL.myOcean = (&curOcean);
	float init_A = 0.0005f;
	curOcean.set_A(init_A);
	curSDL.curA = init_A;
	while (!curSDL.quit) {
		//adding SDL mouse and keyboards support
		curSDL.sdl_event();
		
		//begin rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Set up the camera
		//for rendering ocean, we need to have the view matrix in the vertex shader, therefore, manually set the view matrix  
		curOcean.view = glm::lookAt (
		     glm::vec3(curSDL.camX,curSDL.camY,curSDL.camZ),
		     glm::vec3(curSDL.camX+curSDL.lineX,curSDL.camY+curSDL.lineY,curSDL.camZ+curSDL.lineZ),
		     glm::vec3(0,1,0));	

		//for rendering skybox, we just use OpenGL build-in ModelView matrix, therefore, just change the view
		gluLookAt(curSDL.camX, curSDL.camY, curSDL.camZ,
			curSDL.camX+curSDL.lineX, curSDL.camY+curSDL.lineY, curSDL.camZ+curSDL.lineZ,
			0.0f, 1.0f,  0.0f);
		
		//for rendering skybox, scale the skybox to be as big as the ocean surface	
		glScalef(64*3,64*3,64*3);

		//for rendering ocean, set the eye position to be the current camera position (we need this eye position in the ocean's vertex shader)
		curOcean.ep = glm::vec3(curSDL.camX,curSDL.camY,curSDL.camZ);
		
		//render the ocean surface at time t
		if (curSDL.renderOcean)
			curOcean.render(t.elapsed(false));
	
		//render the sky box		
		curCube.cubeRender();
	
		//swap buffer
		SDL_GL_SwapWindow(curSDL.window);
		/*
		curSDL.camX=12.8854;
		curSDL.camY=49; 
		curSDL.camZ=51.4943; 
		curSDL.lineX=-0.589775;
		curSDL.lineY=-0.61173;
		curSDL.lineZ=-0.807567;
		*/
		/*
		curSDL.camX=0;
		curSDL.camY=10; 
		curSDL.camZ=0;
		*/
		/*
		curSDL.lineX=-0.17903;
		curSDL.lineY=0.375262;
		curSDL.lineZ=0.983844;
		
		std::cout << curSDL.camX << " " << curSDL.camY << " " << curSDL.camZ << " " << curSDL.lineX << " " << curSDL.lineY << " " << curSDL.lineZ << std::endl;
		*/
		//delay the time
		SDL_Delay(30);
	}
}
