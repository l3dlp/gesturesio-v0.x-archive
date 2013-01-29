#include "initworker.h"

InitWorker::InitWorker(QObject *parent) :
    QObject(parent)
{
}

InitWorker::~InitWorker()
{

}

void InitWorker::process()
{
    bool res = false;
    QString license;

    NIServer::license_State stat = NIServer::CheckLicense();

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
    // Send license message
    emit licenseFinished(license);

    if(stat == NIServer::LICENSE_VALID || stat == NIServer::LICENSE_TIMELIMITED)
    {
       res = NIServer::StartNIService();
    }

    if(res)
    {
        emit finished();
    }
    else
    {
        emit error(QString("Failed to start service"));
    }
}
