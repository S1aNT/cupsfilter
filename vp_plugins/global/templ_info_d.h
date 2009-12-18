#ifndef TEMPL_INFO_D_H
#define TEMPL_INFO_D_H
//Приватный класс с разделением данных. Используется QSharedData.
//класс Templ_info_d, представляющий информацию о шаблоне,
#include <QSharedData>
#include <QString>

class Templ_info_d : public QSharedData
{
public:
    // Конструкторы
    Templ_info_d();
    Templ_info_d (const Templ_info_d& other);
    ~Templ_info_d() {};
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
    int fourthPageElemCount;   // число элементов на четвертой странице шаблона
};



#endif
