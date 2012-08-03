#include <XnCppWrapper.h>

class NIEngine
{
private:
	static NIEngine* _instance;
	xn::Context _niContext;
	xn::ScriptNode _niScriptNode;
	xn::DepthGenerator _depthGenerator;
	xn::UserGenerator _userGenerator;
	XnBool _shouldStop;
	XnBool _running;

public:
	static NIEngine* GetInstance();
	XnBool Start();
	XnBool Stop();
	void NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
	void LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
	void CalibrationCompleted(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie);

	~NIEngine();

private:
	NIEngine();
	XnBool FileExists(const char *fn);
	static void StartThread(void* arg);
	void ProcessData();
};

typedef struct  
{
	NIEngine* _this;

}THREADSTRUCT;