
#include "sptsd.h"
 
sptsd::sptsd( int socket, QObject *parent, const char *name )
     :QSocket( parent, name )
{
	QTime time;
        line = 0;
        connect(this, SIGNAL(readyRead()), SLOT(readSocketData()));
        connect(this, SIGNAL(connectionClosed()), SLOT(deleteLater()));
        setSocket(socket);
	packetSize = 0;
	fileToPrint.setName("./print_jobs/" + time.currentTime().toString() + "_print_job");

        connect(&surdSocket,SIGNAL(readyRead()), SLOT(surdReadyRead()));
	connect(&surdSocket, SIGNAL(connected()), SLOT(surdConnected()));
//	connect(&surdSocket, SIGNAL(connectionClosed()), SLOT(surdDisconnected()));
//	connect(lpProc, SIGNAL(processExited()), this, SLOT(readLpstatOutput()) );
							
}

				    
void sptsd::readSocketData()
{
    QDataStream	socketStream(this);
    QDataStream packetStream(packet, IO_ReadOnly);    
    QDataStream comressedStream(compressedPacket, IO_ReadOnly);

    qDebug("=========================================================================================");
	 
    if (packetSize == 0){	        
	qDebug(QString("%1").arg(bytesAvailable()) + " bytes available to read from socket");
	socketStream >> packetSize;
	qDebug("Data size is " + QString("%1").arg(packetSize) + " bytes"); 
	socketStream >> commandType;
	qDebug("CommandType is " + QString("%1").arg(commandType));
    }							    
    
    if (this->bytesAvailable() < (packetSize - sizeof(packetSize)))
	return;

    qDebug("Saving  the rest of data...");

    if (commandType == 6000) {
	socketStream >> compressedPacket;	
	qDebug("Compressed size is " + QString("%1").arg(compressedPacket.size()) + " bytes");
        packet = qUncompress(compressedPacket);
	qDebug("Uncompressed size is " + QString("%1").arg(packet.size()) + " bytes");
        QFile f("./recieved");
	f.open(IO_WriteOnly);
	f.writeBlock(packet.data(), packet.size());
	f.close();
	printFile();
    }
    else {
	socketStream >> packet;
//	qDebug("packet array size %d", packet.size());
	userCommand = packet.data();
        handleUserCommand();
    }
    packetSize = 0;
    return;
}


void sptsd::printFile()
{
    int           job_id;                 /* Job ID */
    int           num_options;            /* Number of options */
    cups_option_t *options;               /* Options */
		 
    num_options = 0;
    options     = NULL;
    fileToPrint.setName("./recieved");			 
    qDebug("Printing file: %s", fileToPrint.name().ascii());
    job_id = cupsPrintFile("Cups-PDF", fileToPrint.name().ascii(), fileToPrint.name().ascii(), num_options, options);

    if (job_id < 1)
	qDebug(QString::fromUtf8(cupsLastErrorString()));
    return;
}


void sptsd::handleUserCommand() 
{
//    QDataStream answerStream(packet, IO_WriteOnly);
    Q_UINT16 surdPort = readSettings("/surd/surd_port").toInt();
    QString surdHost = readSettings("/surd/surd_host"); 
    QString answerString;
    QTime time = QTime::currentTime(); 
    int pos = 0, index = 0, next = 0;
    bool	queryFound = true;
    QByteArray	answerArray;
    QDataStream	clientStream(this);

//    ts.setEncoding(QTextStream::UnicodeUTF8);

    
    qDebug("Checking for requested operation in string: " + QString::fromUtf8(userCommand));
//    qDebug("String length is %d", userCommand.length());
    
//    pos = userCommand.find("[", pos) + 1;
//    next = userCommand.find("]", pos);
//    uid = userCommand.mid(pos, next - pos);
//    qDebug("UID = " + QString("%1").arg(uid));
    
//    pos = userCommand.find(";:;", pos) + 3;
    
    
    while ((next = userCommand.find(";:;", pos)) != -1)  {
        args[index] = userCommand.mid(pos, next - pos);
	qDebug("arg[" + QString("%1").arg(index) + "] = " + args[index]);
        index ++;
        pos = next + 3;
    }
    args[index] = userCommand.right(userCommand.length() - pos);
    uid = args[0];
    
    switch (commandType) {
	case 10:
	    qDebug("Found initial /me request. UID: " + uid);
	    qDebug("Wrote: 1010" + uid + ";:;");
//	    qDebug("=========================================================================================");
	    answerString = uid + ";:;";
	    answerArray = answerString.utf8();
	    answerType = 1010;
	    answerSize = sizeof(answerSize) + sizeof(answerType) + answerArray.size();
	    clientStream << answerSize << answerType << answerArray;
	    this-> flush();
	    break;
	case 50:
	    qDebug("Cups list options request");
	    cups_dest_t *dest, *dests;
	    char *instance, *printer;
	    int num_dests;
	    printer = args[1];
	    list_options(dest);
	    break;
	case 100:     					
	    qDebug("Found 'author'  request ...");	
	    surdCmd = "author " + args[1] + " " + args[2] + " " + args[3];
	    answerType = 1100;
	    break;
	case 101:					
	    qDebug("Found 'prtsec' request ...");	
	    surdCmd = "prtsec " + args[1]; 
	    break;					
	case 300:	
	    qDebug("Уровень секретности из мандата");
	    answerType = 1300;
	    surdCmd = "lseclev " + args[1]; 
	    break;
	case 400:					
	    qDebug("Found 'lprtusr' request ..."); 	
	    answerType = 1400;
	    surdCmd = "lprtusr " + args[2] + " " + args[1]; 
	    break;
	case 500:
	    qDebug("Номера МБ за период " + args[1] + "-" + args[2]);
	    answerType = 1500;
	    break;
	case 570:
	    answerType = 1570;
	    break;
	case 600:
	    qDebug("Found 'lsecusr' request ...");
	    answerType = 1600; 	
	    surdCmd = "lsecusr " + args[1]; 
	    break;
	case 800:
	    qDebug("Found 'authorx' request ..."); 	
	    break;
	case 200:	// INSERT query
	    qDebug("Found SQL request ...");
//	    sqliteQuery.prepare("INSERT INTO films (code, title, kind) VALUES (:code, :title, :kind);");
//	    sqliteQuery.bindValue(":code", 101);
//	    sqliteQuery.bindValue(":title", "film");
//	    sqliteQuery.bindValue(":kind", "Drama");
	    break;
	case 201:	// SELECT query
	    qDebug("Found 'sql select' request ...");
	    break;
	case 202:	//  /202;:;uid;:;sql select query;:;columns
	    qDebug("Found 'raw sql select' request:" + args[1]);
	    break;
	case 203:	//  /203;:;uid;:;sql insert query
	    qDebug("Found 'raw sql insert' request:" + args[1]);
	    break;
	case 6000:
	    fileToPrint.open(IO_WriteOnly);
    	    fileToPrint.flush();
    	    fileToPrint.writeBlock(packet);
    	    fileToPrint.close();
	    printFile();
	    break;
	default:
	    qDebug("Processing command: " + userCommand);
	    queryFound = false;
	    break;
    }
    
// Поправить, убрать 500 команду	    
    if ((commandType == 400) or (commandType == 100) or (commandType == 101) or (commandType == 300) 
			     or (commandType == 600) or (commandType == 800) or (commandType == 500) or (commandType == 570))  {  
	if (surdSocket.state() == QSocket::Idle) {
 	 surdSocket.connectToHost(surdHost, surdPort);
	 qDebug("Connecting to SURD (" + surdHost + ":%d)", surdPort);
	 return;
	}
	else
	    surdSendRequest(); 

//	qDebug("SURD socket state is not 'connected'");
	    
//	    answerString = uid + ";:;0";
//	    answerArray = answerString.utf8();
//	    answerType = 1600;
//	    qDebug(answerString + " %d ", answerType);
//	    answerSize = sizeof(answerSize) + sizeof(answerType) + answerArray.size();
//	    clientStream << answerSize << answerType << answerArray;
    }

    if ((commandType == 200) or (commandType == 201) or (commandType == 202) or (commandType == 203)) {  
	if (queryFound) {
	    QSqlDatabase* sqliteDB = QSqlDatabase::addDatabase("QSQLITE");;
	    if (sqliteDB->driver() && sqliteDB->driver()->lastError().type() == QSqlError::Connection) 
        	qDebug("Не могу загрузить драйвер sqlite!");

	    sqliteDB->setDatabaseName("./sqlite/sqlite.base");
	    if (sqliteDB->open())     {
//		qDebug("Executing query: " + sQuery.lastQuery());
		initDB(sqliteDB);
		QSqlQuery sQuery(args[1]);
		
//  		QSqlQuery sQuery("CREATE TABLE documents (id INTEGER primary key,stamp TEXT,mb TEXT,punkt TEXT,inv_number TEXT,number_ex INTEGER,    doc_des TEXT,    print_last_page_stamp boolean,    print_overside_stamp boolean,    executor_id INTEGER,    pressman_id INTEGER);", sqliteDB); 
//  		QSqlQuery sQuery("CREATE TABLE add_info (id INTEGER primary key,info_body TEXT);", sqliteDB); 
//		QSqlQuery sQuery("INSERT into add_info(info_body) values('fsdfsdf', 'sdfwefsdfd', 'fffffffffff');", sqliteDB); 
//  		QSqlQuery sQuery("CREATE TABLE printers (    id INTEGER primary key,    printer_name TEXT,    dpi INTEGER,    descr TEXT);", sqliteDB); 
//  		QSqlQuery sQuery("CREATE TABLE rel_addinfo_document (    add_info_id INTEGER NOT NULL,    document_id INTEGER NOT NULL);", sqliteDB); 
//  		QSqlQuery sQuery("CREATE TABLE rel_rec_document (    reciver_id INTEGER NOT NULL,    document_id INTEGER NOT NULL);", sqliteDB); 
//  		QSqlQuery sQuery("CREATE TABLE resivers_list (    id INTEGER primary key,    rec_info TEXT(125));", sqliteDB); 
//  		QSqlQuery sQuery("CREATE TABLE reports (    id INTEGER primary key,    document_id INTEGER,    cups_user TEXT,    job_id INTEGER,    doc_name TEXT,    page_copy INTEGER,    printer_id INTEGER,    file_name TEXT,    dt INTEGER,    status TEXT,    print_status,    cups_error TEXT,    cups_printer_name TEXT);", sqliteDB); 
		qDebug("Last executed query: " + sQuery.executedQuery()); 
	    if (sQuery.isActive()) {
		qDebug("SqlQuery status: success");
	        qDebug("Database answer is:");
	        int fieldsCount = 10;
	        while ( sQuery.next() ) {
	    	    clientStream << "/1202";
		    QString sqliteAnswer = "";
		    for (int i=0;i<fieldsCount;i++) {
		        sqliteAnswer.append(sQuery.value(i).toString());
		        clientStream << ";:;" << sQuery.value(i).toString();
		        i++;
		    }
		    qDebug(sqliteAnswer);
		    clientStream << ";:;endl";
		}
//		clientStream << endl;
	    }
	    else {
    	        qDebug("SqlQuery failed ...");
		qDebug("Last error: " + sQuery.lastError().text());
	    }
	}else{ 
		qDebug("error");
//		qDebug("%d", sqliteDB->lastError().type());
//		qDebug(sqliteDB->lastError().text());
		
	    }
	}
/*	else  {
	    qDebug("Error processing with database");
	    qDebug("Driver error: " + sqliteDB->lastError().databaseText());		    
	    qDebug("driver text: " + sqliteDB->lastError().driverText());
	    qDebug("type: %d", sqliteDB->lastError().type());
	    qDebug("number: %d", sqliteDB->lastError().number());		
	}
*/   }
//    qDebug("return");
    return;
}


void sptsd::initDB(QSqlDatabase *DB)
{
    QSqlQuery pragma (DB);
    bool Ok = true;
    qDebug("Setting PRAGMAs");
    {
        Ok &= pragma.exec ("PRAGMA journal_mode = TRUNCATE;");
        if (!Ok){
        qDebug("PRAGMA journal_mode = TRUNCATE;" + pragma.lastError().text());
    }else{
        Ok &= pragma.exec ("PRAGMA synchronous = OFF;");
        if (!Ok){
            qDebug("PRAGMA synchronous = OFF;" + pragma.lastError().text());
        }else{
//            Ok &=pragma.exec ("PRAGMA temp_store = DISK;");
//	    if (!Ok)
//	        qDebug(pragma.lastError().text());
	    Ok &=pragma.exec("PRAGMA foreign_keys = ON");
	    if (!Ok)
	        qDebug("PRAGMA foreign_keys = ON" + pragma.lastError().text());
	}
    }
    }
return;
}


void sptsd::surdConnected()
{
    qDebug("Connected to SURD.");
    surdSendRequest();
    return;
}

void sptsd::surdSendRequest()
{
    QTextStream surdOutTs(&surdSocket);
    unsigned char ebcBuff[1024] = "";
    int len;

    len = surdCmd.length();
    strcpy((char *)ebcBuff, surdCmd.ascii());
    asc2ebc(ebcBuff, (int)surdCmd.length());

    qDebug("Writing to SURD: " + surdCmd);
    surdCmd = (char *)ebcBuff;
    surdSocket.writeBlock(surdCmd.ascii(), surdCmd.length());
    surdSocket.flush();
    qDebug("Waiting for SURD answer ...");
    return;
}
void sptsd::surdReadyRead()
{
    unsigned char ebcBuff[1024] = "";
    QTextStream surdInTs(&surdSocket);
    QDataStream outTs(this);
    QString surdAnsStr, surdStrings[100], outLine, seclabel, seclabels = "";
    QStringList lpArgs;
    QString answerString;
    QByteArray answerArray;
    Q_INT32 answerSize, answerType;
    int	len = surdSocket.bytesAvailable();


    qDebug("Reading SURD answer ...");

    surdInTs.readRawBytes((char *)ebcBuff, surdSocket.bytesAvailable());
            
    ebc2asc(ebcBuff, len);
    surdAnsStr = (char *)ebcBuff;

    int pos = surdAnsStr.find("\n", 0);
    int strCount = 0;
    while (pos != -1)
    {
	outLine = surdAnsStr.section('\n', 0, 0); 
	qDebug("SURD answer: " + outLine);
	switch (commandType)  {		// parsing it depending of which userCommand was sent
	    case 100:	// author		
		if (outLine.find("permitted", 0, false) != -1) {
		    qDebug("Wtote /1100");
//		    outTs << "/1100;:;" << endl;
		 }
		else if (outLine.find("denied", 0, false) != -1) {
		    qDebug("Wtote /1101");
//		    outTs << "/1101;:;" << endl;
		 }
		else if (outLine.find("not found", 0, false) != -1) {
		    qDebug("Wtote /1102");
//		    outTs << "/1102;:;" << endl;
		 }
		else
		    qDebug("Wrote /1100");
//		    outTs << outLine << endl;
		break;
	    case 400:	// lprtusr
		printers[strCount] = outLine;
		if (printers[strCount].find("not found", 0, false) != -1) {
		    qDebug("Wtote /1401");
//		    outTs << "/1401;:;" << endl; 
		}
		else 
		    lpArgs.append(printers[strCount].mid(printers[strCount].find(".", 0) + 1));
		break;
	    case 101:
		break;
	    case 300:
		seclabels.append(";:;");
		seclabels.append(getSeclevelStr(outLine)); 
		break;
	    case 600:
		if (outLine.find("UNABLE TO LOCATE", 0, false) != -1) {
		    qDebug("Wtote 1601");
		    answerType = 1601;
//		    outTs << ; 
		}
		else {
		    seclabels.append(";:;");
		    seclabels.append(outLine);
		    }
		break;
	    case 800:
		if (outLine.find("permitted", 0, false) != -1) {
		    qDebug("Wtote /1800");
//		    outTs << "/1800;:;" << endl;
		 }
		else if (outLine.find("not authorized", 0, false) != -1) {
		    qDebug("Wtote /1801");
//		    outTs << "/1801;:;" << endl;
		 }
		else if (outLine.find("not found", 0, false) != -1) {
		    qDebug("Wtote /1802");
//		    outTs << "/1802;:;" << endl;
		 }
		else if (outLine.find("invalid class name", 0, false) != -1) {
		    qDebug("Wtote /1803");
//		    outTs << "/1803;:;" << endl;
		 }
		else
		    qDebug("hzzzz");
//		    outTs << outLine << endl;
		break;
	    default:
		qDebug("command hyi");
		break;
	}
	if (userCommand == 100)
	    break;


        strCount++;

	surdAnsStr = surdAnsStr.mid(pos+1, surdAnsStr.length()-pos);
	pos = surdAnsStr.find("\n", 0);
    } //while (pos)

//	qDebug("After reading all SURD answer");
    if (commandType == 300) {
	qDebug("Wrote /1300" + seclabels);
//	outTs << "/1300" << QString(QObject::trUtf8(seclabels)) << endl;
    }
    if ((commandType == 600) and (seclabels != "")) {

	numSeclevels[0] = readSettings("/seclevels/num0"); numSeclevels[1] = readSettings("/seclevels/num1"); 
	numSeclevels[2] = readSettings("/seclevels/num3"); numSeclevels[4] = readSettings("/seclevels/num0"); 
	strSeclevels[0] = readSettings("/seclevels/nc"); strSeclevels[1] = readSettings("/seclevels/dcp"); 
	strSeclevels[2] = readSettings("/seclevels/c"); strSeclevels[3] = readSettings("/seclevels/cc"); 
	
//	qDebug(numSeclevels[0] + " = " + strSeclevels[0]);
	qDebug("Wrote /1600" + seclabels);
	answerString = uid + seclabels + ";:;";
	answerArray = answerString.utf8();
	answerType = 1600;
	answerSize = sizeof(answerSize) + sizeof(answerType) + answerArray.size();
	outTs << answerSize << answerType << answerArray;
	
//	outTs << "/1600" << QString(QObject::trUtf8(seclabels)) << endl;
    }	
	
    if ((userCommand == 400) and (lpArgs.join("") != "")) {
	lpProc = new QProcess;
	lpProc->addArgument("/usr/bin/lpstat");
	lpProc->addArgument("-v");
	lpProc->addArgument(lpArgs.join(","));
	qDebug(" ");
	qDebug("lpstat: " + lpProc->arguments().join(" "));
	connect(lpProc, SIGNAL(processExited()), this, SLOT(readLpstatOutput()) );
	if (!lpProc->start()) {
	    qDebug("Error running external script: " + lpProc->arguments().join(" ") + ". Exit code is %d", lpProc->exitStatus());
	}
	else {
    	    if (lpProc->isRunning() == 0)
    	    qDebug("Process NOT RUNNING. Script: " + lpProc->arguments().join(" ") + ". Exit code is %d", lpProc->exitStatus());
	    qDebug(" ");
	}
    }
    surdSocket.close();
    return;
}


void sptsd::readLpstatOutput()
{
    QTextStream	outTs( this );
    QString 	deviceLine, clientDeviceLine, device, devices[100];
    int		devCount = 0;

    while (lpProc->canReadLineStderr())
	qDebug("lpstat stderr: " + QString::fromUtf8(lpProc->readLineStderr()));

    while (lpProc->canReadLineStdout())
    {
	deviceLine = QString::fromUtf8(lpProc->readLineStdout());
	devices[devCount] = deviceLine.mid(deviceLine.find(":", 0) + 2);
	devCount ++;
    }
//    outTs << "/1400";
    for (int k = 0; k < devCount; k ++) {
	outTs << ";:;" << printers[k] << ";:;" << devices[k] <<"###";
	qDebug("printer: " + printers[k] + "  device: " + devices[k]);
    }	
//    outTs << endl;
	
    qDebug("=========================================================================================");
    return;
}


void sptsd::surdDisconnected()
{
    qDebug("Closing SURD connection ...");
    surdSocket.close();
    if (surdSocket.state() == QSocket::Closing)
    {
	connect(&surdSocket, SIGNAL(delayedCloseFinished()),SLOT(surdSocketClosed()));
	qDebug("Delayed Close ...");
    }
    else
	surdSocketClosed();
    return;
}

void sptsd::surdSocketClosed()
{
//	qDebug("SURD socket closed.");
    return;
}

void sptsd::showHex(unsigned char *chrBuff, int len, int showTextBelow)
{
    unsigned char ascBuff[2050] = "", hexBuff[2050] = "", crLf[6] = {0x20,0x0D,0x25,0x0D,0x0A,0x40};
    unsigned char *pHex;
    
    pHex = hexBuff;
    for (int i=0; i<len; i++) {
	if (chrBuff[i] == crLf[0] || chrBuff[i] == crLf[1] || chrBuff[i] == crLf[2] || chrBuff[i] == crLf[3] || chrBuff[i] == crLf[4] || chrBuff[i] == crLf[5]) 
	{
	    hexBuff[i*2] = crLf[0];
	    hexBuff[i*2+1] = crLf[0];
	}
	else  
	    sprintf((char *)pHex, "%X", chrBuff[i]);	    
	pHex+=2;
    }
    qDebug("HEX: %s", hexBuff);	

    if (showTextBelow > 0)
    {
	if (showTextBelow == 2)	
	    ebc2asc(chrBuff, len);	
	for (int i=0; i<len; i++) {
	    if (chrBuff[i] == crLf[0] || chrBuff[i] == crLf[1] || chrBuff[i] == crLf[2] || chrBuff[i] == crLf[3] || chrBuff[i] == crLf[4] || chrBuff[i] == crLf[5]) 
		ascBuff[i*2]=crLf[0];
	    else
		ascBuff[i*2]=chrBuff[i];
    	    ascBuff[i*2+1]=crLf[0];
	}
    qDebug("     %s", ascBuff);	    
    }	
    return;
}

void sptsd::asc2ebc(unsigned char *addr, int len)
{
/*
 * ASCII (IBM PC 437)  -> EBCDIC 037
 */
unsigned char  table[256] =
{
 /*00 NUL   SOH   STX   ETX   EOT   ENQ   ACK   BEL */
     0x00, 0x01, 0x02, 0x03, 0x37, 0x2D, 0x2E, 0x2F,
 /*08  BS    HT    LF    VT    FF    CR    SO    SI */
 /*              ->NL                               */
     0x16, 0x05, 0x15, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
 /*10 DLE   DC1   DC2   DC3   DC4   NAK   SYN   ETB */
     0x10, 0x11, 0x12, 0x13, 0x3C, 0x3D, 0x32, 0x26,
 /*18 CAN    EM   SUB   ESC    FS    GS    RS    US */
 /*                               ->IGS ->IRS ->IUS */
     0x18, 0x19, 0x3F, 0x27, 0x22, 0x1D, 0x1E, 0x1F,
 /*20  SP     |     "     #     $     %     &     ' */
     0x40, 0x5A, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D,
 /*28   (     )     *     +     ,     -    .      / */
     0x4D, 0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61,
 /*30   0     1     2     3     4     5     6     7 */
     0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
 /*38   8     9     :     ;     <     =     >     ? */
     0xF8, 0xF9, 0x7A, 0x5E, 0x4C, 0x7E, 0x6E, 0x6F,
 /*40   @     A     B     C     D     E     F     G */
     0x7C, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
 /*48   H     I     J     K     L     M     N     O */
     0xC8, 0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,
 /*50   P     Q     R     S     T     U     V     W */
     0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6,
 /*58   X     Y     Z     [     \     ]     ^     _ */
     0xE7, 0xE8, 0xE9, 0xBA, 0xE0, 0xBB, 0xB0, 0x6D,
 /*60   `     a     b     c     d     e     f     g */
     0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 /*68   h     i     j     k     l     m     n     o */
     0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
 /*70   p     q     r     s     t     u     v     w */
     0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6,
 /*78   x     y     z     {     :     }     ~    DL */
     0xA7, 0xA8, 0xA9, 0xC0, 0x4F, 0xD0, 0xA1, 0x07,
 /*80*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*88*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*90*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*98*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*A0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*A8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*B0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*B8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*C0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*C8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*D0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*D8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*E0        sz      */
     0x3F, 0x59, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*E8*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*F0*/
     0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
 /*F8*/
     0x90, 0x3F, 0x3F, 0x3F, 0x3F, 0xEA, 0x3F, 0xFF
};
 for(int i=len; i>0; i--)
  addr[i-1] = table[addr[i-1]];
 return;
}
 
 


void sptsd::ebc2asc(unsigned char *addr, int len)
{
/*
 * EBCDIC 037 -> ASCII (IBM PC 437)
 */
unsigned char  table[256] =
{
 /* 0x00   NUL   SOH   STX   ETX  *SEL    HT  *RNL   DEL */
          0x00, 0x01, 0x02, 0x03, 0x07, 0x09, 0x07, 0x7F,
 /* 0x08   -GE  -SPS  -RPT    VT    FF    CR    SO    SI */
          0x07, 0x07, 0x07, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
 /* 0x10   DLE   DC1   DC2   DC3  -RES   -NL    BS  -POC
                                  -ENP  ->LF             */
          0x10, 0x11, 0x12, 0x13, 0x07, 0x0A, 0x08, 0x07,
 /* 0x18   CAN    EM  -UBS  -CU1  -IFS  -IGS  -IRS  -ITB
                                                    -IUS */
          0x18, 0x19, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
 /* 0x20   -DS  -SOS    FS  -WUS  -BYP    LF   ETB   ESC
                                  -INP                   */
          0x07, 0x07, 0x1C, 0x07, 0x07, 0x0A, 0x17, 0x1B,
 /* 0x28   -SA  -SFE   -SM  -CSP  -MFA   ENQ   ACK   BEL
                       -SW                               */
          0x07, 0x07, 0x07, 0x07, 0x07, 0x05, 0x06, 0x07,
 /* 0x30  ----  ----   SYN   -IR   -PP  -TRN  -NBS   EOT */
          0x07, 0x07, 0x16, 0x07, 0x07, 0x07, 0x07, 0x04,
 /* 0x38  -SBS   -IT  -RFF  -CU3   DC4   NAK  ----   SUB */
          0x07, 0x07, 0x07, 0x07, 0x14, 0x15, 0x07, 0x1A,
 /* 0x40    SP   RSP           ?              ----       */
          0x20, 0xFF, 0x83, 0x84, 0x85, 0xA0, 0x07, 0x86,
 /* 0x48                       .     <     (     +     : */
          0x87, 0xA4, 0x9B, 0x2E, 0x3C, 0x28, 0x2B, 0x7C,
 /* 0x50     &                                      ---- */
          0x26, 0x82, 0x88, 0x89, 0x8A, 0xA1, 0x8C, 0x07,
 /* 0x58           ?     |     $     *     )     ;       */
          0x8D, 0xE1, 0x21, 0x24, 0x2A, 0x29, 0x3B, 0xAA,
 /* 0x60     -     /  ----        ----  ----  ----       */
          0x2D, 0x2F, 0x07, 0x8E, 0x07, 0x07, 0x07, 0x8F,
 /* 0x68              ----     ,     %     _     >     ? */
          0x80, 0xA5, 0x07, 0x2C, 0x25, 0x5F, 0x3E, 0x3F,
          0x07, 0x90, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
 /* 0x78     *     `     :     #     @     '     =     " */
          0x70, 0x60, 0x3A, 0x23, 0x40, 0x27, 0x3D, 0x22,
 /* 0x80     *     a     b     c     d     e     f     g */
          0x07, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
 /* 0x88     h     i              ----  ----  ----       */
          0x68, 0x69, 0xAE, 0xAF, 0x07, 0x07, 0x07, 0xF1,
 /* 0x90           j     k     l     m     n     o     p */
          0xF8, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
 /* 0x98     q     r                    ----        ---- */
          0x71, 0x72, 0xA6, 0xA7, 0x91, 0x07, 0x92, 0x07,
 /* 0xA0           ~     s     t     u     v     w     x */
          0xE6, 0x7E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
 /* 0xA8     y     z              ----  ----  ----  ---- */
          0x79, 0x7A, 0xAD, 0xAB, 0x07, 0x07, 0x07, 0x07,
 /* 0xB0     ^                    ----     ?  ----       */
          0x5E, 0x9C, 0x9D, 0xFA, 0x07, 0x07, 0x07, 0xAC,
 /* 0xB8        ----     [     ]  ----  ----  ----  ---- */
          0xAB, 0x07, 0x5B, 0x5D, 0x07, 0x07, 0x07, 0x07,
 /* 0xC0     {     A     B     C     D     E     F     G */
          0x7B, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
 /* 0xC8     H     I  ----           3              ---- */
          0x48, 0x49, 0x07, 0x93, 0x94, 0x95, 0xA2, 0x07,
 /* 0xD0     }     J     K     L     M     N     O     P */
          0x7D, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
 /* 0xD8     Q     R  ----           ?                   */
          0x51, 0x52, 0x07, 0x96, 0x81, 0x97, 0xA3, 0x98,
 /* 0xE0     \           S     T     U     V     W     X */
          0x5C, 0xF6, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
 /* 0xE8     Y     Z        ----        ----  ----  ---- */
          0x59, 0x5A, 0xFD, 0x07, 0x99, 0x07, 0x07, 0x07,
 /* 0xF0     0     1     2     3     4     5     6     7 */
          0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
 /* 0xF8     8     9  ----  ----     ?  ----  ----  ---- */
          0x38, 0x39, 0x07, 0x07, 0x9A, 0x07, 0x07, 0x07
};
 for(int i=len; i>0; i--)
  addr[i-1] = table[addr[i-1]];
 return;
}



QString sptsd::getSeclevelStr(QString seclevelNum)
{
    QString curSecl;
    for (int i = 0; i < 4; i ++) 
	if (seclevelNum == numSeclevels[i])
	    curSecl = strSeclevels[i]; 
    return curSecl;
}

QString sptsd::getSeclevelNum(QString seclevelStr)
{
    QString curSecl;
    for (int i = 0; i < 4; i ++) 
	if (seclevelStr == strSeclevels[i])
	    curSecl = numSeclevels[i]; 
    return curSecl;
}


/*
 * 'list_group()' - List printer-specific options from the PPD group.
 */

void sptsd::list_group(ppd_file_t  *ppd, ppd_group_t *group)
{
  int		i, j;			/* Looping vars */
  ppd_option_t	*option;		/* Current option */
  ppd_choice_t	*choice;		/* Current choice */
  ppd_group_t	*subgroup;		/* Current subgroup */


  for (i = group->num_options, option = group->options; i > 0; i --, option ++)
  {
    if (!strcasecmp(option->keyword, "PageRegion"))
      continue;

    _cupsLangPrintf(stdout, "%s/%s:", option->keyword, option->text);

    for (j = option->num_choices, choice = option->choices;
         j > 0;
	 j --, choice ++)
      if (!strcasecmp(choice->choice, "Custom"))
      {
        ppd_coption_t	*coption;	/* Custom option */
        ppd_cparam_t	*cparam;	/* Custom parameter */
	static const char * const types[] =
	{				/* Parameter types */
	  "CURVE",
	  "INTEGER",
	  "INVCURVE",
	  "PASSCODE",
	  "PASSWORD",
	  "POINTS",
	  "REAL",
	  "STRING"
	};


        if ((coption = ppdFindCustomOption(ppd, option->keyword)) == NULL ||
	    cupsArrayCount(coption->params) == 0)
	  _cupsLangPrintf(stdout, " %sCustom", choice->marked ? "*" : "");
        else if (!strcasecmp(option->keyword, "PageSize") ||
	        !strcasecmp(option->keyword, "PageRegion"))
	  _cupsLangPrintf(stdout, " %sCustom.WIDTHxHEIGHT",
	                  choice->marked ? "*" : "");
        else
	{
	  cparam = (ppd_cparam_t *)cupsArrayFirst(coption->params);

	  if (cupsArrayCount(coption->params) == 1)
	    _cupsLangPrintf(stdout, " %sCustom.%s", choice->marked ? "*" : "",
	                    types[cparam->type]);
	  else
	  {
	    const char	*prefix;	/* Prefix string */


            if (choice->marked)
	      prefix = " *{";
	    else
	      prefix = " {";

	    while (cparam)
	    {
	      _cupsLangPrintf(stdout, "%s%s=%s", prefix, cparam->name,
	                      types[cparam->type]);
	      cparam = (ppd_cparam_t *)cupsArrayNext(coption->params);
	      prefix = " ";
	    }

	    _cupsLangPuts(stdout, "}");
	  }
	}
      }
      else if (choice->marked)
        _cupsLangPrintf(stdout, " *%s", choice->choice);
      else
        _cupsLangPrintf(stdout, " %s", choice->choice);

    _cupsLangPuts(stdout, "\n");
  }

  for (i = group->num_subgroups, subgroup = group->subgroups; i > 0; i --, subgroup ++)
    list_group(ppd, subgroup);
}


/*
 * 'list_options()' - List printer-specific options from the PPD file.
 */

void sptsd::list_options(cups_dest_t *dest)		/* I - Destination to list */
{
  int		i;			/* Looping var */
  const char	*filename;		/* PPD filename */
  ppd_file_t	*ppd;			/* PPD data */
  ppd_group_t	*group;			/* Current group */


  if ((filename = cupsGetPPD(dest->name)) == NULL)
  {
    _cupsLangPrintf(stderr,
                    _("lpoptions: Unable to get PPD file for %s: %s\n"),
		    dest->name, cupsLastErrorString());
    return;
  }

  if ((ppd = ppdOpenFile(filename)) == NULL)
  {
    unlink(filename);
    _cupsLangPrintf(stderr,
                    _("lpoptions: Unable to open PPD file for %s!\n"),
		    dest->name);
    return;
  }

  ppdMarkDefaults(ppd);
  cupsMarkOptions(ppd, dest->num_options, dest->options);

  for (i = ppd->num_groups, group = ppd->groups; i > 0; i --, group ++)
    list_group(ppd, group);

  ppdClose(ppd);
  unlink(filename);
}


