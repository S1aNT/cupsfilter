#include "proc_thread.h"

#include <QtCore/QtDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/QRegExp>

#if defined(Q_OS_WIN)
#include <QTextStream>
#include <QTemporaryFile>
#include <QFileInfo>
#endif

ProcessT::ProcessT( QObject *parent,const QString &jobKey)
        : QThread( parent )
{
    m_ChanMode = QProcess::SeparateChannels;
    job_key = jobKey;
}

ProcessT::~ProcessT()
{
    wait();
    qDebug() << Q_FUNC_INFO << "Thread destroyed";
}




void ProcessT::run()
{
    qDebug() << Q_FUNC_INFO << "Executing command" << m_Command << m_Args << "\n";
    if ( m_Command.isEmpty() ) {
        qWarning() << Q_FUNC_INFO << "No command set, doing nothing";
        return;
    }

    QProcess proc;
    // Add  to the environment
    if (!n_Env.isEmpty()){
        proc.setEnvironment(n_Env);
    }

    proc.setProcessChannelMode( m_ChanMode );

#if defined(Q_OS_UNIX)
    qDebug() << "m_Args\n" << m_Args << "\n";
    proc.start( m_Command, m_Args );
#elif defined(Q_OS_WIN)    
    QFile t_file;
    QString tmp_cmd;
    {
        // Генерация уникального имени файла
        QTemporaryFile tmp_file;// Создадим временный файл
        tmp_file.setFileTemplate("XXXXXXXX.bat");
        if (tmp_file.open()){
            QFileInfo f_info(tmp_file);
            tmp_cmd = f_info.absoluteFilePath();
            tmp_file.close();
            t_file.setFileName(tmp_cmd);
        }else{
            emit jobFinish(job_key,-1,
                           QObject::trUtf8("Ошибка создания временного файла %1\n%2")
                           .arg( tmp_cmd )
                           .arg(QString(Q_FUNC_INFO))
                           );
            return;
        }
    }
    QTextStream out(&t_file);
    if (t_file.open(QIODevice::WriteOnly)){
        out << QObject::tr("@echo off\n");
        if (m_Command.contains(" ") ){
            out << QObject::tr("\"%1\" ").arg(m_Command);
        }else{
            out << QObject::tr("%1 ").arg(m_Command);
        }
        for (int i=0;i<m_Args.size();i++){
            out << m_Args.at(i) << " ";
        }
        t_file.close();
        proc.start( tmp_cmd );
    }else{
        emit jobFinish(job_key,-1,
                       QObject::trUtf8("Ошибка записи данных во временный файл %1\n%2")
                       .arg( tmp_cmd )
                       .arg(QString(Q_FUNC_INFO))
                       );
        return;
    }
#endif
    if (!proc.waitForStarted(10000)) {
        m_Output =QString("Ошибка при запуске приложения %1").arg(m_Command);
        emit jobFinish(job_key,-1, m_Output );
        qDebug()<< m_Output << " QProcess::error() " << proc.error();
    }else{
        proc.waitForFinished(-1);
        proc.closeWriteChannel();
        m_Output = proc.readAll();//.trimmed();
        qDebug() << Q_FUNC_INFO << "m_Output " << m_Output << "\n";
        emit jobFinish(job_key,proc.exitCode(), m_Output );
    }
#if defined(Q_OS_WIN)
    t_file.remove();
#endif
}

void ProcessT::addToEnv(const QStringList & pairs_list )
{
    n_Env = QProcess::systemEnvironment();
    n_Env << pairs_list;
}

void ProcessT::setCommand( const QString &name, const QStringList &args,
                           const QProcess::ProcessChannelMode &mode )
{
    if ( name.isEmpty() ) {
        qWarning() << Q_FUNC_INFO << "Empty command given, doing nothing";
        return;
    }
    m_Command = name;
    m_Args = args;
    m_ChanMode = mode;
}

void ProcessT::execute( const QString &name, const QStringList &args,
                        const QProcess::ProcessChannelMode &mode )
{
    setCommand( name, args, mode );

    if ( !isRunning() ) {
        start();
    }
    else {
        qWarning() << Q_FUNC_INFO << "Thread already running, doing nothing.";
        return;
    }
}

