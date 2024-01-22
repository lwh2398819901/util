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

#ifndef CONFIGURE_H
#define CONFIGURE_H

#include "configurebase.h"

//该文件为模板文件，每一个用户都需要自己实现该文件，如果不需要，可以保持空文件

class Configure:public ConfigureBase
{
    explicit Configure(const QString &filePath);
    Configure(const Configure &) = delete;
    void operator=(const Configure &) = delete;
public:
    ~Configure();
    static Configure *Instance(const QString &filePath = "./config.ini");
    
    bool readConfig();
    bool writeConfig();
};





#endif // CONFIGURE_H
