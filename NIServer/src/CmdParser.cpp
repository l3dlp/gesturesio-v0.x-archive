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

    qDebug(cmd.toAscii());

    memset(&data,0,BUFFER_SIZE);

    XnPoint3D leftHandPos = NIEngine::GetInstance()->GetLeftHandPosProjective();
    XnPoint3D rightHandPos = NIEngine::GetInstance()->GetRightHandPosProjective();
    XnPoint3D headPos = NIEngine::GetInstance()->GetHeadPosProjective();

    // Retrieve gesture
    std::string leftGesture = "NONE";
    std::string rightGesture = "NONE";

    GESTURERECORD gestureRec = NIEngine::GetInstance()->GetGesture();
    if (gestureRec.name != "") 		// TODO: check timestamp and give up outdate gestures
    {
        //printf("valid gesture %s %d\n",gestureRec.name.c_str(),gestureRec.isRightHand);

        if (gestureRec.isRightHand == true)
        {
            rightGesture = gestureRec.name;
        }
        else
        {
            leftGesture = gestureRec.name;
        }
    }

    if(cmd.compare("getCoordsT") == 0)
    {
        //printf("%f %f %f %f %f %f\n",leftHandPos.X,leftHandPos.Y,leftHandPos.Z,rightHandPos.X,rightHandPos.Y,rightHandPos.Z);

        sprintf_s(data,"%f %f %f %f %f %f %f %f %f %s %s",leftHandPos.X,leftHandPos.Y,leftHandPos.Z,
            rightHandPos.X,rightHandPos.Y,rightHandPos.Z,headPos.X,headPos.Y,headPos.Z,leftGesture.c_str(),rightGesture.c_str());

    }

    if (cmd.compare("getCoordsX") == 0)
    {
        int length = 0;
        length += sprintf_s(data,"<?xml version=\"1.0\" encoding=\"utf-8\">");
        length += sprintf(data + length,"<coords>");
        length += sprintf(data + length,"<leftHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"></leftHand>",leftGesture.c_str(),leftHandPos.X,leftHandPos.Y,leftHandPos.Z);
        length += sprintf(data + length,"<rightHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"></rightHand>",rightGesture.c_str(),rightHandPos.X,rightHandPos.Y,rightHandPos.Z);
        length += sprintf(data + length,"<headPos x=\"%f\" y=\"%f\" z=\"%f\"></headPos>",headPos.X,headPos.Y,headPos.Z);
        sprintf(data + length,"</coords>");
    }

    if (cmd.compare("getCoordsJ") == 0)
    {
        int length = 0;
        length += sprintf_s(data,"[");
        length += sprintf(data + length,"\"coords\",{");
        length += sprintf(data + length,"\"leftHandX\":\"%f\",\"leftHandY\":\"%f\",\"leftHandZ\":\"%f\",",leftHandPos.X,leftHandPos.Y,leftHandPos.Z);
        length += sprintf(data + length,"\"rightHandX\":\"%f\",\"rightHandY\":\"%f\",\"rightHandZ\":\"%f\",",rightHandPos.X,rightHandPos.Y,rightHandPos.Z);
        length += sprintf(data + length,"\"headPosX\":\"%f\",\"headPosY\":\"%f\",\"headPosZ\":\"%f\",",headPos.X,headPos.Y,headPos.Z);
        sprintf(data + length,"}]");
    }

    emit parsed(QString(data));
}
