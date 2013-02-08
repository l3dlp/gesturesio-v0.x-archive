#ifndef INITWORKER_H
#define INITWORKER_H

#include <QObject>
#include "NIServer.h"

class ServerWorker : public QObject
{
    Q_OBJECT

public:
    enum Mission
    {
        ToCheckLicense,
        ToStartNIService,
        ToInit,
        ToEnd,
        Idle
    };

public:
    explicit ServerWorker(QObject *parent = 0);
    ~ServerWorker();

signals:
    void initFinished();
    void licenseChecked(QString stat);
    void engineFinished();
    void ended();
    void error(QString err);

public slots:
    void process();
    void CheckLicense();
    void StartNIService();
    void Init(); // check license and start NI service
    void End();

private:
    QString ConvertLicenseState(NIServer::license_State stat);

private:
    Mission curMission;
};

#endif // INITWORKER_H
