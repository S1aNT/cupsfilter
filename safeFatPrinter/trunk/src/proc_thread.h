#ifndef PROC_THREAD_H
#define PROC_THREAD_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QtDebug> 


/**
* 
* 
* @short Class which can execute a command as a seperate thread.
* 
*/

class ProcessT : public QThread {

	Q_OBJECT

	public:
		/**
		 * Constructor. Creats a new ProcessT object.
		 * @param parent The QObject parent of this object.
		 */
		ProcessT( QObject *parent = 0 );
		/**
		 * Destructor.
		 */
		virtual ~ProcessT();
		/**
		 * Reimplementation of QThread::run().
		 * Executes the command which was set using setCommand(). 
		 * If no command was set, this function does nothing.
		 */
		void run();
		/**
		 * Set the given command to be executed when run() is called.
		 * @param name The name of the program.
		 * @param args The command line arguments.
		 * @param mode The process channel modes of the command which will be executed.
		 */
		void setCommand( const QString &name, const QStringList &args, const QProcess::ProcessChannelMode &mode = QProcess::SeparateChannels );
		/**
		 * Execute the given command. Calls setCommand() first and then just starts the thread if it's not running.
		 * @param name The name of the program.
		 * @param args The command line arguments.
		 * @param mode The process channel modes of the command which will be executed.
		 */
		void execute( const QString &name, const QStringList &args, const QProcess::ProcessChannelMode &mode = QProcess::SeparateChannels );

	signals:
		/**
		 * Emitted when a command has finished running. 
		 * @param output The output of the command which was executed.
		 */
		void commandOutput( QString output );
		void proc_finished(int, QProcess::ExitStatus);
		
	private:
		QString m_Command;
		QStringList m_Args;
		QProcess::ProcessChannelMode m_ChanMode;
		QString m_Output;

};

#endif //PROC_THREAD_H
