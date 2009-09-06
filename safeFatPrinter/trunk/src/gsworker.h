#ifndef GSWORKER_H
#define GSWORKER_H
#include <QDebug>
#include <QObject>
#include <QStringList>
#include <QString>
#include <QProcess>
#include <QTemporaryFile>
#include <QUuid>
#include <stdio.h>
#include "proc_thread.h"



class gsWorker : public QObject
{
	Q_OBJECT
	Q_PROPERTY(void setUserName WRITE setUserName)
	Q_PROPERTY(void setDocName WRITE setDocName)

public:
    gsWorker(QStringList &argList);
    ~gsWorker();

    void  setUserName() const;
    void setDocName() const;
    void convert2PDF();

public slots:
    void converDone(int Code, QProcess::ExitStatus Status );
signals:
    void error(int codeError, const QString &message);

private:
    QString userName;
    QString docName;
    QUuid SID;
    QProcess *process;
    QString gsFile;
    QString rspFile;
    QString tempPDF;
    enum exitCode
    {
	normal=0,
	needMoreArg=500
    };

};

#endif // GSWORKER_H
