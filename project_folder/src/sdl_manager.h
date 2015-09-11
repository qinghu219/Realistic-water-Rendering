#ifndef sdl_manager_H
#define sdl_manager_H
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "vec.h"
#include "c_num.h"
#include "fft.h"
#include "ocean.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "math.h"
class sdl_manager
{
	public:
		float angelXZ, angelYZ, camX, camY, camZ, lineX, lineY, lineZ, angel_delta, deltaY,line_fraction;
		int quit, w, h;
		float mouseX, mouseY;
		bool renderOcean;
		bool mouse;
		SDL_Event sdlEv;		
		SDL_GLContext glcontext;
		SDL_Window *window;
		ocean* myOcean;
		float curA;
        sdl_manager(std::string window_name, int width, int height);
		void sdl_event();
		
};
#endif
