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
    if (!m_reTime.isActive() && !disConnectFlag)
        m_reTime.start(3000);
}


void ReconnectTcpclient::tryConnect()
{
    LOGGER_DEBUG("tryConnect:" + m_host + " " + QString::number(m_port));
    QAbstractSocket::SocketState socketState = state();

    if (socketState == QAbstractSocket::UnconnectedState) {
        connectToHost(m_host, m_port);
        m_reTime.start(10000);
        emit sig_tryConnected();
        LOGGER_DEBUG(u8"tryConnect: Attempt to link or Try to connect, Start the timer..... ");
    } else if (socketState == QAbstractSocket::HostLookupState) {
    } else if (socketState == QAbstractSocket::ConnectingState) {
    } else if (socketState == QAbstractSocket::ConnectedState) {
        LOGGER_DEBUG(u8"tryConnect: Connected, Stop the timer ");
        m_reTime.stop();
    } else {
    }
}
