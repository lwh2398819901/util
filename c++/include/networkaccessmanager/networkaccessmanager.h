/**
 * @copyright 2023-2023 Uniontech Technology Co., Ltd.
 *
 * @file networkaccessmanager.h
 *
 * @brief 网络访问管理器类的声明
 *
 * @date 2023-01-08
 *
 * Author: liuwh  <2398819901@qq.com>
 *
 * Maintainer: liuwh  <2398819901@qq.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include "publish.h"
#include <QTimer>
#include <QObject>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>

// 网络访问管理器类
class NetworkAccessManager : public QObject
{
    Q_OBJECT

public:
    // 构造函数
    explicit NetworkAccessManager(QObject *parent = nullptr);

    // 发送 GET 请求
    void get(const QNetworkRequest &request, int timeout = 30000);

    // 发送 POST 请求（字节数组）
    void post(const QNetworkRequest &request, const QByteArray &data, int timeout = 30000);

    // 发送 POST 请求（多部分数据）
    void post(const QNetworkRequest &request, QHttpMultiPart *data, int timeout = 30000);

    // 获取网络访问管理器实例
    QNetworkAccessManager* getManager() { return &m_anager; }

    // 获取 POST 数据
    QByteArray getPostData() { return m_arr; }

signals:
    void sig_timeout(QString url);            // 超时信号
    void sig_finish(QNetworkReply *re);       // 完成信号
    void sig_networkError(QNetworkReply *re); // 网络错误信号

private:
    QTimer m_timer;                           // 定时器
    QNetworkAccessManager m_anager;           // 网络访问管理器
    QNetworkCookieJar jar;                    // Cookie Jar
    QString m_url;                            // URL
    QByteArray m_arr;                         // POST 数据
};

#endif // NETWORKACCESSMANAGER_H
