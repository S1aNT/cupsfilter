#include "spyder.h"

Spider::Spider(QObject *parent)
    : QObject(parent)
{
    connect(&ftp, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
    connect(&ftp, SIGNAL(listInfo(const QUrlInfo &)),
            this, SLOT(ftpListInfo(const QUrlInfo &)));
}


bool Spider::getDirectory(const QUrl &url,const QString &saveTo)
{
    if (!url.isValid()) {
	qDebug() << "Error: Invalid URL" << endl;
        return false;
    }
    if (url.scheme() != "ftp") {
	qDebug() << "Error: URL must start with 'ftp:'" << endl;
        return false;
    }
    ftp.connectToHost(url.host(), url.port(21));
    ftp.login();
    QString path = url.path();
    if (path.isEmpty())
        path = "/";
    pendingDirs.append(path);
    currentLocalDir=saveTo;
    processNextDirectory();
    return true;
}

void Spider::processNextDirectory()
{
    if (!pendingDirs.isEmpty()) {
        //currentDir = pendingDirs.takeFirst();
        //currentLocalDir = "downloads/" + currentDir;
        //QDir(".").mkpath(currentLocalDir);

	//QDir.mkpath(currentLocalDir);
        ftp.cd(currentDir);
        ftp.list();
    } else {
        emit done();
    }
}

void Spider::ftpListInfo(const QUrlInfo &urlInfo)
{
    if (urlInfo.isFile()) {
        if (urlInfo.isReadable()) {
            QFile *file = new QFile(currentLocalDir + "/"
                                    + urlInfo.name());
            if (!file->open(QIODevice::WriteOnly)) {
		qDebug() << "Warning: Cannot open file "
                     << qPrintable(
                            QDir::convertSeparators(file->fileName()))
                     << endl;
                return;
            }
            ftp.get(urlInfo.name(), file);
            openedFiles.append(file);
        }
    }/* else if (urlInfo.isDir() && !urlInfo.isSymLink()) {
        pendingDirs.append(currentDir + "/" + urlInfo.name());
    }  */
}

void Spider::ftpDone(bool error)
{
    if (error) {
	qDebug() << "Error: " << qPrintable(ftp.errorString()) << endl;
    } else {
	qDebug() << "Downloaded " << qPrintable(currentDir) << " to "
             << qPrintable(QDir::convertSeparators(
                               QDir(currentLocalDir).canonicalPath()));
    }
    qDeleteAll(openedFiles);
    openedFiles.clear();
    processNextDirectory();
}
