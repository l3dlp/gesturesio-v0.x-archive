#include "ServerWorker.h"
#include "NIEngine.h"
#include "tinyThread/tinythread.h"
#include "Utils.h"

ServerWorker::ServerWorker(QObject *parent) :
    QObject(parent)
{
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

	Logger::GetInstance()->Log("Server worker thread process running..");

	// Create a dedicated thread to handle data reading.
	// The reason to do it here is to keep the thread reference before thread is joint.
	// Ideally the worker should not know NIEngine except NIServer...
	THREADSTRUCT* param = new THREADSTRUCT;
	param->_this = NIEngine::GetInstance();
	tthread::thread niThread(NIEngine::StartThread,param);
	niThread.detach();

	bool shouldRun = true;

    while(shouldRun)
    {
		Mission curMission = DoNothing;

		if (_missions.size() > 0)
		{
			curMission = _missions.front();
			_missions.pop();
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
                   emit initialized();
               }
               else
               {
                   emit error(QString("Failed to launch engine"));
               }
            }
            break;

        case ToCheckLicense:
            stat = NIServer::CheckLicense();
            license = ConvertLicenseState(stat);
            emit licenseChecked(license); // Send license message
            break;

        case ToStartNIService:
            res = NIServer::StartNIService();
            if(res)
            {
                emit niServiceStateChanged(true);
            }
            else
            {
                emit error(QString("Failed to start NI service"));
            }
            break;

		case ToStopNIService:
			res = NIServer::StopNIService();
			if (res)
			{
				niServiceStateChanged(false);
			} 
			else
			{
				emit error(QString("Failed to stop NI service"));
			}
			break;

        case ToEnd:
			res = NIServer::StopNIService();
			if (res)
			{
				shouldRun = false;
				emit ended();
			}
            break;

        default:
            break;
        }
    }

	Logger::GetInstance()->Log("Server worker thread process ends");
}

void ServerWorker::CheckLicense()
{
	Mission cmd = ToCheckLicense;
	_missions.push(cmd);
}

void ServerWorker::StartNIService()
{
   Mission cmd = ToStartNIService;
   _missions.push(cmd);
}

void ServerWorker::StopNIService()
{
	Mission cmd = ToStopNIService;
	_missions.push(cmd);
}

void ServerWorker::Init()
{
	Mission cmd = ToInit;
	_missions.push(cmd);
}

void ServerWorker::End()
{
	Mission cmd = ToEnd;
	_missions.push(cmd);
}
