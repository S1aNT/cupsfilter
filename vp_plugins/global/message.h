#ifndef MESSAGE_H
#define MESSAGE_H
/**
  * @class Messeage Данный класс инкапсулирует в себе сообщение передаваемое от
  * клиента к серверу и обратно
  */

#include <QObject>
#include <QStringList>

#include <QtCore/QByteArray>

#include "mytypes.h"

using namespace VPrn;


class Message : public QObject
{
    Q_OBJECT
    Q_ENUMS   ( VPrn::MessageType )
/// @todo Перечисление типов сообщений внести в класс
public:
            explicit Message(QObject *parent = 0);
    /**
      * @fn void clear();
      * @brief Очистка сообщения
      */
    void clear();




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
    QByteArray getMessageData() const;

    QStringList getMessageDataList() const;
    /**
     * @brief type property: Getter
     * @return MessageType
     */
    MessageType getType() const;

    /**
     * @brief type property: Setter
     * @param tp MessageType
     */
    void setType( MessageType tp );

    QByteArray createPacket() const;
private:
    MessageType messageType;
    QByteArray msgData;
    bool m_valid;

};

#endif // MESSAGE_H
