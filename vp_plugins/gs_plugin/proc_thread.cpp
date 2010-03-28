#include "proc_thread.h"
#include <QtDebug>
#include <QString>
#include <QStringList>
#include <QProcess>



ProcessT::ProcessT( QObject *parent,VPrn::Jobs job_id,const QString &client)
    : QThread( parent )
{
    m_JobId    = job_id;
    client_id  = client;
    m_ChanMode = QProcess::SeparateChannels;
}

ProcessT::~ProcessT()
{
    wait();
    qDebug() << Q_FUNC_INFO << "Thread destroyed";
}

void ProcessT::run()
{
    if ( m_Command.isEmpty() ) {
        qWarning() << Q_FUNC_INFO << "No command set, doing nothing";
        return;
    }

    qDebug() << Q_FUNC_INFO << "Executing command" << m_Command << m_Args << "\n";

    QProcess proc;
    // Add  to the environment
    if (!n_Env.isEmpty()){
        proc.setEnvironment(n_Env);
    }

    proc.setProcessChannelMode( m_ChanMode );
    proc.start( m_Command, m_Args );
    if (!proc.waitForStarted()) {
        m_Output =QString("Ошибка при запуске приложения %1").arg(m_Command);
        emit jobFinish(client_id,m_JobId,-1, m_Output );
        qDebug()<< m_Output;
    }else{
        proc.waitForFinished(-1);
        proc.closeWriteChannel();
        m_Output = proc.readAll().trimmed();
        qDebug() << QString("Exit code %1").arg(proc.exitCode());

        //emit commandOutput(proc.exitCode(), m_Output );
        emit jobFinish(client_id,m_JobId,proc.exitCode(), m_Output );
        qDebug() << Q_FUNC_INFO << "Command execution finished,"<<m_Output
                << " for job " << m_JobId;
    }
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

