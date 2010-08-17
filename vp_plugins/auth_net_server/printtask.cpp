#include "printtask.h"

PrintTask::PrintTask(QObject *parent)
    :QObject(parent)
    ,m_docName( QString() )
    ,m_mb( QString() )    
    ,m_printerQueue( QString() )
    ,m_pageCount(0)    

{
}
