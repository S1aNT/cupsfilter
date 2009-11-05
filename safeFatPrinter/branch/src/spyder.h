#ifndef FTP_LOADER_H
#define FTP_LOADER_H

#include <QDebug>
#include <QObject>
#include <QFtp>
#include <QFile>
#include <QDir>
#include <QList>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QUrlInfo>

class Spider : public QObject
{
    Q_OBJECT
public:
    Spider(QObject *parent = 0);
    bool getDirectory(const QUrl &url,const QString &saveTo);
signals:
    void done();
private slots:
    void ftpDone(bool error);
    void ftpListInfo(const QUrlInfo &urlInfo);
private:
    void processNextDirectory();
    QFtp ftp;
    QList<QFile *> openedFiles;
    QString currentDir;
    QString currentLocalDir;
    QStringList pendingDirs;
};
#endif 
