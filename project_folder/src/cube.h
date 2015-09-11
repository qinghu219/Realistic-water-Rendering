#ifndef cube_H 
#define cube_H
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class cube {
	public:
		GLuint cubeArray, cube_arrayBuffer, cube_eleBuffer, cubeProgram, cube_fs, cube_vs, cubTexture;
		void setupCubeTexture(GLuint& curTexture);
		char* file2string(const char *path);
		void loadCubeShaders();
		void renderCubeTexture();
		void cubeRender();
		void init();



};

#endif
