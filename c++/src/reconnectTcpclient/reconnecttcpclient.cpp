#include "reconnecttcpclient.h"
#include "pub_macro.h"

ReconnectTcpclient::ReconnectTcpclient(QObject *parent)
    : QTcpSocket{parent}
{
    connect(this, &QTcpSocket::disconnected, this, &ReconnectTcpclient::onDisconnected);
    connect(&m_reTime, &QTimer::timeout, this, &ReconnectTcpclient::tryConnect);
}

ReconnectTcpclient::~ReconnectTcpclient()
{

}

void ReconnectTcpclient::connectToHostWithReconnect(const QString &host, quint16 port)
{
    m_host = host;
    m_port = port;
    disConnectFlag = false;
    tryConnect();
}

void ReconnectTcpclient::disconectTcpServer()
{
    disConnectFlag = true;
    m_reTime.stop();
    disconnectFromHost();
    close();
}

void ReconnectTcpclient::onDisconnected()
{
    // 当连接断开时，启动重连定时器
    if (!m_reTime.isActive() && !disConnectFlag)
        m_reTime.start(3000);
}


void ReconnectTcpclient::tryConnect()
{
    LOGGER_DEBUG("tryConnect:" + m_host + " " + QString::number(m_port));
    QAbstractSocket::SocketState socketState = state();

    if (socketState == QAbstractSocket::UnconnectedState) {
        // 在此状态下可以安全地调用 connectToHost()
        // 连接到主机
        connectToHost(m_host, m_port);
        m_reTime.start(10000);
        emit sig_tryConnected();
        LOGGER_DEBUG(u8"tryConnect: 尝试链接 启动定时器..... ");
    } else if (socketState == QAbstractSocket::HostLookupState) {
        // 正在查找主机，可以等待或处理其他逻辑
    } else if (socketState == QAbstractSocket::ConnectingState) {
        // 正在连接中，可以等待或处理其他逻辑
    } else if (socketState == QAbstractSocket::ConnectedState) {
        // 已连接，可能需要关闭现有连接或执行其他操作
        LOGGER_DEBUG(u8"tryConnect: 已连接 关闭定时器 ");
        m_reTime.stop();
    } else {
        // 处理其他状态或错误情况
    }
}
