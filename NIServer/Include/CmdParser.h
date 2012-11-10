#ifndef CMDPARSER_H
#define CMDPARSER_H

#include <QObject>
#include <QString>

class CmdParser : public QObject
{
    Q_OBJECT
public:
    CmdParser(QObject* parent = 0);
    ~CmdParser();

public slots:
    void parse(QString cmd);

signals:
    void parsed(QString);
};

#endif // CMDPARSER_H
