#include "initworker.h"
#include "NIEngine.h"
#include "tinyThread/tinythread.h"

ServerWorker::ServerWorker(QObject *parent) :
    QObject(parent)
{
    curMission = Idle;
}

ServerWorker::~ServerWorker()
{

}

QString ServerWorker::ConvertLicenseState(NIServer::license_State stat)
{
    QString license;

    // Convert license message
    switch(stat)
    {
    case NIServer::LICENSE_VALID:
        license = "License is valid,starting NIEngine...";
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

void ServerWorker::process()
{
    bool res = false;
    QString license;
    NIServer::license_State stat;
    bool shouldRun = true;

    qDebug("worker running..");

	// Create a dedicated thread to handle data reading.
	// The reason to do it here is to keep the thread reference before thread is joint.
	// Ideally the worker should not know NIEngine except NIServer...
	THREADSTRUCT* param = new THREADSTRUCT;
	param->_this = NIEngine::GetInstance();
	tthread::thread niThread(NIEngine::StartThread,param);
	niThread.detach();

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
			qDebug("Signal to end...");
            NIServer::SignalToStopNIService();
            curMission = Idle;
            break;

        default:
            break;
        }

		if (NIServer::CanStopNIService() == true)
		{
			NIServer::StopNIService();
			shouldRun = false;
			qDebug("NIService really ended.");
		}
    }
    qDebug("worker ended");
    emit ended();
}

void ServerWorker::CheckLicense()
{
    // Ideally we should push coming task to a pipeline,here we just
    // simply ignore the command when there is a working task.
    if(curMission == Idle)
    {
        curMission = ToCheckLicense;
    }
}

void ServerWorker::StartNIService()
{
   if(curMission == Idle)
   {
       curMission = ToStartNIService;
   }
}

void ServerWorker::Init()
{
    if(curMission == Idle)
    {
        curMission = ToInit;
    }
}

void ServerWorker::End()
{
    if(curMission == Idle)
    {
        curMission = ToEnd;
    }
}
