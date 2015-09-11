#include "sdl_manager.h"
#include <iostream>


sdl_manager::sdl_manager(std::string window_name, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
		exit(EXIT_FAILURE);		
	};
	

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	window = SDL_CreateWindow(
	    window_name.c_str(), 0, 0, width, height,
   		SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	glcontext = SDL_GL_CreateContext(window);
	angelXZ = M_PI / 2.;
	angelYZ = 0.0;
	camX = 0.0;
	camY = 10.0;
	camZ = 0.0;
	lineX = cos(angelXZ);
	lineY = tan(angelYZ);
	lineZ = sin(angelXZ);
	angel_delta = 0.01f;
	deltaY = 1.f;
	line_fraction = 10.f;
	mouse = false;
	mouseX = 0.f;
	mouseY = 0.f;
	quit = 0;	
	renderOcean = false;
	w = width;
	h = height;

}
void sdl_manager::sdl_event()
{
	while (SDL_PollEvent(&sdlEv)) 
		switch (sdlEv.type)
		{
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				switch (sdlEv.key.keysym.sym) {
					case SDLK_LEFT:
						angelXZ -= angel_delta;
						lineX = cos(angelXZ);
						lineZ = sin(angelXZ);	
						break;
					case SDLK_RIGHT:
						angelXZ += angel_delta;
						lineX = cos(angelXZ);
						lineZ = sin(angelXZ);
						break;
					case SDLK_UP:
						angelYZ += angel_delta;
						if (angelYZ > M_PI / 2. ) angelYZ -= angel_delta;
						lineY = tan(angelYZ);
						break;
					case SDLK_DOWN:
						angelYZ -= angel_delta;
						if (angelYZ < 0.f) angelYZ = 0.f;
						lineY = tan(angelYZ);
						break;
					case SDLK_u:
						camY += deltaY;
						break;
					case SDLK_d:
						camY -= deltaY;
						break;
					case SDLK_w:		
						curA += 0.000001f;
						myOcean->set_A(curA);
						break;
					case SDLK_q:
						curA -= 0.000001f;
						if (curA < 0.) curA = 0.0f;
						myOcean->set_A(curA);
						break;
					case SDLK_a:
						renderOcean = !renderOcean;
						break;
					case SDLK_SPACE:
						camX = camX + lineX * line_fraction;
						camZ = camZ + lineZ * line_fraction;
						break;
						
				}	
			case SDL_MOUSEMOTION:
				if (sdlEv.button.button == SDL_BUTTON_LEFT) {
					float temX = sdlEv.motion.xrel;
					float temY = sdlEv.motion.yrel;
					angelXZ -= angel_delta * temX * 0.1;
					angelYZ += angel_delta * temY * 0.1;
                    if (angelYZ < -1.570) {
                        angelYZ = -1.570;
                    }
                    if (angelYZ > 1.570) {
                        angelYZ = 1.570;
                    }

					lineX = cos(angelXZ);
					lineZ = sin(angelXZ);	
					lineY = tan(angelYZ);
				}
				break;	
			case SDL_MOUSEBUTTONDOWN:
				SDL_SetRelativeMouseMode(SDL_TRUE);
				break;
			case SDL_MOUSEBUTTONUP:
				SDL_SetRelativeMouseMode(SDL_FALSE);
				SDL_WarpMouseInWindow(window,w/2,h/2);
				break;				
			default:
				break;
		}

}
