#include "GL/glut.h"

class Viewer
{
public:
	Viewer();
	virtual~ Viewer();
	virtual bool Init(int argc, char **argv);
	virtual bool Run();
protected:
	virtual void Display();
	virtual void OnKey(unsigned char key, int x, int y);
	virtual void InitOpenGL(int argc, char** argv);
	void InitOpenGLHooks();
	void DrawPoint(float x, float y, int thickness, int colorIndex);

private:
	static void glutIdle();
	static void glutDisplay();
	static void glutKeyboard(unsigned char key, int x, int y);

	GLuint InitTexture(void** buf, int& width, int& height);
	void DrawTexture(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY);
	void DrawRectangle(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY);
	unsigned int GetClosestPowerOfTwo(unsigned int n);

private:
	static Viewer* ms_self;
	GLfloat texcoords[8];
};

class SkeletonViewer:Viewer
{

};