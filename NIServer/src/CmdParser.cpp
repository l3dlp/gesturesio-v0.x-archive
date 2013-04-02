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

    // Retrieve gesture
    std::string gesture = NIEngine::GetInstance()->GetGesture();

    if(cmd.compare("getCoordsT") == 0)
    {
        //printf("%f %f %f %f %f %f\n",leftHandPos.X,leftHandPos.Y,leftHandPos.Z,rightHandPos.X,rightHandPos.Y,rightHandPos.Z);

        sprintf_s(data,"%f %f %f %f %f %f %f %f %f %s",leftHandPos.x,leftHandPos.y,leftHandPos.z,
            rightHandPos.x,rightHandPos.y,rightHandPos.z,headPos.x,headPos.y,headPos.z,gesture.c_str());
    }

    if (cmd.compare("getCoordsX") == 0)
    {
        int length = 0;
        length += sprintf_s(data,"<?xml version=\"1.0\" encoding=\"utf-8\">\n");
        length += sprintf(data + length,"<coords>\n");
        length += sprintf(data + length,"<leftHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"/>\n",gesture.c_str(),leftHandPos.x,leftHandPos.y,leftHandPos.z);
        length += sprintf(data + length,"<rightHand a=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\"/>\n",gesture.c_str(),rightHandPos.x,rightHandPos.y,rightHandPos.z);
        length += sprintf(data + length,"<headPos x=\"%f\" y=\"%f\" z=\"%f\"/>\n",headPos.x,headPos.y,headPos.z);
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
