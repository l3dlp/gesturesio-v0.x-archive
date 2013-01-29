#include "initworker.h"

InitWorker::InitWorker(QObject *parent) :
    QObject(parent)
{
    curMission = Idle;
}

InitWorker::~InitWorker()
{

}

QString InitWorker::ConvertLicenseState(NIServer::license_State stat)
{
    QString license;

    // Convert license message
    switch(stat)
    {
    case NIServer::LICENSE_VALID:
        license = "Lincense is valid,starting NIEngine...";
        break;

    case NIServer::LICENSE_INVALID:
        license = "License is invalid.";
        break;

    case NIServer::LICENSE_TIMELIMITED:
        license = "TimeLimitedLicense";
        break;

    case NIServer::LICENSE_UNKNOWN:
    default:
        license = "Failed to validate license.";
        break;
    }

    return license;
}

void InitWorker::process()
{
    bool res = false;
    QString license;
    NIServer::license_State stat;
    bool shouldRun = true;

    qDebug("worker running..");

    while(shouldRun)
    {
        if(curMission != Idle)
        {
            qDebug("new command comes");
        }

        switch(curMission)
        {
        case ToInit:
            // Check license
            stat = NIServer::CheckLicense();
            license = ConvertLicenseState(stat);
            emit licenseChecked(license); // Send license message

            // Start Engine
            if(stat == NIServer::LICENSE_VALID || stat == NIServer::LICENSE_TIMELIMITED)
            {
               res = NIServer::StartNIService();

               if(res)
               {
                   emit initFinished();
               }
               else
               {
                   emit error(QString("Failed to launch engine"));
               }
            }
            curMission = Idle;
            break;

        case ToCheckLicense:
            stat = NIServer::CheckLicense();
            license = ConvertLicenseState(stat);
            emit licenseChecked(license); // Send license message
            curMission = Idle;
            break;

        case ToStartNIService:
            res = NIServer::StartNIService();
            if(res)
            {
                emit engineFinished();
            }
            else
            {
                emit error(QString("Failed to start NI service"));
            }
            curMission = Idle;
            break;

        case ToEnd:
            NIServer::StopNIService();
            curMission = Idle;
            shouldRun = false;
            break;

        default:
            break;
        }
    }
    qDebug("worker ended");
    emit ended();
}

void InitWorker::CheckLicense()
{
    // Ideally we should push coming task to a pipeline,here we just
    // simply ignore the command when there is a working task.
    if(curMission == Idle)
    {
        curMission = ToCheckLicense;
    }
}

void InitWorker::StartNIService()
{
   if(curMission == Idle)
   {
       curMission = ToStartNIService;
   }
}

void InitWorker::Init()
{
    if(curMission == Idle)
    {
        curMission = ToInit;
    }
}

void InitWorker::End()
{
    if(curMission == Idle)
    {
        curMission = ToEnd;
    }
}
