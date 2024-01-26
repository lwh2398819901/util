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


#include "configure.h"
#include <QSysInfo>


Configure::Configure(const QString &filePath):ConfigureBase(filePath)
{
    readConfig();
    writeConfig();
}

Configure::~Configure()
{
    writeConfig();
    delete m_ini;
}

Configure *Configure::Instance(const QString &filePath)
{
    static Configure *instance{};
    if (!instance) {
        instance = new Configure(filePath);
    }
    return instance;
}


bool Configure::readConfig()
{
    ConfigureBase::readConfig();



    return true;
}

bool Configure::writeConfig()
{
    ConfigureBase::writeConfig();


    m_ini->sync();
    return true;
}
