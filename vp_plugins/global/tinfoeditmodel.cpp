#include <QtSql>
#include "tinfoeditmodel.h"

TemplateInfoEditModel::TemplateInfoEditModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    //
    qRegisterMetaType<tInfoColumnOrder>("tInfoColumnOrder");
}


Qt::ItemFlags TemplateInfoEditModel::flags(
        const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    /*
    if (index.column() == 1  || index.column() == 2 || 
        index.column() == 4  || index.column() == 5 || 
        index.column() == 6  || index.column() == 7 || 
        index.column() == 8  || index.column() == 9 || 
        index.column() == 10 || index.column() == 11){
        flags |= Qt::ItemIsEditable;
    }
    */
    if (index.column() !=0){
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

bool TemplateInfoEditModel::setData(const QModelIndex &index, const QVariant &value, int /* role */)
{
    if (index.column() ==0) //|| index.column() == 3)
        return false;

    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
    int id = data(primaryKeyIndex).toInt();
    clear();

    bool ok;
    switch (index.column()){
    case tInfo_name:    ok = setTemplateName(id,value.toString());
        break;
    case tInfo_desc:    ok = setTemplateDesc(id,value.toString());
        break;
    case tInfo_pageID:  ok = setTemplatePageId(id,value.toInt());
        break;
    case tInfo_angle:   ok = setTemplateAngle(id,value.toInt());
        break;
    case tInfo_ctime:   ok = setTemplateCTime(id,value.toInt());
        break;
    case tInfo_mtime:   ok = setTemplateMTime(id,value.toInt());
        break;
    case tInfo_author:  ok = setTemplateAuthor(id,value.toString());
        break;
    case tInfo_mtop:    ok = setTemplateMTop(id,value.toInt());
        break;
    case tInfo_mbottom: ok = setTemplateMBottom(id,value.toInt());
        break;
    case tInfo_mleft:   ok = setTemplateMLeft(id,value.toInt());
        break;
    case tInfo_mright:  ok = setTemplateMRight(id,value.toInt());
        break;
    case tInfo_p_width:  ok = false;
        break;
    case tInfo_p_height:  ok = false;
        break;

    }

    refresh();
    if (ok){
        emit dataChanged(index, index);
    }    
    return ok;
}


void TemplateInfoEditModel::refresh()
{
    setQuery("SELECT template.id,t_name,t_desc,"
             "page_size_id,angle,"
             "c_time,m_time,author,margin_top,margin_bottom,"
             "margin_left,margin_right,page_size.p_witdh,page_size.p_height  "
             "FROM template INNER JOIN page_size ON template.page_size_id=page_size.id");
          setHeaderData(tInfo_id,     Qt::Horizontal, tr("ID"));                 // 0
          setHeaderData(tInfo_name,   Qt::Horizontal, tr("Имя шаблона"));        // 1
          setHeaderData(tInfo_desc,   Qt::Horizontal, tr("Описание"));           // 2
          setHeaderData(tInfo_pageID, Qt::Horizontal, tr("PSize_id"));           // 3
          setHeaderData(tInfo_angle,  Qt::Horizontal, tr("Поворот (град.)"));    // 4
          setHeaderData(tInfo_ctime,  Qt::Horizontal, tr("Время создания"));     // 5
          setHeaderData(tInfo_mtime,  Qt::Horizontal, tr("Время изменения"));    // 6
          setHeaderData(tInfo_author, Qt::Horizontal, tr("Автор"));              // 7
          setHeaderData(tInfo_mtop,   Qt::Horizontal, tr("Отступ сверху (мм)")); // 8
          setHeaderData(tInfo_mbottom,Qt::Horizontal, tr("Отступ снизу (мм)"));  // 9
          setHeaderData(tInfo_mleft,  Qt::Horizontal, tr("Отступ слева (мм)"));  // 10
          setHeaderData(tInfo_mright, Qt::Horizontal, tr("Отступ справа (мм)")); // 11
          setHeaderData(tInfo_p_width, Qt::Horizontal,  tr("Ширина (мм)")); // 12
          setHeaderData(tInfo_p_height, Qt::Horizontal, tr("Высота (мм)")); // 13

}

bool TemplateInfoEditModel::setTemplateName(int Id, const QString &templateName)
{
    QSqlQuery query;
    query.prepare("update template set t_name = ? where id = ?");
    query.addBindValue(templateName);
    query.addBindValue(Id);
    return query.exec();
}

bool TemplateInfoEditModel::setTemplateDesc(int Id, const QString &templateDesc)
{
    QSqlQuery query;
    query.prepare("update template set t_desc = ? where id = ?");
    query.addBindValue(templateDesc);
    query.addBindValue(Id);
    return query.exec();
}

bool TemplateInfoEditModel::setTemplatePageId(int Id, int pageId)
{
    QSqlQuery query;
    query.prepare("update template set page_size_id = ? where id = ?");
    query.addBindValue(pageId);
    query.addBindValue(Id);
    return query.exec();
}

bool TemplateInfoEditModel::setTemplateAngle(int Id, int angle)
{
    QSqlQuery query;
    query.prepare("update template set angle = ? where id = ?");
    query.addBindValue(angle);
    query.addBindValue(Id);
    return query.exec();
}

bool TemplateInfoEditModel::setTemplateCTime(int Id, int cTime)
{
    QSqlQuery query;
    query.prepare("update template set c_time = ? where id = ?");
    query.addBindValue(cTime);
    query.addBindValue(Id);
    return query.exec();
}

bool TemplateInfoEditModel::setTemplateMTime(int Id, int mTime)
{
    QSqlQuery query;
    query.prepare("update template set m_time = ? where id = ?");
    query.addBindValue(mTime);
    query.addBindValue(Id);
    return query.exec();
}

bool TemplateInfoEditModel::setTemplateAuthor(int Id, const QString &templateAuthor)
{
    QSqlQuery query;
    query.prepare("update template set author = ? where id = ?");
    query.addBindValue(templateAuthor);
    query.addBindValue(Id);
    return query.exec();
}

bool TemplateInfoEditModel::setTemplateMTop(int Id, int mTop)
{
    QSqlQuery query;
    query.prepare("update template set margin_top = ? where id = ?");
    query.addBindValue(mTop);
    query.addBindValue(Id);
    return query.exec();
}

bool TemplateInfoEditModel::setTemplateMBottom(int Id, int mBottom)
{
    QSqlQuery query;
    query.prepare("update template set margin_bottom = ? where id = ?");
    query.addBindValue(mBottom);
    query.addBindValue(Id);
    return query.exec();
}

bool TemplateInfoEditModel::setTemplateMLeft(int Id, int mLeft)
{
    QSqlQuery query;
    query.prepare("update template set margin_left = ? where id = ?");
    query.addBindValue(mLeft);
    query.addBindValue(Id);
    return query.exec();
}

bool TemplateInfoEditModel::setTemplateMRight(int Id, int mRight)
{
    QSqlQuery query;
    query.prepare("update template set margin_right = ? where id = ?");
    query.addBindValue(mRight);
    query.addBindValue(Id);
    return query.exec();
}
