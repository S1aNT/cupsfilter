#ifndef THREADEXECUTOR_H
#define THREADEXECUTOR_H
#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QtDebug>
#include <QSettings>

#include "proc_thread.h"

class ThreadExecutor : public ProcessT
{
    Q_OBJECT
public:
		/**
		 * Constructor.
		 */
		ThreadExecutor( QObject *parent = 0 );
		/**
		 * Destructor.
		 */
		virtual ~ThreadExecutor();
		/**
		 * Set a input  file. This file will be used to read Poscrcript Data.
		 * @param file The path to the input file.
		 */
		void setInputFile( const QString &file );
		/**
		 * Set a document title/
		 * @param file The path to the input file.
		 */
		void setDocTitle( const QString &doc_title );
		/**
		 * Execute a command
		 * @param command QStringList containing the command to be executed.
		 * @param detached Execute the command either detached or start it normally and wait for it to be finished.
		 */
		void execute( const QStringList &command );
private:
		QString m_DocTitle;
		QString m_InputFile;
};

#endif // THREADEXECUTOR_H
