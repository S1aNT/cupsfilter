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

	qDebug() << Q_FUNC_INFO << "Executing command" << m_Command << m_Args << "...";
	QProcess proc;
	connect(proc,
			SIGNAL(finished(int, QProcess::ExitStatus)),
		this,
			SIGNAL(proc_finished(int, QProcess::ExitStatus))
	);

	proc.setProcessChannelMode( m_ChanMode );
	proc.start( m_Command, m_Args );
	proc.waitForStarted();
	proc.waitForFinished();
	proc.closeWriteChannel();
	
	m_Output = proc.readAll().trimmed();

	emit commandOutput( m_Output );
	qDebug() << Q_FUNC_INFO << "Command execution finished.\n" <<m_Output;

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

