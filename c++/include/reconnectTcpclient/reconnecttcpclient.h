#ifndef RECONNECTTCPCLIENT_H
#define RECONNECTTCPCLIENT_H

#include <QTcpSocket>
#include <QTimer>
#include <QObject>
class ReconnectTcpclient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit ReconnectTcpclient(QObject *parent = nullptr);  // 构造函数
    ~ReconnectTcpclient();                                   // 析构函数
    void connectToHostWithReconnect(const QString &host, quint16 port);  // 连接到指定主机
    void disconectTcpServer();                               // 断开与 TCP 服务器的连接
signals:
    void sig_tryConnected();                                 // 尝试连接信号
private slots:
    void onDisconnected();                                   // 当 TCP 连接断开时调用
    void tryConnect();                                       // 尝试重新连接到服务器
private:
    bool disConnectFlag{false};                              // 主动断开连接标识
    QString m_host{};                                        // 主机地址
    quint16 m_port{};                                        // 端口号
    QTimer m_reTime;                                         // 重连定时器
};

#endif // RECONNECTTCPCLIENT_H
