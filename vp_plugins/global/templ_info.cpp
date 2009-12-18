#include "templ_info.h"

Templ_info::Templ_info()
{
    d = new Templ_info_d;
}

Templ_info::Templ_info (int t_ver, QString date_time, QString t_author,
                        QString t_name, QString t_desc, QString p_size,
                        bool page_orient,
                        qreal page_width, qreal page_height,
                        qreal m_top, qreal m_bottom,
                        qreal m_left, qreal m_right,
                        int firstPageElemCount, int secondPageElemCount,
                        int thirdPageElemCount, int fourthPageElemCount)
{
    d = new Templ_info_d;
    setT_ver(t_ver);
    setDate_time(date_time);
    setT_author(t_author);
    setT_name(t_name);
    setT_desc(t_desc);
    setP_size(p_size);
    setPage_orient(page_orient);
    setPage_width(page_width);
    setPage_height(page_height);
    setM_top(m_top);
    setM_bottom(m_bottom);
    setM_left(m_left);
    setM_right(m_right);
    setFirstPageElemCount(firstPageElemCount);
    setSecondPageElemCount(secondPageElemCount);
    setThirdPageElemCount(thirdPageElemCount);
    setFourthPageElemCount(fourthPageElemCount);
}

int Templ_info::t_ver() const
{
    return d->t_ver;
}
void Templ_info::setT_ver(int t_ver)
{
    d->t_ver = t_ver;
}

QString Templ_info::date_time() const
{
    return d->date_time;
}
void Templ_info::setDate_time(QString date_time)
{
    d->date_time = date_time;
}

QString Templ_info::t_author() const
{
    return d->t_author;
}
void Templ_info::setT_author(QString t_author)
{
    d->t_author = t_author;
}

QString Templ_info::t_name() const
{
    return d->t_name;
}
void Templ_info::setT_name(QString t_name)
{
    d->t_name = t_name;
}

QString Templ_info::t_desc() const
{
    return d->t_desc;
}
void Templ_info::setT_desc(QString t_desc)
{
    d->t_desc = t_desc;
}

QString Templ_info::p_size() const
{
    return d->p_size;
}
void Templ_info::setP_size(QString p_size)
{
    d->p_size = p_size;
}

bool Templ_info::page_orient() const
{
    return d->page_orient;
}
void Templ_info::setPage_orient(bool page_orient)
{
    d->page_orient = page_orient;
}

qreal Templ_info::page_width() const
{
    return d->page_width;
}
void Templ_info::setPage_width(qreal page_width)
{
    d->page_width = page_width;
}

qreal Templ_info::page_height() const
{
    return d->page_height;
}
void Templ_info::setPage_height(qreal page_height)
{
    d->page_height = page_height;
}

qreal Templ_info::m_top() const
{
    return d->m_top;
}
void Templ_info::setM_top(qreal m_top)
{
    d->m_top = m_top;
}

qreal Templ_info::m_bottom() const
{
    return d->m_bottom;
}
void Templ_info::setM_bottom(qreal m_bottom)
{
    d->m_bottom = m_bottom;
}

qreal Templ_info::m_left() const
{
    return d->m_left;
}
void Templ_info::setM_left(qreal m_left)
{
    d->m_left = m_left;
}

qreal Templ_info::m_right() const
{
    return d->m_right;
}
void Templ_info::setM_right(qreal m_right)
{
    d->m_right = m_right;
}

int Templ_info::firstPageElemCount() const
{
    return d->firstPageElemCount;
}
void Templ_info::setFirstPageElemCount(int firstPageElemCount)
{
    d->firstPageElemCount = firstPageElemCount;
}

int Templ_info::secondPageElemCount() const
{
    return d->secondPageElemCount;
}
void Templ_info::setSecondPageElemCount(int secondPageElemCount)
{
    d->secondPageElemCount = secondPageElemCount;
}

int Templ_info::thirdPageElemCount() const
{
    return d->thirdPageElemCount;
}
void Templ_info::setThirdPageElemCount(int thirdPageElemCount)
{
    d->thirdPageElemCount = thirdPageElemCount;
}

int Templ_info::fourthPageElemCount() const
{
    return d->fourthPageElemCount;
}
void Templ_info::setFourthPageElemCount(int fourthPageElemCount)
{
    d->fourthPageElemCount = fourthPageElemCount;
}


Templ_info Templ_info::clone() const {
  Templ_info copy(*this);
  copy.d.detach();
  return copy;
}

QDataStream &operator<<( QDataStream &out, const Templ_info& save )
{
    out << save.t_ver();
    out << save.date_time();
    out << save.t_author();
    out << save.t_name();
    out << save.t_desc();
    out << save.p_size();
    out << save.page_orient();
    out << save.page_width();
    out << save.page_height();
    out << save.m_top();
    out << save.m_bottom();
    out << save.m_left();
    out << save.m_right();
    out << save.firstPageElemCount();
    out << save.secondPageElemCount();
    out << save.thirdPageElemCount();
    out << save.fourthPageElemCount();
    return out;
}

QDebug operator << ( QDebug &out, const Templ_info& save )
{
    out << "t_ver: " << save.t_ver();
    out << "date_time: " << save.date_time();
    out << "t_author: " << save.t_author();
    out << "t_name: " << save.t_name();
    out << "t_desc: " << save.t_desc();
    out << "p_size: " << save.p_size();
    out << "page_orient: " << save.page_orient();
    out << "page_width: " << save.page_width();
    out << "page_height: " << save.page_height();
    out << "m_top: " << save.m_top();
    out << "m_bottom: " << save.m_bottom();
    out << "m_left: " << save.m_left();
    out << "m_right: " << save.m_right();
    out << "firstPageElemCount: " << save.firstPageElemCount();
    out << "secondPageElemCount: " << save.secondPageElemCount();
    out << "thirdPageElemCount: " << save.thirdPageElemCount();
    out << "fourthPageElemCount: " << save.fourthPageElemCount();
    return out;
}
QDataStream &operator>>( QDataStream &in, Templ_info& load)
{
    int t_ver; 	        // версия шаблона
    QString date_time;  // дата и время создания
    QString t_author;   // автор шаблона
    QString t_name;     // имя шаблона
    QString t_desc;     // описание шаблона
    QString p_size;     // размер бумаги (Для человека)
    bool  page_orient;  // ориентация страницы Книжная/альбомная true/false
    qreal page_width;   // ширина листа в  [мм -> point] (для компьютера)
    qreal page_height;  // высота листа в  [мм -> point] (для компьютера)
    qreal m_top;        // отступ сверху в [мм -> point] (для компьютера)
    qreal m_bottom;     // отступ снизу в [мм -> point] (для компьютера)
    qreal m_left;       // отступ слева в [мм -> point] (для компьютера)
    qreal m_right;      // отступ справа в [мм -> point] (для компьютера)
    int firstPageElemCount;    // число элементов на первой странице шаблона
    int secondPageElemCount;   // число элементов на второй странице шаблона
    int thirdPageElemCount;    // число элементов на третьей странице шаблона
    int fourthPageElemCount;

    in >> t_ver;
    in >> date_time;
    in >> t_author;
    in >> t_name;
    in >> t_desc;
    in >> p_size;
    in >> page_orient;
    in >> page_width;
    in >> page_height;
    in >> m_top;
    in >> m_bottom;
    in >> m_left;
    in >> m_right;
    in >> firstPageElemCount;
    in >> secondPageElemCount;
    in >> thirdPageElemCount;
    in >> fourthPageElemCount;
    load = Templ_info(t_ver, date_time, t_author,t_name, t_desc, p_size,
                      page_orient, page_width, page_height,
                      m_top, m_bottom, m_left, m_right,
                      firstPageElemCount, secondPageElemCount,
                      thirdPageElemCount, fourthPageElemCount);
    return in;
}
