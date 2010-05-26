#include "clientdata.h"

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

void  ClientData::startConverPdf2Png(int number)
{
    metaJob_pdf2png.push(number);
}

bool ClientData::isFinishedConvertPdf2Png()
{
    metaJob_pdf2png.pop();
    return metaJob_pdf2png.isEmpty();
}
