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

QString Configure::getUUID()
{
    if(getDebug() == "debug"){
        return getDebugSystemSn();
    }else if(getDebug() == "release_test"){
        return getReleaseTestSystemSn();
    }else{
        return getSystemSn();
    }
}

bool Configure::readConfig()
{
    ConfigureBase::readConfig();

    getValue("system", "rsa_pubkey",  "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCASPyNBddGZh7KeHdMr6+4bIQEniZ1lZCTpPm1ZsOU7YJEm0WkA272/qQYICvDDtKA/jQ+dStAIKtF4lBUNGlC4iPyd+pTa5lBbsMv33yBCmlwtO9IQA5wFwfj1kElF29h5cI+VUKU7qlN6wg7kqJi6lc3/r+Y6yQDGzM+g8wX5wIDAQAB");

    getValue("Service","websocketServerIp","36.134.40.96");
    getValue("Service","websocketPort","8111");
    getValue("Service","websocketPrefix","/api");
    getValue("Service","useWebPrefix","yes");
    if(getUseWebPrefix() != "yes"){
        m_values["Service"]["websocketPrefix"] = "";
    }

    getValue("Service","httpServerIp","36.134.40.96");
    getValue("Service","httpPort","8111");
    getValue("Service","httpPrefix","/api");
    getValue("Service","useHttpPrefix","yes");
    if(getUseHttpPrefix() != "yes"){
        m_values["Service"]["httpPrefix"] = "";
    }

    m_values["system"]["sn"] = QSysInfo::machineUniqueId(); //只读数据 不从配置文件中读取
    if(getDebug() == "debug"){
        getValue("test","debugSn","54afea6ee5af42bdbfa6a1e436f2aea5");
    }

    if(getDebug() == "release_test"){
        getValue("test","releaseSn","");
    }

    getValue("Service","shutdownTime","");
    getValue("Service","heartBeat","60000");
    getValue("Service","uploadInterval","60000");

    getValue("test","userAes","no");

    return true;
}

bool Configure::writeConfig()
{
    ConfigureBase::writeConfig();

    setValue("Service","websocketServerIp","");
    setValue("Service","websocketPort","");
    setValue("Service","websocketPrefix","");
    setValue("Service","useWebPrefix","");
    setValue("Service","httpServerIp","");
    setValue("Service","httpPort","");
    setValue("Service","httpPrefix","");
    setValue("Service","useHttpPrefix","");

    setValue("system", "sn", "");
    if(getDebug() == "debug"){
        setValue("test","debugSn","");
    }
    if(getDebug() == "release_test"){
        setValue("test","releaseSn","");
    }

    setValue("Service","shutdownTime","");
    setValue("Service","heartBeat","");
    setValue("Service","uploadInterval","");
    setValue("test","userAes","");
    m_ini->sync();
    return true;
}
