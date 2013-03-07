#ifndef INITWORKER_H
#define INITWORKER_H

#include <QObject>
#include <queue>
#include "NIServer.h"

class ServerWorker : public QObject
{
    Q_OBJECT

public:
    explicit ServerWorker(QObject *parent = 0);
    ~ServerWorker();

signals:
    void licenseChecked(QString stat);
	void threadRunning();

public slots:
	/// Worker thread process to handle time consuming tasks like checking license,
	/// and to hold NIServer's thread object, which cannot be released before its process ends.
    void process();  

private:
    QString ConvertLicenseState(NIServer::license_State stat);
};

#endif // INITWORKER_H
