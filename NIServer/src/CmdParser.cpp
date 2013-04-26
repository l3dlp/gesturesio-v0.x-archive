#include "NIEngine.h"
#include "CmdParser.h"

using namespace std;

#define MAX_PACKET  1024
#define BUFFER_SIZE ((MAX_PACKET)*4)  // sizeof(int)=4

CmdParser::CmdParser(QObject *parent): QObject(parent)
{

}

CmdParser::~CmdParser()
{

}

void CmdParser::parse(QString cmd)
{
    char data[BUFFER_SIZE];

    //qDebug(cmd.toAscii());

    memset(&data,'\0',BUFFER_SIZE);

    nite::Point3f leftHandPos = NIEngine::GetInstance()->GetLeftHandPosProjective();
    nite::Point3f rightHandPos = NIEngine::GetInstance()->GetRightHandPosProjective();
    nite::Point3f headPos = NIEngine::GetInstance()->GetHeadPosProjective();

	nite::Point3f leftHandPos3D = NIEngine::GetInstance()->GetLeftHandPos();
	nite::Point3f rightHandPos3D = NIEngine::GetInstance()->GetRightHandPos();
	nite::Point3f headPos3D = NIEngine::GetInstance()->GetHeadPos();
	
    // Retrieve gesture
    GestureInfo gesture = NIEngine::GetInstance()->GetGesture();

	string leftHand = "NONE";
	string rightHand = "NONE";

	if (gesture.handType == LEFT_HAND)
	{
		leftHand = gesture.name;
	}

	if (gesture.handType == RIGHT_HAND)
	{
		rightHand = gesture.name;
	}

    if(cmd.compare("getCoordsT") == 0)
    {
        //printf("%f %f %f %f %f %f\n",leftHandPos.X,leftHandPos.Y,leftHandPos.Z,rightHandPos.X,rightHandPos.Y,rightHandPos.Z);

        sprintf_s(data,"%s %f %f %f %s %f %f %f %f %f %f %s",leftHand.c_str(),leftHandPos.x,leftHandPos.y,leftHandPos.z,
															 rightHand.c_str(),rightHandPos.x,rightHandPos.y,rightHandPos.z,
															 headPos.x,headPos.y,headPos.z);
    }

    if (cmd.compare("getCoordsX") == 0)
    {
        int length = 0;

        length += sprintf(data + length,"<coords>");
		if (NIEngine::GetInstance()->TrackingActiveUser())
		{
			length += sprintf(data + length,"\n");
			length += sprintf(data + length,"<leftHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"/>\n",leftHand.c_str(),leftHandPos.x,leftHandPos.y,leftHandPos.z);
			length += sprintf(data + length,"<rightHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"/>\n",rightHand.c_str(),rightHandPos.x,rightHandPos.y,rightHandPos.z);
			length += sprintf(data + length,"<head x=\"%f\" y=\"%f\" z=\"%f\"/>\n",headPos.x,headPos.y,headPos.z);
		}
        sprintf(data + length,"</coords>");
    }

	if (cmd.compare("get3dX") == 0)
	{
		int length = 0;

		length += sprintf(data + length,"<coords>");
		if (NIEngine::GetInstance()->TrackingActiveUser())
		{
			length += sprintf(data + length,"\n");
			length += sprintf(data + length,"<leftHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"/>\n",leftHand.c_str(),leftHandPos3D.x,leftHandPos3D.y,leftHandPos3D.z);
			length += sprintf(data + length,"<rightHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"/>\n",rightHand.c_str(),rightHandPos3D.x,rightHandPos3D.y,rightHandPos3D.z);
			length += sprintf(data + length,"<head x=\"%f\" y=\"%f\" z=\"%f\"/>\n",headPos3D.x,headPos3D.y,headPos3D.z);
		}
		sprintf(data + length,"</coords>");
	}

    if (cmd.compare("getCoordsJ") == 0)
    {
        int length = 0;
        length += sprintf_s(data,"[");
        length += sprintf(data + length,"\"coords\",{");
        length += sprintf(data + length,"\"leftHandX\":\"%f\",\"leftHandY\":\"%f\",\"leftHandZ\":\"%f\",",leftHandPos.x,leftHandPos.y,leftHandPos.z);
        length += sprintf(data + length,"\"rightHandX\":\"%f\",\"rightHandY\":\"%f\",\"rightHandZ\":\"%f\",",rightHandPos.x,rightHandPos.y,rightHandPos.z);
        length += sprintf(data + length,"\"headPosX\":\"%f\",\"headPosY\":\"%f\",\"headPosZ\":\"%f\",",headPos.x,headPos.y,headPos.z);
        sprintf(data + length,"}]");
    }

    emit parsed(QString(data));
}
