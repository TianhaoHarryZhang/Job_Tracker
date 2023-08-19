#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H



#include <QObject>

class MessageHandler : public QObject
{
    Q_OBJECT

public:
    explicit MessageHandler(QObject *parent = nullptr);

signals:
    void messageHandler(const QString& message);

public slots:
    void handleMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg);
};


#endif // MESSAGEHANDLER_H
