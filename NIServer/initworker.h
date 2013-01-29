#ifndef INITWORKER_H
#define INITWORKER_H

#include <QObject>
#include "NIServer.h"

class InitWorker : public QObject
{
    Q_OBJECT
public:
    explicit InitWorker(QObject *parent = 0);
    ~InitWorker();

signals:
    void finished();
    void licenseFinished(QString stat);
    void NIServerFinished();
    void error(QString err);

public slots:
    void process();
};

#endif // INITWORKER_H
