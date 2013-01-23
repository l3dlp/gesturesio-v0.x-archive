#include <stdlib.h>
#include "Viewer.h"
#include "NIEngine.h"

#define GL_WIN_SIZE_X	800
#define GL_WIN_SIZE_Y	600
#define TEXTURE_SIZE	512

const int CROPBOX_WIDTH = 400; // mm
const int CROPBOX_HEIGHT = 300; // mm
const int CROPBOX_DEPTH = 3500; // mm

// Colors for the points
float Colors[][3] =
{
	{0.5,0.5,0.5},	// Grey
	{0,1,0},		// Green
	{0,0.5,1},		// Light blue
	{1,1,0},		// Yellow
	{1,0.5,0},		// Orange
	{1,0,1},		// Purple
	{1,1,1},		// White
	{1,0,0},        // Red
	{0,0,1}         // Blue
};

Viewer* Viewer::ms_self = NULL;

Viewer::Viewer()
{
	ms_self = this;
}

Viewer::~Viewer()
{
	ms_self = NULL;
}

bool Viewer::Init(int argc, char **argv)
{
	InitOpenGL(argc, argv);

	return true;
}

void Viewer::InitOpenGL(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	glutCreateWindow ("Viewer");
	// 	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	InitOpenGLHooks();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);	
}

bool Viewer::Run()
{
	glutMainLoop();
	return true;
}

void Viewer::InitOpenGLHooks()
{
	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);
}

void Viewer::glutIdle()
{
	glutPostRedisplay();
}

void Viewer::glutDisplay()
{
	Viewer::ms_self->Display();
}

void Viewer::glutKeyboard(unsigned char key, int x, int y)
{
	Viewer::ms_self->OnKey(key, x, y);
}

void Viewer::OnKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit (1);
	}	
}

void Viewer::Display()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the OpenGL viewpoint
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, GL_WIN_SIZE_X, GL_WIN_SIZE_Y, 0, -1.0, 1.0);

	glDisable(GL_TEXTURE_2D);

	nite::Point3f headPos = NIEngine::GetInstance()->GetHeadPosProjective();
	nite::Point3f lHandPos = NIEngine::GetInstance()->GetLeftHandPosProjective();
	nite::Point3f rHandPos = NIEngine::GetInstance()->GetRightHandPosProjective();
	std::string gesture = NIEngine::GetInstance()->GetGesture();

	// Map points
	float lx = lHandPos.x / CROPBOX_WIDTH * GL_WIN_SIZE_X;
	float ly = lHandPos.y / CROPBOX_HEIGHT * GL_WIN_SIZE_Y;
	float lz = (1 - lHandPos.z / CROPBOX_DEPTH) * 100;
	float rx = rHandPos.x / CROPBOX_WIDTH * GL_WIN_SIZE_X;
	float ry = rHandPos.y / CROPBOX_HEIGHT * GL_WIN_SIZE_Y;
	float rz = (1 - rHandPos.z / CROPBOX_DEPTH) * 100;

	int leftHandColor = 8; // blue
	int rightHandColor = 7; // red

	// Draw a white square as the background for jitter tracking
	DrawPoint(350, 150, 100, 6);
	//DrawPoint(500, 150, 100, 6);

	// Draw head point
	float hx = headPos.x / CROPBOX_WIDTH * GL_WIN_SIZE_X;
	float hy = headPos.y / CROPBOX_HEIGHT * GL_WIN_SIZE_Y;
	float hz = (1 - headPos.z / CROPBOX_DEPTH) * 100; 
	DrawPoint(hx,hy,40,1);  //Green

	// Draw hand points
	DrawPoint(lx,ly,20,leftHandColor);
	DrawPoint(rx,ry,20,rightHandColor);

	// Draw indication of gesture event
	if (gesture == "click")
	{
		DrawPoint(500,150,50,3);
	}
	else if (gesture == "wave")
	{
		DrawPoint(200,150,50,3);
	}

	// Swap the OpenGL display buffers
	glutSwapBuffers();
}

GLuint Viewer::InitTexture(void** buf, int& width, int& height)
{
	GLuint texID = 0;
	glGenTextures(1,&texID);

	width = GetClosestPowerOfTwo(width);
	height = GetClosestPowerOfTwo(height); 
	*buf = new unsigned char[width*height*4];
	glBindTexture(GL_TEXTURE_2D,texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texID;
}

void Viewer::DrawRectangle(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
	GLfloat verts[8] = {	topLeftX, topLeftY,
		topLeftX, bottomRightY,
		bottomRightX, bottomRightY,
		bottomRightX, topLeftY
	};
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glFlush();
}

void Viewer::DrawTexture(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

	DrawRectangle(topLeftX, topLeftY, bottomRightX, bottomRightY);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Viewer::DrawPoint(float x, float y, int thickness, int colorIndex)
{
	static bool bInitialized = FALSE;
	static GLuint depthTexID;
	static unsigned char* pDepthTexBuf;
	static int texWidth, texHeight;
	float texXpos;
	float texYpos;

	if (!bInitialized)
	{
		bInitialized = TRUE;
		int nXRes = 640;
		int nYRes = 480;

		texWidth =  GetClosestPowerOfTwo(nXRes);
		texHeight = GetClosestPowerOfTwo(nYRes);
		depthTexID = InitTexture((void**)&pDepthTexBuf,texWidth, texHeight) ;
		texXpos =(float)nXRes/texWidth;
		texYpos  =(float)nYRes/texHeight;

		memset(texcoords, 0, 8*sizeof(float));
		texcoords[0] = texXpos, texcoords[1] = texYpos, texcoords[2] = texXpos, texcoords[7] = texYpos;
	}

	glBindTexture(GL_TEXTURE_2D, depthTexID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pDepthTexBuf);

	// Display the OpenGL texture map
	glColor4f(Colors[colorIndex][0],Colors[colorIndex][1],Colors[colorIndex][2],1);

	glEnable(GL_TEXTURE_2D);
	DrawTexture(x,y,x+thickness,y+thickness);	
	glDisable(GL_TEXTURE_2D);
}

unsigned int Viewer::GetClosestPowerOfTwo(unsigned int n)
{
	unsigned int m = 2;
	while(m < n) m<<=1;

	return m;
}