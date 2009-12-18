#ifndef TEMPL_INFO_H
#define TEMPL_INFO_H
#include <QDebug>
#include <QString>
#include <QSharedDataPointer>
#include <QDataStream>
#include<QMetaType>
#include "templ_info_d.h"

class Templ_info
{
public:
    Templ_info();
    Templ_info (int t_ver, QString date_time, QString t_author,
                QString t_name, QString t_desc, QString p_size,
                bool page_orient, qreal page_width, qreal page_height,
                qreal m_top, qreal m_bottom, qreal m_left, qreal m_right,
                int firstPageElemCount, int secondPageElemCount,
                int thirdPageElemCount, int fourthPageElemCount);
    Templ_info (const Templ_info& other) : d (other.d) {};

    int t_ver() const;
    void setT_ver(int t_ver);

    QString date_time() const;
    void setDate_time(QString date_time);

    QString t_author() const;
    void setT_author(QString t_author);

    QString t_name() const;
    void setT_name(QString t_name);

    QString t_desc() const;
    void setT_desc(QString t_desc);

    QString p_size() const;
    void setP_size(QString p_size);

    bool page_orient() const;
    void setPage_orient(bool page_orient);

    qreal page_width() const;
    void setPage_width(qreal page_width);

    qreal page_height() const;
    void setPage_height(qreal page_height);

    qreal m_top() const;
    void setM_top(qreal m_top);

    qreal m_bottom() const;
    void setM_bottom(qreal m_bottom);

    qreal m_left() const;
    void setM_left(qreal m_left);

    qreal m_right() const;
    void setM_right(qreal m_right);

    int firstPageElemCount() const;
    void setFirstPageElemCount(int firstPageElemCount);

    int secondPageElemCount() const;
    void setSecondPageElemCount(int secondPageElemCount);

    int thirdPageElemCount() const;
    void setThirdPageElemCount(int thirdPageElemCount);

    int fourthPageElemCount() const;
    void setFourthPageElemCount(int fourthPageElemCount);

//    Templ_info clone() const;	
private:


    //QSharedDataPointer<Templ_info_d> d;
    QExplicitlySharedDataPointer <Templ_info_d> d;


    friend QDataStream &operator >> (QDataStream &in, Templ_info& load);
    friend QDataStream &operator << (QDataStream &out, const Templ_info& save);
    friend QDebug      operator <<  (QDebug &out, const Templ_info& save);
};



Q_DECLARE_METATYPE(Templ_info);

#endif
