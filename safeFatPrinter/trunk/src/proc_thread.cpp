#include "proc_thread.h"


ProcessT::ProcessT( QObject *parent ) :
	QThread( parent )
{
	m_ChanMode = QProcess::SeparateChannels;
}

ProcessT::~ProcessT() {

    wait();
    qDebug() << Q_FUNC_INFO << "Thread destroyed";
}

void ProcessT::run() {


	if ( m_Command.isEmpty() ) {
		qWarning() << Q_FUNC_INFO << "No command set, doing nothing";
		return;
	}

	qDebug() << Q_FUNC_INFO << "Executing command" << m_Command << m_Args << "\n";

	QProcess proc;

	proc.setProcessChannelMode( m_ChanMode );
	proc.start( m_Command, m_Args );
	if (!proc.waitForStarted()) {
	    qDebug()<< QString("Unable to launch application %1").arg(m_Command);
	}else{
	    proc.waitForFinished(-1);
	    proc.closeWriteChannel();
	    m_Output = proc.readAll().trimmed();
	    qDebug() << QString("Exit code %1").arg(proc.exitCode());

	    emit commandOutput(proc.exitCode(), m_Output );
	    qDebug() << Q_FUNC_INFO << "Command execution finished.\n" <<m_Output;
	}
}

void ProcessT::setCommand( const QString &name, const QStringList &args, const QProcess::ProcessChannelMode &mode ) {

	if ( name.isEmpty() ) {
		qWarning() << Q_FUNC_INFO << "Empty command given, doing nothing";
		return;
	}
	m_Command = name;
	m_Args = args;
	m_ChanMode = mode;
}

void ProcessT::execute( const QString &name, const QStringList &args, const QProcess::ProcessChannelMode &mode ) {

	setCommand( name, args, mode );
	
	if ( !isRunning() ) {
		start();
	}
	else {
		qWarning() << Q_FUNC_INFO << "Thread already running, doing nothing.";
		return;
	}

}

