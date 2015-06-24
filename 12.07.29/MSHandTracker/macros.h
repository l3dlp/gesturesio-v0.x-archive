#ifndef MS_MACROS
#define MS_MACROS

#include <XnCppWrapper.h>
#define MAX_CHAR_SIZE 1024

#define MS_DEBUG_MSG(args) printf("[DEBUG at %s:%d] : ", __FUNCTION__ ,__LINE__);printf(args);printf("\n");

#define OpenNIErrorCheck(err,what) if(err != XN_STATUS_OK){MS_DEBUG_MSG(what) MS_DEBUG_MSG(	xnGetStatusString(err))}

//#define OpenNIErrorCheck(err,what) if(err != XN_STATUS_OK){MS_DEBUG_MSG("failed %s, error message : %s",what,xnGetStatusString(err))}

#endif // MS_MACROS