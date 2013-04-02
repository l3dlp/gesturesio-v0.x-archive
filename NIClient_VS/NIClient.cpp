#include "clsockets/ActiveSocket.h"
#include <sstream>
#include <vector>

#ifdef USE_GLUT
#include "GL/glut.h"
//#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
//#include <GLUT/glut.h>
//#else
//#include <GL/glut.h>
//#endif
#elif defined(USE_GLES)
#include "opengles.h"
#endif

#define MAX_PACKET 1024

#define GL_WIN_SIZE_X 800
#define GL_WIN_SIZE_Y 600
int g_xres = 640;
int g_yres = 480;

float leftHandPosX = 0;
float leftHandPosY = 0;
float leftHandPosZ = 0;
float rightHandPosX = 0;
float rightHandPosY = 0;
float rightHandPosZ = 0;

float headPosX = 0;
float headPosY = 0;
float headPosZ = 0;

std::string leftHandgesture;
std::string rightHandgesture;

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

CActiveSocket niSocket; // Instantiate active socket object (defaults to TCP).
char data[MAX_PACKET];

std::vector<std::string> parse(std::string l)
{
	std::istringstream stm(l);
	std::vector<std::string> tokens;
	for (;;)
	{
		std::string word;
		if (!(stm >> word))break;
		tokens.push_back(word);
	}
	return tokens;
}

const int CROPBOX_WIDTH = 400; // mm
const int CROPBOX_HEIGHT = 300; // mm
const int CROPBOX_DEPTH = 3500; // mm

int g_dataFormat = 0;

bool Query()
{
	memset(&data,0,MAX_PACKET);

	niSocket.Initialize();

	// To see what format to ask the server
	int dataFormat = g_dataFormat;

	if (niSocket.Open((const UINT8*)"localhost",6789))
	{
		//printf("Socket open.\n");
		int bytesSent;
		if(dataFormat == 1)
		{
			bytesSent = niSocket.Send((const uint8*)"getCoordsX",10);
		}
		else if(dataFormat == 2)
		{
			bytesSent = niSocket.Send((const uint8*)"getCoordsJ",10);
		}
		else
		{
			bytesSent = niSocket.Send((const uint8*)"getCoordsT",10);
		}

		if(bytesSent == 10)
		{
			niSocket.Receive(MAX_PACKET-1);
			int count = niSocket.GetBytesReceived();
			memcpy(&data,niSocket.GetData(),count);

			//printf("echo: %d %s\n",niSocket.GetBytesReceived(),data);

			// parse the data according to the format we ask
			if (dataFormat == 1)
			{
				// xml
			}
			else if (dataFormat == 2)
			{
				// json
			}
			else
			{
				// plaint text parser
				std::string s = data;
				std::vector<std::string> positions = parse(s);
				std::vector<std::string>::iterator it;
				it = positions.begin();
				leftHandgesture = (*it).c_str();	
				it++;
				leftHandPosX = atof((*it).c_str());
				it++;
				leftHandPosY = atof((*it).c_str());
				it++;
				leftHandPosZ = atof((*it).c_str());
				it++;
				rightHandgesture = (*it).c_str();	
				it++;
				rightHandPosX = atof((*it).c_str());
				it++;
				rightHandPosY = atof((*it).c_str());
				it++;
				rightHandPosZ = atof((*it).c_str());
				it++;
				headPosX = atof((*it).c_str());
				it++;
				headPosY = atof((*it).c_str());
				it++;
				headPosZ = atof((*it).c_str());
			}
		}

		niSocket.Close();
	}

	return TRUE;
}

void DrawRectangle(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
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

GLfloat texcoords[8];
void DrawTexture(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

	DrawRectangle(topLeftX, topLeftY, bottomRightX, bottomRightY);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

unsigned int getClosestPowerOfTwo(unsigned int n)
{
	unsigned int m = 2;
	while(m < n) m<<=1;

	return m;
}

GLuint initTexture(void** buf, int& width, int& height)
{
	GLuint texID = 0;
	glGenTextures(1,&texID);

	width = getClosestPowerOfTwo(width);
	height = getClosestPowerOfTwo(height); 
	*buf = new unsigned char[width*height*4];
	glBindTexture(GL_TEXTURE_2D,texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texID;
}

void DrawPoint(float x, float y, int thickness, int colorIndex)
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
		uint16 nXRes = g_xres;
		uint16 nYRes = g_yres;

		texWidth =  getClosestPowerOfTwo(nXRes);
		texHeight = getClosestPowerOfTwo(nYRes);
		depthTexID = initTexture((void**)&pDepthTexBuf,texWidth, texHeight) ;
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

void UpdateHands()
{
	int x = rand();
	leftHandPosX = x % GL_WIN_SIZE_X;
	int y = rand();
	leftHandPosY = y % GL_WIN_SIZE_Y;
	x = rand();
	rightHandPosX = x % GL_WIN_SIZE_Y;
	y = rand();
	rightHandPosY = y % g_yres;
	x = rand();
	leftHandPosZ = x % 10;
	y = rand();
	rightHandPosZ = y % 10;
}

bool g_bQuit = false;
bool g_bQueryInLoop = false;

// This function is called each frame
void glutDisplay (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the OpenGL viewpoint
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

#ifdef USE_GLUT
	glOrtho(0, 800, 600, 0, -1.0, 1.0);
#elif defined(USE_GLES)
	glOrthof(0, 800, 600, 0, -1.0, 1.0);
#endif

	glDisable(GL_TEXTURE_2D);

#ifdef USE_GLUT
	//printf("hpx %f hpy %f \n",leftHandPosX,leftHandPosY);

	// Map points
	float lx = leftHandPosX / CROPBOX_WIDTH * GL_WIN_SIZE_X;
	float ly = leftHandPosY / CROPBOX_HEIGHT * GL_WIN_SIZE_Y;
	float lz = (1 - leftHandPosZ / CROPBOX_DEPTH) * 100;
	float rx = rightHandPosX / CROPBOX_WIDTH * GL_WIN_SIZE_X;
	float ry = rightHandPosY / CROPBOX_HEIGHT * GL_WIN_SIZE_Y;
	float rz = (1 - rightHandPosZ / CROPBOX_DEPTH) * 100;

	int leftHandColor = 8; // blue
	int rightHandColor = 7; // red
	
	// Draw a white square as the background for jitter tracking
	DrawPoint(350, 150, 100, 6);
	//DrawPoint(500, 150, 100, 6);

	// Draw head point
	float hx = headPosX / CROPBOX_WIDTH * GL_WIN_SIZE_X;
	float hy = headPosY / CROPBOX_HEIGHT * GL_WIN_SIZE_Y;
	float hz = (1 - headPosZ / CROPBOX_DEPTH) * 100; 
	DrawPoint(hx,hy,40,1);  //Green

	// Draw hand points
	DrawPoint(lx,ly,20,leftHandColor);
	DrawPoint(rx,ry,20,rightHandColor);

	// Draw indication of gesture event
	if (leftHandgesture != "NONE")
	{
		if (leftHandgesture == "click")
		{
			DrawPoint(500,150,50,3);
		}
		else if (leftHandgesture == "wave")
		{
			DrawPoint(200,150,50,3);
		}
	}

	if (rightHandgesture != "NONE")
	{
		if (rightHandgesture == "click")
		{
			DrawPoint(500,150,50,5);
		}
		else if (rightHandgesture == "wave")
		{
			DrawPoint(200,150,50,5);
		}
	}


#endif

#ifdef USE_GLUT
	glutSwapBuffers();
#endif
}

#ifdef USE_GLUT

void glutIdle (void)
{
	if (g_bQuit) 
	{
		exit(1);
	}

	if (g_bQueryInLoop)
	{
		Query();
	}

	// Display the frame
	glutPostRedisplay();
}

void glutKeyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		// Exit
	case 'x':
		niSocket.Close();
		printf("Socket closed.\n");
		exit(1);
		break;
	case 'f':
		g_dataFormat += 1;
		g_dataFormat %= 3;// support 3 formats: plain text, xml, json. this command is to switch between them.
		break;
	case'q':
		Query(); 	// Single inquire
		break;
	case 'l':
		g_bQueryInLoop = !g_bQueryInLoop;
		break;
	case 'd':
		UpdateHands(); // debug: update hands
		break;
	default:
		break;
	}
}

void glInit (int * pargc, char ** argv)
{
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	glutCreateWindow ("NISocket Client");
	//glutFullScreen();
	//glutSetCursor(GLUT_CURSOR_NONE);

	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

#endif

int main(int argc, char** argv)
{
	printf("Usage:\n q - Query\n l - Query in loop\n f - switch data format\n x - exit\n");

#ifdef USE_GLUT
	glInit(&argc, argv);
	glutMainLoop();
#endif

	return 1;
}
