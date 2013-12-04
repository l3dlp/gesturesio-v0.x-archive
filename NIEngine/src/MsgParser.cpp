#include "MsgParser.h"

using namespace std;

#define MAX_PACKET  1024
#define BUFFER_SIZE ((MAX_PACKET)*4)  // sizeof(int)=4

MsgParser::MsgParser()
{

}

MsgParser::~MsgParser()
{

}

void MsgParser::ReadNIInfo()
{
	
	niInfo.LeftHandPos = NIEngine::GetInstance()->GetLeftHandPosProjective();
	niInfo.RightHandPos = NIEngine::GetInstance()->GetRightHandPosProjective();
	niInfo.HeadPos = NIEngine::GetInstance()->GetHeadPosProjective();

	niInfo.LeftHandPos3D = NIEngine::GetInstance()->GetLeftHandPos();
	niInfo.RightHandPos3D = NIEngine::GetInstance()->GetRightHandPos();
	niInfo.HeadPos3D = NIEngine::GetInstance()->GetHeadPos();

	// Retrieve gesture
	GestureInfo gesture = NIEngine::GetInstance()->GetGesture();

	niInfo.LeftHandGestureName = "NONE";
	niInfo.RightHandGestureName = "NONE";

	if (gesture.handType == LEFT_HAND)
	{
		niInfo.LeftHandGestureName = gesture.name;
	}

	if (gesture.handType == RIGHT_HAND)
	{
		niInfo.RightHandGestureName = gesture.name;
	}
}

string MsgParser::Parse(string msg)
{
    char data[BUFFER_SIZE];

    memset(&data,'\0',BUFFER_SIZE);

    if(msg.compare("getCoordsT") == 0)
    {
		ReadNIInfo();

        //printf("%f %f %f %f %f %f\n",leftHandPos.X,leftHandPos.Y,leftHandPos.Z,rightHandPos.X,rightHandPos.Y,rightHandPos.Z);

        sprintf_s(data,"%s %f %f %f %s %f %f %f %f %f %f %s",
						niInfo.LeftHandGestureName.c_str(),niInfo.LeftHandPos.x,niInfo.LeftHandPos.y,niInfo.LeftHandPos.z,
						niInfo.RightHandGestureName.c_str(),niInfo.RightHandPos.x,niInfo.RightHandPos.y,niInfo.RightHandPos.z,
						niInfo.HeadPos.x,niInfo.HeadPos.y,niInfo.HeadPos.z);
    }

    if (msg.compare("getCoordsX") == 0)
    {
		ReadNIInfo();

        int length = 0;

        length += sprintf(data + length,"<coords>");
		if (NIEngine::GetInstance()->TrackingActiveUser())
		{
			length += sprintf(data + length,"\n");
			
			length += sprintf(data + length,"<leftHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"/>\n",
				niInfo.LeftHandGestureName.c_str(),niInfo.LeftHandPos.x,niInfo.LeftHandPos.y,niInfo.LeftHandPos.z);

			length += sprintf(data + length,"<rightHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"/>\n",
				niInfo.RightHandGestureName.c_str(),niInfo.RightHandPos.x,niInfo.RightHandPos.y,niInfo.RightHandPos.z);

			length += sprintf(data + length,"<head x=\"%f\" y=\"%f\" z=\"%f\"/>\n",niInfo.HeadPos.x,niInfo.HeadPos.y,niInfo.HeadPos.z);
		}
        sprintf(data + length,"</coords>");
    }

	if (msg.compare("get3dX") == 0)
	{
		ReadNIInfo();

		int length = 0;

		length += sprintf(data + length,"<coords>");
		if (NIEngine::GetInstance()->TrackingActiveUser())
		{
			length += sprintf(data + length,"\n");
			
			length += sprintf(data + length,"<leftHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"/>\n",
				niInfo.LeftHandGestureName.c_str(),niInfo.LeftHandPos3D.x,niInfo.LeftHandPos3D.y,niInfo.LeftHandPos3D.z);

			length += sprintf(data + length,"<rightHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"/>\n",
				niInfo.RightHandGestureName.c_str(),niInfo.RightHandPos3D.x,niInfo.RightHandPos3D.y,niInfo.RightHandPos3D.z);

			length += sprintf(data + length,"<head x=\"%f\" y=\"%f\" z=\"%f\"/>\n",niInfo.HeadPos3D.x,niInfo.HeadPos3D.y,niInfo.HeadPos3D.z);
		}
		sprintf(data + length,"</coords>");
	}

    if (msg.compare("getCoordsJ") == 0)
    {
		ReadNIInfo();

        int length = 0;
        length += sprintf_s(data,"[");
        length += sprintf(data + length,"\"coords\",{");
        
		length += sprintf(data + length,"\"leftHandX\":\"%f\",\"leftHandY\":\"%f\",\"leftHandZ\":\"%f\",",
			niInfo.LeftHandPos.x,niInfo.LeftHandPos.y,niInfo.LeftHandPos.z);

        length += sprintf(data + length,"\"rightHandX\":\"%f\",\"rightHandY\":\"%f\",\"rightHandZ\":\"%f\",",
			niInfo.RightHandPos.x,niInfo.RightHandPos.y,niInfo.RightHandPos.z);

        length += sprintf(data + length,"\"headPosX\":\"%f\",\"headPosY\":\"%f\",\"headPosZ\":\"%f\",",
			niInfo.HeadPos.x,niInfo.HeadPos.y,niInfo.HeadPos.z);

        sprintf(data + length,"}]");
    }

	return data;
}
