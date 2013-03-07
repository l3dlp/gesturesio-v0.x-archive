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
	Logger::GetInstance()->Log("Server worker thread process running..");

	// Check license
	NIServer::license_State stat = NIServer::CheckLicense();
	QString license = ConvertLicenseState(stat);
	Logger::GetInstance()->Log("License state: " + license.toStdString());
	emit licenseChecked(license); // Report license message

	if (stat == NIServer::LICENSE_VALID || stat == NIServer::LICENSE_TIMELIMITED)
	{
		// Create a dedicated data reading thread. We will wait until it ends.
		// We do this in worker thread to avoid blocking main thread(the UI).
		// Ideally the worker should not know NIEngine except NIServer, for now we put it here.
		THREADSTRUCT* param = new THREADSTRUCT;
		param->_this = NIEngine::GetInstance();
		tthread::thread niThread(NIEngine::MainProc,param);
		emit threadRunning();
		niThread.join();
	}

	Logger::GetInstance()->Log("Server worker thread process ends");
}
