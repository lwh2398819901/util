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

#ifndef CONFIGUREBASE_H
#define CONFIGUREBASE_H
#include <QMap>
#include <QSettings>
#include <QString>
#include <QFile>
#include "version.h"

#define  getSetString(funcName,arg1,feild,key) inline QString get##funcName(){ return m_values[feild][key];}\
inline void set##funcName(const QString& arg1){ m_values[feild][key] = arg1;\
        writeConfig();}

#define  getSetInt(funcName,arg1,feild,key) inline int get##funcName(){ return m_values[feild][key].toInt();}\
inline void set##funcName(const int& arg1){ m_values[feild][key] = QString::number(arg1);\
        writeConfig();}

#define  getSetUint(funcName,arg1,feild,key) inline uint get##funcName(){ return m_values[feild][key].toUInt();}\
inline void set##funcName(const uint& arg1){ m_values[feild][key] = QString::number(arg1);\
        writeConfig();}

#define  getSetQuint16(funcName,arg1,feild,key) inline quint16 get##funcName(){ return m_values[feild][key].toUShort();}\
inline void set##funcName(const quint16& arg1){ m_values[feild][key] = QString::number(arg1);\
        writeConfig();}

#define  getSetQStringList(funcName,arg1,feild,key) inline QStringList get##funcName(){\
QStringList list;\
    foreach (auto it, m_values[feild][key].split(",")) {\
        if (!it.isEmpty()) {\
            list.append(it);\
    }\
}\
    return list;}\
    inline void set##funcName(const QStringList& arg1){ m_values[feild][key] =arg1.join(",");\
        writeConfig();}




class ConfigureBase
{
public:
    explicit ConfigureBase(const QString &filePath);
    ConfigureBase(const ConfigureBase &) = delete;
    void operator=(const ConfigureBase &) = delete;
    ~ConfigureBase();
    bool readConfig();
    bool writeConfig();
    QString getValue(QString field, QString key, QString defaultValue);
    void setValue(QString field, QString key, QString value);
    void setValue(QString field, QString key);

    getSetString(SoftName, name, "config", "softName")                              // 软件名称
    getSetString(Version, version, "config", "version")                             // 软件版本
    getSetString(Debug, debug, "config", "debug")                                   // 软件功能控制：debug/release/release_test
    getSetString(SingletonProcess, singletonProcess, "config", "singletonProcess")  // 进程是否为单例
    getSetString(UseNetworkProxy, useNetworkProxy, "config", "useNetworkProxy")     // 是否使用网络代理

    getSetString(DefaultMaxFileSize, maxFileSize, "Log", "defaultMaxFileSize")      // 日志文件大小
    getSetString(DefaultMaxFiles, maxFiles, "Log", "defaultMaxFiles")               // 日志文件数量
    getSetString(DefaultLevel, level, "Log", "defaultLevel")                        // 日志级别
    getSetString(DefaultFilePath, filePath, "Log", "defaultFilePath")               // 日志文件路径

protected:
    QMap<QString, QMap<QString, QString>> m_values;                         
    QSettings *m_ini{};                                                        
};





#endif // CONFIGUREBASE_H
