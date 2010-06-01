#ifndef MESSAGE_H
#define MESSAGE_H
/**
  * @class Messeage Данный класс инкапсулирует в себе сообщение передаваемое от
  * клиента к серверу и обратно
  */
#include "mytypes.h"

#include <QObject>
#include <QStringList>

#include <QtCore/QByteArray>

using namespace VPrn;



class Message : public QObject
{
    Q_OBJECT
    Q_ENUMS   ( VPrn::MessageType )
public:
    explicit Message(QObject *parent = 0);
    /**
      * @fn void clear();
      * @brief Очистка сообщения
      */
    void clear();
    /**
      * @brief Создает и возвращает пакет данных готовый для передачи
      * @param msg Сообщение включаемое в пакет
      * @param type Тип сообщения в пакете
      * @return QByteArray
      */
     QByteArray createPacket() const;

    /**
      * @fn bool isMessageReady();
      * @brief Возвращает TRUE Если сообщение готов для отправки иначе FALSE
      * @return bool
      */
    bool isMessageReady();

    /**
      * @fn void prepareMessage();
      * @brief Подготовка сообщения перед  отправкой
      * @internal
     */
    void prepareMessage();
    /**
      * @fn void setMessage( const QByteArray &msg );
      * @param msg const QByteArray &
      * @brief Запись msg во внутреннюю структуру и разбор его как сообщения
      */
    void setMessageData( const QByteArray &m_data );

    void setMessageData( QStringList &m_list );

    /**
     * @brief Возвращает данные подготовленные для пересылки
     * @return QByteArray
     */
    QByteArray messageData() const;

    QStringList messageDataList() const;
    /**
     * @brief type property: Getter
     * @return MessageType
     */
    MessageType type() const;

    /**
     * @brief type property: Setter
     * @param tp MessageType
     */
    void setType( MessageType tp );

signals:

public slots:

private:
    MessageType messageType;
    QByteArray msgData;
    bool ready;

};

#endif // MESSAGE_H
