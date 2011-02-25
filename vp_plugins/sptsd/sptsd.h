#ifndef SPTSD_H
#define SPTSD_H

#include "common.h"

#include <qsocket.h>
#include <qsqldatabase.h>
#include <qsqldriver.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qregexp.h>
#include <qdatetime.h>
#include <qprocess.h>
#include <qwaitcondition.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
    
#include <./cups/cups.h>
#include <./cups/string.h>
#include <./cups/i18n.h>


QString readSettings(QString key);

class sptsd : public QSocket
{
    Q_OBJECT

public:
    sptsd(int sock, QObject *parent=0, const char *name=0);
    ~sptsd(){}
    
    QString uid, args[20], printers[100], seclevelNum, numSeclevels[4], seclevelStr, strSeclevels[4], surdCmd, userCommand;	
    QString getSeclevelStr(QString seclevelNum);
    QString getSeclevelNum(QString seclevelStr);
    QFile	fileToPrint;
    Q_INT32	packetSize, arraySize, commandType, answerType, answerSize;

signals:
    void writeLog(const QString&);
    void packetIsRead();    

private slots:
    void list_options(cups_dest_t *dest);
    void list_group(ppd_file_t  *ppd, ppd_group_t *group);
    void printFile();
    void readSocketData();
    void handleUserCommand();
    void initDB(QSqlDatabase *DB);
    void surdConnected();
    void surdReadyRead();
    void surdSendRequest();
    void readLpstatOutput();
    void surdDisconnected();
    void surdSocketClosed();
    void showHex(unsigned char *chrBuff, int len, int showTextBelow);
    void asc2ebc(unsigned char *addr, int len);
    void ebc2asc(unsigned char *addr, int len);

private:
    int	line, command, cmdCounter;
    QByteArray	packet, compressedPacket;
    QSocket	surdSocket;
    QProcess	*lpProc;
//    QSqlDatabase	sqliteDB;
};
#endif

