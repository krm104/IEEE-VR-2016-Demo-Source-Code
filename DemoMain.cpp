
//#include "Globals.h"
#include "WindowFunctions.h"
#include "LeapFunctions.h"
#include "ARTFunctions.h"
#include "GLUIFunctions.h"
#include "StateFunctions.h"

int main(int argc, char** argv)
{
	//////////Build GL Window//////////
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 1024);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	WindowHandle = glutCreateWindow("VR 2016 Demo");

	glutReshapeFunc(WindowReshape);
	glutDisplayFunc(WindowRender);
	//glutIdleFunc(WindowIdle);

	glewInit();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	///////////////////////////////////

	//////////Build GLUI Menu//////////
	BuildGLUI();
	GLUI_Master.set_glutIdleFunc(WindowIdle);
	///////////////////////////////////

	//////////Initialize Leap and ART//////////
	initLM();
	startDtrack(5002);
	///////////////////////////////////

	//////////Build State Items and Objects//////////
	BuildStates();
	///////////////////////////////////

	//////////Start Rendering//////////
	glutMainLoop();
	///////////////////////////////////

	//////////Clean Up//////////
	stopDtrack();
	unInitLM();
	///////////////////////////////////

	//End of Program//
	exit(0);
}
