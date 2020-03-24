/* frontend for opengl */
#include <string>
#include "open3D.hpp"

/* global variables */
#define ANGLE 0.0017683882f
GLint window;
bool* keyStates;
GLboolean leave = false;
GLboolean rotate = false;

GLfloat eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz;
//angle of rotation
GLfloat angx, angy, angz; 
//change in coordinate
GLfloat shift_x, shift_y, shift_z;

GLint open3D(int argc, char **argv, GLsizei xLim, GLsizei yLim){
	
	GLint xPos, yPos;
	/* Some initialization */
	initGlobals();
	
	glutInit(&argc, argv); // Initialize GLUT
	/* Single or double buffer */
	// glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); // Set up a basic display buffer (only single buffered for now)
	// for double buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
	/* define the windows size*/
	glutInitWindowSize(xLim, yLim); // Set the width and height of the window 
	// needed to continue if window is closed but it does not work properly
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_CONTINUE_EXECUTION); 
	xPos = glutGet(GLUT_SCREEN_WIDTH);
	yPos = glutGet(GLUT_SCREEN_HEIGHT);
	xPos= (xPos - xLim)/2;
	yPos= (yPos - yLim)/2;
	/* we define the initial position */
	glutInitWindowPosition(xPos, yPos); // Set the position of the window
	/* finally we open the window */
	window = glutCreateWindow("Gravity"); // Set the title for the window
	
	/* for the lights */
	enableLights();
	
	/* we define th callbacks */
	// Tell GLUT to use the method "display" for rendering
	glutDisplayFunc(display);
	// Tell GLUT to use the method ""idler" as our idle method as well
	glutIdleFunc(idler); 
	// Tell GLUT to use the method "reshape" for rendering
	glutReshapeFunc(reshape); 
	// Tell GLUT to use the method "keyPressed" for key presses 
	glutKeyboardFunc(keyPressed);
	// Tell GLUT to use the method "keyUp" for key up events 
	glutKeyboardUpFunc(keyUp);
	// Special key to exit 
	glutSpecialFunc(keySpecial);
	glutSpecialUpFunc(keySpecialUp);
#if FREEGLUT
	glutCloseFunc(closeAll);
#endif
	return window;
};

void camera (void) {
	
	glLoadIdentity();
	glTranslatef(-eyex, -eyey, -eyez); //translate the screen to the position of our camera
	if (true)
	{
		glRotatef(angx, 1.0, 0.0, 0.0);  //rotate our camera on the x-axis (left and right)
	    glRotatef(angy, 0.0, 1.0, 0.0);  //rotate our camera on the y-axis (up and down)
	    glRotatef(angz, 0.0, 0.0, 1.0);  //rotate our camera on the z-axis (up and down)
	}
   
    return;
}

/* most advanced version */
void renderForCamera(Planet *p){ 
	
	GLfloat x, y, z;
	
	const GLfloat *cmap = colorMap[p->readIndex()]; 
	x = p->x[XX];
	y = p->x[YY];
	z = p->x[ZZ];
	
	glPushMatrix();
	glColor3f(cmap[0], cmap[1], cmap[2]);
	glTranslatef(x, y, z);
	glutSolidSphere(0.5, 8, 8);
	glPopMatrix();

	return;
}

void reshape(int width, int height){
	
	// Set our viewport to the size of our window 
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  
	glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed 
	
	glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up) 
	
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes 
	
	glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly 
	
	return;
}

void idler(void){

	//keyOperations();
	/* ADD HERE THE MAIN LOOP OF THE CALCULATION AND PLOT STEP BY STEP */
	//glFinish
	
};

void display(void){
	
	// Clear the background of our window to red 
	// glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	// Clear the colour buffer (more buffers later on) 
	// glClear(GL_COLOR_BUFFER_BIT); 
	
	// Load the Identity Matrix to reset our drawing locations 
	
	// Push eveything 50 units back into the scene, otherwise we won't see the primitive 
	//glTranslatef(0.0f, 0.0f, -eyez);
	//renderPrimitive(); // Render the primitive
	//glutWireCube(2.0f); // Render the primitive
	//glColor3f(0.5, 0.5, 0.5);
	//glutSolidTeapot(2.0);
	
	glLoadIdentity(); 
	/* set the lights ? */
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	// Flush the OpenGL buffers
	glFlush();
	glutSwapBuffers();
	return; 
}

void closeAll(void){
	
	//glFinish();
	//glutSwapBuffers();
	glutDestroyWindow(window);
	exit(-1);
	return;
}

void keyPressed(GLubyte key, GLint x, GLint y){

	keyStates[key] = true; // Set the state of the current key to pressed 	
	if (key == 27){ // ESC
		glFlush();
		glFinish();
		glutSwapBuffers();
		leave = true;
		exit(-1);
		return;
	}
	
	if (key == 'a')
	{
		float yrotrad;
		
		angy += -5.0;
		if (angx <= 360) angx = 0.0; 
		yrotrad = (angy * ANGLE);
//	    xrotrad = (angx * ANGLE);
	    eyex  =  eyex*cosf(yrotrad);
//	    eyey  =  eyey;
	    eyez -= +eyey*sinf(yrotrad);
		rotate = true;
		return;
	}
	
	if (key == 'd')
	{
		float yrotrad;
		
		angy += +5.0;
		if (angx >= 360) angx = 0.0; 
		yrotrad = (angy * ANGLE);
//	    xrotrad = (angx * ANGLE);
	    eyex  =  eyex*cosf(yrotrad);
//	    eyey  =  eyey;
	    eyez -= +eyey*sinf(yrotrad);
		rotate = true;
		return;
	}
	
	if (key == 'w')
	{
		float xrotrad;
		
		angx += +5.0;
		if (angx >= 360) angx = 0.0; 
//		yrotrad = (angy * ANGLE);
	    xrotrad = (angx * ANGLE);
//	    eyex  = eyex;
	    eyey  =  eyey*cosf(xrotrad);
	    eyez -= -eyey*sinf(xrotrad);
		rotate = true;
		return;
	}
	
	if (key == 'x')
	{
		float xrotrad;
		
		angx += -5.0;
		if (angx <= -360) angx = 0.0; 
//		yrotrad = (angy * ANGLE);
	    xrotrad = (angx * ANGLE);
//	    eyex  = eyex;
	    eyey  =  eyey*cosf(xrotrad);
	    eyez -= -eyey*sinf(xrotrad);
		rotate = true;
		return;
	}
	
	
	if (key == 'q')
	{
		glFlush();
		glFinish();
		glutSwapBuffers();
		leave = true;
		return;
	}
	
	if (key == '+')
	{
		eyez = eyez - 1.0;
		if (eyez < 0.0) eyez = 0.0;
		return;
	}
	
	if (key == '0')
	{
		initGlobals();
		return;
	}
	
	if (key == '-')
	{
		eyez = eyez + 1.0;
		return;
	}
	
	
	return;
} 

void keyUp(GLubyte key, int x, int y){ 
	
	keyStates[key] = false; // Set the state of the current key to not pressed 
	return;
}

void keySpecial(GLint key, int x, int y){ 
	
	keyStates[key] = true;
	
	// Rotate Y axis clockwise
	if (keyStates[GLUT_KEY_LEFT]) 
	{ 
		eyex += +1.0;
		/*
	    yrotrad = (angy * ANGLE);
	    xrotrad = (angx * ANGLE);
	   
	    eyex -= float(sinf(yrotrad));
	    eyez += float(cosf(yrotrad));
	    eyey += float(sinf(xrotrad));
		eyex += -1.0;
		* */
		return;
	}

	// rotate Y axis anticlockwise
	if (keyStates[GLUT_KEY_RIGHT]) 
	{ 

		eyex -= +1.0;
		return;
	}
	
	if (keyStates[GLUT_KEY_UP]) 
	{ 
	    
	    eyey -= +1.0;
	    /*
	     * float xrotrad, yrotrad;
	    yrotrad = (angy * ANGLE);
	    xrotrad = (angx * ANGLE);
	    eyex += float(sinf(yrotrad)) ;
	    eyez -= float(cosf(yrotrad)) ;
	    eyey -= float(sinf(xrotrad)) ;
	    */
		return;
	}
	
	if (keyStates[GLUT_KEY_DOWN]) 
	{ 
		
		eyey += +1.0;
		/*GLfloat xrotrad, yrotrad;
	    yrotrad = (angy * ANGLE);
	    xrotrad = (angx * ANGLE);
	    eyex -= float(sinf(yrotrad));
	    eyez += float(cosf(yrotrad));
	    eyey += float(sinf(xrotrad));
	    */
		return;
	}
	
	return;
}

void keySpecialUp(GLint key, int x, int y){ 
	
	keyStates[key] = false; 
	return;
}

void initGlobals(void){

	eyex = 0.0;
	eyey = 0.0;
	eyez = BASE_Z;
	centerx = 0.0;
	centery = 0.0;
	centerz = 0.0;
	upx = 0.0;
	upy = 1.0;
	upz = 0.0;
	angx = 0.0;
	angy = 0.0;
	angz = 0.0;
	shift_x = 0.0;
	shift_y = 0.0;
	shift_z = 0.0;
	if (keyStates == NULL)
	{
		keyStates = new bool[256];
	}
	
	return;
}
void enableLights(void){
	
	/* for the lights */
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    return;
}

void utilPrintEnd(){

	glColor3f(1.0, 1.0, 1.0);
	glRasterPos3f(1.0, 1.0, 1.0);

#ifdef FREEGLUT
	const unsigned char s[] = "Game Over";
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, s);
#else 
	std::string str("Game Over");
	for (std::string::iterator it = str.begin(); it < str.end(); it++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *it);
	}
#endif

	glFinish();
	glutSwapBuffers();
	return;
}

/* UNUSED */

void renderPrimitive(void){ 
	/* Basic example to draw a square */
	glColor3f(0.0f, 0.0f, 1.0f); // Set the colour of the square to blue
	glBegin(GL_QUADS); // Start drawing a quad primitive 
	glVertex3f(-1.0f, -1.0f, 0.0f); // The bottom left corner 
	glVertex3f(-1.0f, 1.0f, 0.0f); // The top left corner 
	glVertex3f(1.0f, 1.0f, 0.0f); // The top right corner 
	glVertex3f(1.0f, -1.0f, 0.0f); // The bottom right corner 
	glEnd();
	 
	return;
}


/* old version */
void renderPrimitive(Planet *p){ 
	
	GLfloat x, y, z;
	x = p->x[XX];
	y = p->x[YY];
	z = p->x[ZZ];

	const GLfloat *cmap = colorMap[p->readIndex()]; 
	glColor3f(cmap[0], cmap[1], cmap[2]); 
	//glClear(GL_COLOR_BUFFER_BIT); 
	// Load the Identity Matrix to reset our drawing locations 
	glLoadIdentity();
	/* translate the voxel to */
	glTranslatef(x-eyex, y-eyey, z-eyez);
	/* draw a shere */
	glutSolidCube(1);
//	glutSolidSphere(1.0, 8, 8);
	//glutSwapBuffers();
	
	return;
}

void renderPrimitive(Planet *p, GLsizei xSiz, GLsizei ySiz){ 
	
	GLfloat x, y, z;
	
	x = p->x[XX];
	y = p->x[YY];
	z = p->x[ZZ];

	// Set the colour of the drawn to blue
	glColor3f(0.0f, 0.0f, 1.0f); 
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Load the Identity Matrix to reset our drawing locations 
	glLoadIdentity(); 
	glTranslatef(x, y, z-eyez);
	glutSolidSphere(1.0, 8, 8);
	//glTranslatef(0.0, 0.0, 0.0);
	//glutSwapBuffers();
	return;
}
