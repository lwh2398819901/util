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


#include "networkaccessmanager.h"


NetworkAccessManager::NetworkAccessManager(QObject *parent) : QObject(parent)
{
    connect(&m_anager, &QNetworkAccessManager::finished, [ = ](QNetworkReply * re) {
        m_timer.stop();
        if (re == nullptr) {
            LOGGER_ERR("请求失败:" + m_url);
            emit sig_timeout(m_url);
            return;
        }

        if (re->error() != QNetworkReply::NetworkError::NoError) {
            LOGGER_ERR("网络异常:" + m_url);
            emit sig_networkError(re);
        } else {
            LOGGER_INFO("请求成功:" + m_url);
            emit sig_finish(re);
        }
    });
}

void NetworkAccessManager::get(const QNetworkRequest &request, int timeout)
{
    m_timer.setSingleShot(true);
    m_timer.start(timeout);
    m_url = request.url().url();
    QNetworkReply *pReply = m_anager.get(request);
    connect(&m_timer, &QTimer::timeout, [ = ]() {
        pReply->abort();
        pReply->deleteLater();
        m_timer.stop();
        emit sig_timeout(m_url);
        LOGGER_ERR("请求超时:" + m_url);
    });
}

void NetworkAccessManager::post(const QNetworkRequest &request, const QByteArray &data, int timeout)
{
    m_timer.setSingleShot(true);
    m_timer.start(timeout);
    m_url = request.url().url();
    QNetworkReply *pReply = m_anager.post(request, data);
    connect(&m_timer, &QTimer::timeout, [ = ]() {
        pReply->abort();
        pReply->deleteLater();
        m_timer.stop();
        emit sig_timeout(m_url);
        LOGGER_ERR("请求超时:" + m_url);
    });
}

void NetworkAccessManager::post(const QNetworkRequest &request,  QHttpMultiPart *data, int timeout)
{
    m_timer.setSingleShot(true);
    m_timer.start(timeout);
    m_url = request.url().url();
    QNetworkReply *pReply = m_anager.post(request, data);
    connect(&m_timer, &QTimer::timeout, [ = ]() {
        pReply->abort();
        pReply->deleteLater();
        m_timer.stop();
        emit sig_timeout(m_url);
        LOGGER_ERR("请求超时:" + m_url);
    });
}
