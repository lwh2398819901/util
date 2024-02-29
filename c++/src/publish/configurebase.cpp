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


#include "configurebase.h"





ConfigureBase::ConfigureBase(const QString &filePath)
{
    m_ini = new QSettings(filePath, QSettings::IniFormat);
}

ConfigureBase::~ConfigureBase()
{
    delete m_ini;
}

/*
[config]
version=1.0.0
debug=release

[Log]
maxFileSize=104857600
maxFiles=10
level=info
filePath=./logs/log.log
*/

bool ConfigureBase::readConfig()
{
    m_values["config"]["softName"] = PRODUCT_NAME;
    m_values["config"]["version"] = GIT_PROJECT_VERSION;
    getValue("config", "debug", "release");
    getValue("config", "singletonProcess", "yes");
    getValue("config", "useNetworkProxy", "yes");
    getValue("Log", "defaultMaxFileSize", "104857600");
    getValue("Log", "defaultMaxFiles", "10");
    getValue("Log", "defaultLevel", "info");
    getValue("Log", "defaultFilePath", "./logs/log.log");
    return true;
}

bool ConfigureBase::writeConfig()
{
    setValue("config", "softName");
    setValue("config", "version");
    setValue("config", "debug");
    setValue("config", "singletonProcess");
    setValue("config", "useNetworkProxy");
    setValue("Log", "defaultMaxFileSize");
    setValue("Log", "defaultMaxFiles");
    setValue("Log", "defaultLevel");
    setValue("Log", "defaultFilePath");
    m_ini->sync();
    return true;
}

void ConfigureBase::getValue(QString field, QString key, QString defaultValue)
{
    m_values[field][key] = m_ini->value(field + "/" + key,defaultValue).toString();
}

void ConfigureBase::setValue(QString field, QString key, QString value)
{
    m_values[field][key] = value;
    m_ini->setValue(field + "/" + key, m_values[field][key]);
}

void ConfigureBase::setValue(QString field, QString key)
{
    m_ini->setValue(field + "/" + key, m_values[field][key]);
}
