#ifndef INITWORKER_H
#define INITWORKER_H

#include <QObject>
#include <queue>
#include "NIServer.h"

class ServerWorker : public QObject
{
    Q_OBJECT

public:
    enum Mission
    {
        ToCheckLicense,
        ToStartNIService,
		ToStopNIService,
        ToInit,
        ToEnd,
        DoNothing
    };

public:
    explicit ServerWorker(QObject *parent = 0);
    ~ServerWorker();

signals:
    void initialized();
    void licenseChecked(QString stat);
    void niServiceStateChanged(bool);
    void ended();
    void error(QString err);

public slots:
    void process();
    void CheckLicense();
    void StartNIService();
	void StopNIService();
    void Init(); // check license and start NI service
    void End();

private:
    QString ConvertLicenseState(NIServer::license_State stat);

private:
	std::queue<Mission> _missions;
};

#endif // INITWORKER_H
