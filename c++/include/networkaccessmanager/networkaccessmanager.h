/**
 * @copyright 2023-2023 Uniontech Technology Co., Ltd.
 *
 * @file
 *
 * @brief
 *
 * @date
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


class NetworkAccessManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkAccessManager(QObject *parent = nullptr);
    void get(const QNetworkRequest &request, int timeout = 30000);
    void post(const QNetworkRequest &request, const QByteArray &data, int timeout = 30000);
    void post(const QNetworkRequest &request, QHttpMultiPart *data, int timeout = 30000);
    QNetworkAccessManager*getManager(){return &m_anager;}
signals:
    void sig_timeout(QString url);
    void sig_finish(QNetworkReply *re);
    void sig_networkError(QNetworkReply *re);
private:
    QTimer m_timer;
    QNetworkAccessManager m_anager;
    QNetworkCookieJar jar;
    QString m_url;
};

#endif // NETWORKACCESSMANAGER_H
