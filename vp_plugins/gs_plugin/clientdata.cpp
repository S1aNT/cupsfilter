#include "clientdata.h"
#include <QMutableHashIterator>

ClientData::ClientData(QObject *parent)
    : QObject(parent)
    , fullDocConvert( false )
    , main_file  ( QString() )
    , first_page ( QString() )
    , other_page ( QString() )
    , page_count(0)
{

}

void ClientData::clear()
{
    fullDocConvert =false;
    first_page.clear();
    other_page.clear();
    page_count = 0;
    metaJob_merge.clear();
    metaJob_cat.clear();
    metaJob_pdf2png.clear();
    metaJob_pdf2ps.clear();
}

void ClientData::incJobsCount(const QString &job_id)
{
    if ( !hash.contains(job_id) ){
        hash.insert(job_id,0);
    }else{
        QMutableHashIterator<QString, int> i(hash);
        while (i.hasNext()) {
            i.next();
            if (i.key().compare(job_id,Qt::CaseInsensitive)){
                int job_count = i.value();
                i.setValue(job_count++);
            }
            qDebug() << i.key() << ": " << i.value();
        }
    }
}

bool ClientData::isJobFinished(const QString &job_id)
{
    if ( hash.contains(job_id) ){
        QMutableHashIterator<QString, int> i(hash);
        while (i.hasNext()) {
            i.next();
            qDebug() <<"isJobFinished " << i.key() << ": " << i.value();

            if (i.key().compare(job_id,Qt::CaseInsensitive)){
                int job_count = i.value();
                if (job_count == 0){
                    i.remove();
                    return true;
                }else{
                    i.setValue(job_count--);
                    return false;
                }
            }

        }
    }
    return false;
}

void ClientData::setConvertMode(bool b)
{
    fullDocConvert = b;
}

bool ClientData::getConvertMode()
{
    return fullDocConvert;
}


void ClientData::setMainFile (const QString &m)
{
    main_file = m;
}

QString ClientData::getMainFile() const
{
    return main_file;
}

void ClientData::setFirstPage (const QString &fp)
{
    first_page = fp;
}

QString ClientData::getFirstPage() const
{
    return first_page;
}

void ClientData::setOtherPage (const QString &op)
{
    other_page = op;
}

QString ClientData::getOtherPage() const
{
    return other_page;
}

void ClientData::setPageCount(int p)
{
    page_count = p;
}

int ClientData::getPageCount()
{
    return page_count;
}

void ClientData::startMerge(int number)
{
    metaJob_merge.push(number);
}

bool ClientData::isFinishedMerge()
{
    metaJob_merge.pop();
    return metaJob_merge.isEmpty();
}

void ClientData::startCat(int number)
{
    metaJob_cat.push(number);
}

bool ClientData::isFinishedCat()
{
    metaJob_cat.pop();
    return metaJob_cat.isEmpty();
}

void  ClientData::startConvertPdf2Png(int number)
{
    metaJob_pdf2png.push(number);
}

bool ClientData::isFinishedConvertPdf2Png()
{
    metaJob_pdf2png.pop();
    return metaJob_pdf2png.isEmpty();
}

void ClientData::startConvertPdf2Ps(int number)
{
    metaJob_pdf2ps.push(number);
}

bool ClientData::isFinishedConvertPdf2Ps()
{
    metaJob_pdf2ps.pop();
    return metaJob_pdf2ps.isEmpty();
}
