#ifndef MSGPARSER_H
#define MSGPARSER_H

#include "NIEngine.h"

using namespace std;

class MsgParser
{
	struct NIInfo
	{
		nite::Point3f LeftHandPos;
		nite::Point3f RightHandPos;
		nite::Point3f HeadPos;

		nite::Point3f LeftHandPos3D;
		nite::Point3f RightHandPos3D;
		nite::Point3f HeadPos3D;

		string LeftHandGestureName;
		string RightHandGestureName;
	};

public:
    MsgParser();
    ~MsgParser();

	string Parse(string cmd);

private:
	NIInfo niInfo;
	void ReadNIInfo();
};

#endif // MSGPARSER_H