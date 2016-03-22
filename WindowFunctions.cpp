#include "WindowFunctions.h"

void WindowReshape(int wth, int hgt)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, double(wth) / double(hgt), 1, 1500);
	glViewport(0, 0, wth, hgt);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void WindowRender()
{
	if (glutGetWindow() != WindowHandle)
		glutSetWindow(WindowHandle);
	///////////////////////////////////////////////

	LeapTracking();
	UpdateDtrack();
	UpdateState();
	
	MainRenderFunction();

	///////////////////////////////////////////////
	glutSwapBuffers();
}

void WindowIdle()
{
	if (glutGetWindow() != WindowHandle)
		glutSetWindow(WindowHandle);

	glutPostRedisplay();
	glui->sync_live();
}