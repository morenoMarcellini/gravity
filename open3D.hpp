#ifndef _OUTPUT3D_
#define _OUTPUT3D_

/* OPENGL 2.1 to be run on RPI first generation */

// Include standard headers 
#include <stdio.h> 
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic. 
#include <GL/glew.h>

// include glut.h
#include <GL/glut.h> // Include the GLUT header file
#include <GL/freeglut.h> // it is needed for few functions not included in standard glut

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM even if we do not use it: it does not hurt
#include <glm/glm.hpp>

#include <cmath>
#include <iostream>

#include "main.hpp"
#include "planet.hpp"
#include "colormap.hpp"

#define BASE_Z	10.0

/* true global variable */
extern GLboolean leave;

/* methods */
GLint open3D(int argc, char **argv, GLsizei xLim, GLsizei yLim);
void display(void);
void idler(void);
void reshape(GLsizei xSize, GLsizei ySize);
void closeAll(void);
void initGlobals(void);
void enableLights(void);
void camera(void);

/* keyboard callbacks*/
void keyPressed(GLubyte key, GLint xMouse, GLint yMouse);
void keyUp(GLubyte key, GLint x, GLint yMouse);
void keySpecial(GLint key, GLint xMouse, GLint yMouse);
void keySpecialUp(GLint key, GLint xMouse, GLint yMouse);

/* renderers */
void renderForCamera(Planet *p);
void renderPrimitive(void);
void renderPrimitive(Planet *p);
void renderPrimitive(Planet *p, GLsizei xSiz, GLsizei ySiz);

/* utilities*/
void utilPrintEnd();

#endif
