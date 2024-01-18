#ifndef RECONNECTTCPCLIENT_H
#define RECONNECTTCPCLIENT_H

#include <QTcpSocket>
#include <QTimer>
#include <QObject>
class ReconnectTcpclient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit ReconnectTcpclient(QObject *parent = nullptr);
    ~ReconnectTcpclient();
    void connectToHostWithReconnect(const QString &host, quint16 port);
    void disconectTcpServer();
signals:
    void sig_tryConnected();
private slots:
    void onDisconnected();
    void tryConnect();
private:
    bool disConnectFlag{false};
    QString m_host{};
    quint16 m_port{};
    QTimer m_reTime;
};

#endif // RECONNECTTCPCLIENT_H
