// TNM084 Lab 1
// Procedural textures
// Draws concentric circles on CPU and GPU
// Make something more interesting!

// For the CPU part, you should primarily work in maketexture() below.

// New C++ version!

//uses framework Cocoa
//uses framework OpenGL
#define MAIN
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include <stdio.h>
#include "VectorUtils4.h"
#include "LittleOBJLoaderX.h"

// Lab 1 specific
// Code for you to access to make interesting patterns.
// This is for CPU. For GPU, I provide separate files
// with functions that you can add to your fragment shaders.
#include "noise1234.h"
#include "simplexnoise1234.h"
#include "cellular.h"

#define kTextureSize 512
GLubyte ptex[kTextureSize][kTextureSize][3];
//Change Uniform frequency
const float ringDensity = 60.0;

// Example: Radial pattern.
void maketexture()
{
	int x, y;
	float fx, fy, fxo, fyo;
//	char val;

	for (x = 0; x < kTextureSize; x++)
	for (y = 0; y < kTextureSize; y++)
	{
	    //Matrix
		if(y % 24 == 0) ptex[x][y][1] = snoise2(x,y)*255;

	}
}

// Globals
// Data would normally be read from files
vec3 vertices[] = { vec3(-1.0f,-1.0f,0.0f),
					vec3(1.0f,-1.0f,0.0f),
					vec3(1.0f,1.0f,0.0f),
					vec3(-1.0f,1.0f,0.0f),
					vec3(1.0f,1.0f,0.0f),
					vec3(1.0f,-1.0f,0.0f) };
vec2 texCoords[] = {vec2(0.0f,0.0f),
					vec2(1.0f,0.0f),
					vec2(1.0f,1.0f),
					vec2(0.0f,1.0f),
					vec2(1.0f,1.0f),
					vec2(1.0f,0.0f) };
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

// vertex array object
//unsigned int vertexArrayObjID;
// Texture reference
GLuint texid;
// Switch between CPU and shader generation
int displayGPUversion = 0;
// Reference to shader program
GLuint program;
// The quad
Model* quad;

float time;
void init(void)
{
	// two vertex buffer objects, used for uploading the
//	unsigned int vertexBufferObjID;
//	unsigned int texBufferObjID;

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1.vert", "lab1.frag");
	glUseProgram(program);
	printError("init shader");

	// Upload geometry to the GPU:

	quad = LoadDataToModel(vertices,NULL,texCoords,NULL,indices,4,6);
	// Texture unit
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0

// Constants common to CPU and GPU
    glutRepeatingTimer(100);
    time = 100.0;
	glUniform1i(glGetUniformLocation(program, "displayGPUversion"), 0); // shader generation off
	glUniform1f(glGetUniformLocation(program, "ringDensity"), ringDensity);
	glUniform1f(glGetUniformLocation(program, "t"), time);

	maketexture();

// Upload texture
	glGenTextures(1, &texid); // texture id
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, kTextureSize, kTextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, ptex);

	// End of upload of geometry
	printError("init arrays");
}

// Switch on any key
void key(unsigned char key, int x, int y)
{
	displayGPUversion = !displayGPUversion;
	glUniform1i(glGetUniformLocation(program, "displayGPUversion"), displayGPUversion); // shader generation off
	printf("Changed flag to %d\n", displayGPUversion);
	glutPostRedisplay();
}

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawModel(quad, program, "in_Position", NULL, "in_TexCoord");

    glUniform1f(glGetUniformLocation(program, "t"), (float)glutGet(GLUT_ELAPSED_TIME)/1000.0);

	printError("display");

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowSize(kTextureSize, kTextureSize);
	glutCreateWindow ("Lab 1");
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutRepeatingTimer(10);
	init ();
	glutMainLoop();
}
