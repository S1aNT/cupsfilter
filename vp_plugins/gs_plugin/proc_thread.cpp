#include "proc_thread.h"

#include <QtCore/QtDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/QRegExp>

#if defined(Q_OS_WIN)
#include <QTextStream>
#include <QTemporaryFile>
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
    qDebug() << "m_Args\n" << m_Args << "\n";
    proc.setProcessChannelMode( m_ChanMode );
#if defined(Q_OS_UNIX)
    proc.start( m_Command, m_Args );
#elif defined(Q_OS_WIN)
    // Создадим временный файл
    QTemporaryFile t_file;

    t_file.setAutoRemove(false);
    t_file.setFileTemplate("XXXXXXXX.bat");
    QTextStream out(&t_file);
    if (t_file.open()){
        QString tmp_cmd = t_file.fileName();
        out << tr("@echo off\n");
        if (m_Command.contains(" ") ){
            out << tr("\"%1\" ").arg(m_Command);
        }else{
            out << tr("%1 ").arg(m_Command);
        }
        for (int i=0;i<m_Args.size();i++){
            out << m_Args.at(i) << " ";
        }
        t_file.close();

        proc.start( tmp_cmd );
#endif
        if (!proc.waitForStarted()) {
            m_Output =QString("Ошибка при запуске приложения %1").arg(m_Command);
            emit jobFinish(job_key,-1, m_Output );
            qDebug()<< m_Output;
        }else{
            proc.waitForFinished(-1);
            proc.closeWriteChannel();
            m_Output = proc.readAll();//.trimmed();
            qDebug() << Q_FUNC_INFO << "m_Output " << m_Output << "\n";
            emit jobFinish(job_key,proc.exitCode(), m_Output );
        }
    }else{
        emit jobFinish(job_key,-1,
                       QObject::trUtf8("Ошибка создания временного файла %1\n%2")
                       .arg( t_file.fileName())
                       .arg(QString(Q_FUNC_INFO))
                       );
        qDebug() << QString("Cmd %1 with arg: ").arg(m_Command) << m_Args;

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

