#ifndef TEMPLATESINFO_H
#define TEMPLATESINFO_H
#include <QObject>
#include <QByteArray>


/**
  * @class TemplatesInfo Преданзначен для работы с моделью информация о шаблонах
  * умеет конструировать QStandardItemModel  и заполнять ее данными. Имеет функции
  * отвечающие на вопросы:
  * @li есть ли в шаблоне с именем [t_name] лицевая страница c № [page_number]
  * @li полный фаловый путь для шаблона с именем [t_name]
  */
/**
    t->setModel( new QStandardItemModel() );      <- класс станет владельцем модели он ее и удалит
    t->setModel( new QStandardItemModel(this) );  <- тот кто вызвал, чей this тот и владелец модели,
                                                  он ее и удаляет
*/

QT_FORWARD_DECLARE_CLASS ( QStandardItemModel )
QT_FORWARD_DECLARE_CLASS ( QDataStream )

class TemplatesInfo : public QObject
{
Q_OBJECT
public:
    TemplatesInfo(QObject *parent = 0);
    ~TemplatesInfo();
    void setStdHeader();

    QStandardItemModel *model(){return tInfoModel;}

    QByteArray toByteArray() const;


public slots:
    void fromByteArray( QByteArray m_data );

private:
    /**
      * @var tInfoModel; Модель содержащая в себе сведения о всех шаблонах
      * которые доступны пользователю.Ее заполняет GateKeeper  и передает в сообщении
      */
    QStandardItemModel * tInfoModel;

   // friend QDebug      operator  << (QDebug &out,      const TemplatesInfo& save);
   // friend QDataStream &operator << (QDataStream &out, const TemplatesInfo& save);
   // friend QDataStream &operator >> (QDataStream &in,  TemplatesInfo& load);
};


#endif // TEMPLATESINFO_H
