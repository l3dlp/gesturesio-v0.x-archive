#include "ServerWorker.h"
#include "NIEngine.h"
#include "tinyThread/tinythread.h"
#include "Utils.h"

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
        license = "Valid license";
        break;

    case NIServer::LICENSE_INVALID:
        license = "Invalid license";
        break;

    case NIServer::LICENSE_TIMELIMITED:
        license = "TimeLimited license";
        break;

    case NIServer::LICENSE_UNKNOWN:
    default:
        license = "Unknown license.";
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

	Logger::GetInstance()->Log("Server worker thread process running..");

	// Create a dedicated thread to handle data reading.
	// The reason to do it here is to keep the thread reference before thread is joint.
	// Ideally the worker should not know NIEngine except NIServer...
	THREADSTRUCT* param = new THREADSTRUCT;
	param->_this = NIEngine::GetInstance();
	tthread::thread niThread(NIEngine::StartThread,param);
	niThread.detach();

	bool endCmdIssued = false;

    while(shouldRun)
    {
		if (endCmdIssued == true && NIServer::IsNIRunning() == false)
		{
			shouldRun = false;
		}

        switch(curMission)
        {
        case ToInit:
            // Check license
            stat = NIServer::CheckLicense();
            license = ConvertLicenseState(stat);
			Logger::GetInstance()->Log("License state: " + license.toStdString());
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
			endCmdIssued = true;
			curMission = Idle;
            break;

        default:
            break;
        }
    }

    qDebug("worker ended");
	Logger::GetInstance()->Log("Server worker thread process ends");

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
