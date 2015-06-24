/*// Headers for OpenNI
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>

// Header for NITE
#include "XnVNite.h"
// local header
#include "PointDrawer.h"

#define CHECK_RC(OpenNIErrorCheck, what)											\
	if (OpenNIErrorCheck != XN_STATUS_OK)											\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(OpenNIErrorCheck));		\
		return OpenNIErrorCheck;													\
	}

#define CHECK_ERRORS(OpenNIErrorCheck, errors, what)		\
	if (OpenNIErrorCheck == XN_STATUS_NO_NODE_PRESENT)	\
	{										\
		XnChar strError[1024];				\
		errors.ToString(strError, 1024);	\
		printf("%s\n", strError);			\
		return (OpenNIErrorCheck);						\
	}


#include <GL/glut.h>
//#include "signal_catch.h"

// OpenNI objects
xn::Context g_Context;
xn::ScriptNode g_ScriptNode;
xn::DepthGenerator g_DepthGenerator;
xn::HandsGenerator g_HandsGenerator;
xn::GestureGenerator g_GestureGenerator;

// Draw the depth map?
XnBool g_bDrawDepthMap = true;
XnBool g_bPrintFrameID = false;
// Use smoothing?
XnFloat g_fSmoothing = 0.0f;
XnBool g_bPause = false;
XnBool g_bQuit = false;

SessionState g_SessionState = NOT_IN_SESSION;

// NITE objects
XnVSessionManager* g_pSessionManager;
XnVFlowRouter* g_pFlowRouter;

// the drawer
XnVPointDrawer* g_pDrawer;

#define GL_WIN_SIZE_X 720
#define GL_WIN_SIZE_Y 480

void CleanupExit()
{
	g_ScriptNode.Release();
	g_DepthGenerator.Release();
	g_HandsGenerator.Release();
	g_GestureGenerator.Release();
	g_Context.Release();

	exit (1);
}

void glutDisplay (void)
{

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the OpenGL viewpoint
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	XnMapOutputMode mode;
	g_DepthGenerator.GetMapOutputMode(mode);
	glOrtho(0, mode.nXRes, mode.nYRes, 0, -1.0, 1.0);


	glDisable(GL_TEXTURE_2D);

	if (!g_bPause)
	{
		// Read next available data
		g_Context.WaitOneUpdateAll(g_DepthGenerator);
		// Update NITE tree
		g_pSessionManager->Update(&g_Context);

		PrintSessionState(g_SessionState);

	}


	glutSwapBuffers();
}


void glutIdle (void)
{
	if (g_bQuit) {
		CleanupExit();
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
		CleanupExit();
	case'p':
		// Toggle pause
		g_bPause = !g_bPause;
		break;
	case 'd':
		// Toggle drawing of the depth map
		g_bDrawDepthMap = !g_bDrawDepthMap;
		g_pDrawer->SetDepthMap(g_bDrawDepthMap);
		break;
	case 'f':
		g_bPrintFrameID = !g_bPrintFrameID;
		g_pDrawer->SetFrameID(g_bPrintFrameID);
		break;
	case 's':
		// Toggle smoothing
		if (g_fSmoothing == 0)
			g_fSmoothing = 0.1;
		else 
			g_fSmoothing = 0;
		g_HandsGenerator.SetSmoothing(g_fSmoothing);
		break;
	case 'e':
		// end current session
		g_pSessionManager->EndSession();
		break;
	}
}
void glInit (int * pargc, char ** argv)
{
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	glutCreateWindow ("PrimeSense Nite Point Viewer");
	//glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

int main()
{
	XnStatus OpenNIErrorCheck = XN_STATUS_OK;
	OpenNIErrorCheck = g_Context.Init();
	CHECK_RC(OpenNIErrorCheck, "context init");

	OpenNIErrorCheck = g_DepthGenerator.Create(g_Context);
	CHECK_RC(OpenNIErrorCheck, "depth generator creation");

	OpenNIErrorCheck = g_DepthGenerator.Create(g_Context);
	CHECK_RC(OpenNIErrorCheck, "depth generator creation");

	OpenNIErrorCheck = g_DepthGenerator.Create(g_Context);
	CHECK_RC(OpenNIErrorCheck, "depth generator creation");




}*/

#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include "macros.h"

// Header for NITE
#include <XnVNite.h>

typedef enum
{
	IN_SESSION,
	NOT_IN_SESSION,
	QUICK_REFOCUS
} SessionState;

SessionState g_SessionState = NOT_IN_SESSION;
bool gRun;
XnPoint3D gPos;


void XN_CALLBACK_TYPE GestureIntermediateStageCompletedHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Intermediate stage complete (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureReadyForNextIntermediateStageHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Ready for next intermediate stage (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureProgressHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie)
{
	printf("Gesture %s progress: %f (%f,%f,%f)\n", strGesture, fProgress, pPosition->X, pPosition->Y, pPosition->Z);
}

void XN_CALLBACK_TYPE SessionStarting(const XnPoint3D& ptPosition, void* UserCxt)
{
	printf("Session start: (%f,%f,%f)\n", ptPosition.X, ptPosition.Y, ptPosition.Z);
	g_SessionState = IN_SESSION;
}
// Callback for session end
void XN_CALLBACK_TYPE SessionEnding(void* UserCxt)
{
	printf("Session end\n");
	g_SessionState = NOT_IN_SESSION;
}
void XN_CALLBACK_TYPE NoHands(void* UserCxt)
{
	if (g_SessionState != NOT_IN_SESSION)
	{
		printf("Quick refocus\n");
		g_SessionState = QUICK_REFOCUS;
	}
}

void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt)
{
	printf("Focus progress: %s @(%f,%f,%f): %f\n", strFocus, ptPosition.X, ptPosition.Y, ptPosition.Z, fProgress);
}

void XN_CALLBACK_TYPE OnPointUpdated(const XnVHandPointContext* pContext, void* cxt)
{
	gPos.X = pContext->ptPosition.X;
	gPos.Y = pContext->ptPosition.Y;
	gPos.Z = pContext->ptPosition.Z;
}


int main()
{
	XnStatus mOpenNIChecker = XN_STATUS_OK;
	xn::Context mContext;
	mOpenNIChecker = mContext.Init();
	OpenNIErrorCheck(mOpenNIChecker,"Context initialisation");

	xn::DepthGenerator mDepthGenerator;
	mOpenNIChecker = mDepthGenerator.Create(mContext);
	OpenNIErrorCheck(mOpenNIChecker,"depth generator creation");

	xn::HandsGenerator mHandGenerator;
	mOpenNIChecker = mHandGenerator.Create(mContext);
	OpenNIErrorCheck(mOpenNIChecker,"hand generator creation");

	xn::UserGenerator mUserGenerator;
	mOpenNIChecker = mUserGenerator.Create(mContext);
	OpenNIErrorCheck(mOpenNIChecker, "User generator creation");

	xn::GestureGenerator mGestureGenerator;
	mOpenNIChecker = mGestureGenerator.Create(mContext);
	OpenNIErrorCheck(mOpenNIChecker, "Gesture generator creation");

	XnCallbackHandle hGestureIntermediateStageCompleted, hGestureProgress, hGestureReadyForNextIntermediateStage;
	

	mOpenNIChecker = mGestureGenerator.RegisterToGestureIntermediateStageCompleted(GestureIntermediateStageCompletedHandler, NULL, hGestureIntermediateStageCompleted);
	OpenNIErrorCheck(mOpenNIChecker, "Gesture intermediate completed registeration");

	mOpenNIChecker = mGestureGenerator.RegisterToGestureReadyForNextIntermediateStage(GestureReadyForNextIntermediateStageHandler, NULL, hGestureReadyForNextIntermediateStage);
	OpenNIErrorCheck(mOpenNIChecker, "Gesture ready for next intermediate stage registeration");

	mOpenNIChecker = mGestureGenerator.RegisterGestureCallbacks(NULL, GestureProgressHandler, NULL, hGestureProgress);
	OpenNIErrorCheck(mOpenNIChecker, "Gesture callback registeration");

	// NITE objects
	XnVSessionManager* mSessionManager;
	XnVFlowRouter* mFlowRouter;

	// Create NITE objects
	mSessionManager = new XnVSessionManager();
	mOpenNIChecker = mSessionManager->Initialize(&mContext, "Click,Wave", "RaiseHand");
	OpenNIErrorCheck(mOpenNIChecker, "SessionManager::Initialize");

	mSessionManager->RegisterSession(NULL, SessionStarting, SessionEnding, FocusProgress);

	mFlowRouter = new XnVFlowRouter();
	XnVPointControl *mPointControl = new XnVPointControl;

	mFlowRouter->SetActive(mPointControl);
	
	mSessionManager->AddListener(mFlowRouter);

	mPointControl->RegisterPrimaryPointUpdate(&mContext, OnPointUpdated);

	mOpenNIChecker = mContext.StartGeneratingAll();
	OpenNIErrorCheck(mOpenNIChecker, "Start generating all");

	gRun = true;
	int count = 0;
	while((mOpenNIChecker = mContext.WaitOneUpdateAll(mDepthGenerator)) == XN_STATUS_OK && gRun)
	{
		mSessionManager->Update(&mContext);
		if(g_SessionState == IN_SESSION && count%10 == 0)
		{
			
			printf("IN SESSION - %g %g %g\n", gPos.X, gPos.Y, gPos.Z);
		}
		count++;
	}

	OpenNIErrorCheck(mOpenNIChecker, "Waiting depth and updating all");

	delete mSessionManager;
	mSessionManager = NULL;
	
	mDepthGenerator.Release();
	mHandGenerator.Release();

	mContext.Release();

	return 0;
}