#!/bin/bash
# 作者 lwh
# 日期 2023.12.20

# 加载通用函数
source ./util.sh
source ./user_functions.sh
# ***************************************** 函数定义 ********************************************#
function help() {
    echo "Usage: ./install.sh [OPTIONS]"
    echo "Options:"
    echo "  -h, --help                                      显示本帮助文档并退出"
    echo "  -A, --allSettings                               安装依赖 + 系统设置 需要root权限"
    echo "  -a, --allUserSettings                           用户系统设置 + 安装软件 不要使用sudo或root用户执行"
    echo "  -I, --installDepends                            安装依赖 使用sudo或root用户执行"
    echo "  -U, --uninstallDepends                          卸载依赖 使用sudo或root用户执行"
    echo "  -B, --installBuildDepends                       安装编译依赖 仅开发环境需要安装 用户请勿安装 使用sudo或root用户执行"
    echo "  -b, --uninstallBuildDepends                     卸载编译依赖 使用sudo或root用户执行"
    echo "  -S, --systemSetting                             系统设置 需要root权限"
    echo "  -s, --userSettings                              用户系统设置 不要使用sudo或root用户执行"
    echo "  -i, --installSoft                               安装软件 不要使用sudo或root用户执行"
    echo "  -u, --uninstallSoft                             卸载软件 不要使用sudo或root用户执行"
    
    echo ""
    echo ""
    
    echo "首次安装请使用 -A 安装依赖和系统设置，设置后无需重复设置"
    echo "之后请使用 -a 选项设置用户系统设置及安装软件，设置后无需重复设置"
    echo "首次安装示例流程:"
    echo "sudo bash ./install.sh -A  (不要重启)"
    echo "bash ./install.sh -a  (重启)"
    echo ""
    echo "升级软件请使用 -i 选项安装或升级软件"
    exit 0
}

# 安装软件依赖
function installDepends() {
    check_root
    if [[ $? -ne 0 ]]; then
        echo "请使用root用户运行"
        exit 1
    fi
    
    apt_update
    if [[ $? -ne 0 ]]; then
        exit 1
    fi
    
    for i in ${s_dependList[@]}; do
        check_and_install_package $i
        if [[ $? -ne 0 ]]; then
            echo -e "\e[31m安装依赖 $i 失败\e[0m"
        fi
    done
}

# 卸载软件依赖
function uninstallDepends() {
    check_root
    if [[ $? -ne 0 ]]; then
        echo "请使用root用户运行"
        exit 1
    fi
    
    for i in ${s_dependList[@]}; do
        check_and_uninstall_package $i
        if [[ $? -ne 0 ]]; then
            echo -e "\e[31m卸载依赖 $i 失败\e[0m"
        fi
    done
}

# 安装编译软件依赖
function installBuildDepends() {
    check_root
    if [[ $? -ne 0 ]]; then
        echo "请使用root用户运行"
        exit 1
    fi
    
    apt_update
    if [[ $? -ne 0 ]]; then
        exit 1
    fi
    
    for i in ${s_buildDependList[@]}; do
        check_and_install_package $i
        if [[ $? -ne 0 ]]; then
            echo -e "\e[31m安装编译依赖 $i 失败\e[0m"
        fi
    done
}

# 卸载编译软件依赖
function uninstallBuildDepends() {
    check_root
    if [[ $? -ne 0 ]]; then
        echo "请使用root用户运行"
        exit 1
    fi
    
    for i in ${s_buildDependList[@]}; do
        check_and_uninstall_package $i
        if [[ $? -ne 0 ]]; then
            echo -e "\e[31m卸载编译依赖 $i 失败\e[0m"
        fi
    done
}

# 设置系统设置
function systemSetting() {
    check_root
    if [[ $? -ne 0 ]]; then
        echo "请使用root用户运行"
        exit 1
    fi
    
    if [[ $s_userName == "root" ]]; then
        echo -e "\e[31m当前用户名配置为root，建议修改配置文件内userName字段\e[0m"
        read -rsp $'按回车键继续... 或者 ctrl + c 结束安装\n'
    fi
    
    # 判断用户是否存在
    check_user_exist $s_userName
    if [[ $? -ne 0 ]]; then
        exit 1
    fi
    
    system_settings $s_userName
}

function userSettings() {
    user_settings
}

# 安装软件 使用普通用户安装 root用户警告
installSoft() {
    if [[ $EUID -eq 0 ]]; then
        echo -e "\e[31m当前使用root用户安装，建议使用普通用户安装\e[0m"
        read -rsp $'按回车键继续... 或者 ctrl + c 结束安装\n'
    fi
    
    local userName=$s_userName
    local softName=$s_softName
    local softDir=/home/$userName/.local/bin/$softName
    
    echo "默认停止当前监控进程"
    pkill $softName
    pkill -f /home/$userName/.local/bin/$softName/$softName
    pkill -f ~/.local/bin/$softName/$softName
    
    # 判断用户是否存在
    check_user_exist $userName
    
    # 创建目录
    mkdir -p "$softDir"
    if [ ! -d "$softDir" ]; then
        echo -e "\e[31m创建目录失败  $softDir\e[0m"
        exit 1
    fi
    
    # 复制软件
    if [ ! -d "$s_buildType" ]; then
        echo -e "\e[31m$s_buildType 文件夹不存在\e[0m"
        exit 1
    else
        copy_files "./$s_buildType/$softName" "$softDir"
    fi
    # 复制logo
    if [ ! -f "logo.png" ]; then
        echo -e "\e[31mlogo.png文件不存在\e[0m"
    else
        copy_files logo.png "$softDir"
    fi
    
    # 复制desktop文件
    if [ ! -f "$softName.desktop" ]; then
        echo -e "\e[31m$softName.desktop文件不存在\e[0m"
    else
        # 复制配置文件，然后替换其中的[user]为当前用户名
        copy_files "$softName.desktop" "$softDir"
        sed -i "s/\[user\]/$userName/g" "$softDir/$softName.desktop"
        
        if [ ! -d /home/$userName/.local/share/applications ]; then
            mkdir -p /home/$userName/.local/share/applications
            if [ ! -d /home/$userName/.local/share/applications ]; then
                echo -e "\e[31m创建目录失败 /home/$userName/.local/share/applications\e[0m"
                exit 1
            fi
        fi
        copy_files "$softDir/$softName.desktop" /home/$userName/.local/share/applications
        
        if [ ! -d /home/$userName/.config/autostart/ ]; then
            mkdir -p /home/$userName/.config/autostart/
            if [ ! -d /home/$userName/.config/autostart/ ]; then
                echo -e "\e[31m创建目录失败 /home/$userName/.config/autostart/\e[0m"
                exit 1
            fi
        fi
        copy_files "$softDir/$softName.desktop" /home/$userName/.config/autostart/
        
        if [ ! -d /home/$userName/桌面 ]; then
            mkdir -p /home/$userName/桌面
            if [ ! -d /home/$userName/桌面 ]; then
                echo -e "\e[31m创建目录失败 /home/$userName/桌面\e[0m"
                exit 1
            fi
        fi
        
        copy_files "$softDir/$softName.desktop" /home/$userName/桌面
    fi
    
    if [[ $s_lnName == "" ]]; then
        echo "未指定软连接名，使用desktop内名称"
        s_lnName=$(read_iniFile_field "$softDir/$softName.desktop" "Name")
    fi
    
    if [[ $s_lnName != "" ]]; then
        if [ ! -d /home/$userName/桌面/$s_lnName ]; then
            ln -s "$softDir/" /home/$userName/桌面/$s_lnName
        fi
    fi
    
    install_soft
    echo "安装完成"
}

# 卸载软件
function uninstallSoft() {
    local userName=$s_userName
    local softName=$s_softName
    # 删除应用程序和桌面快捷方式
    rm -f /home/$userName/.local/share/applications/$softName.desktop
    rm -f /home/$userName/桌面/$softName.desktop
    rm -rf /home/$userName/.config/autostart/media$softNameClient.desktop
    
    # 删除安装目录和文件
    softDir=/home/$userName/.local/bin/$softName
    if [ -d "$softDir" ]; then
        # 删除快捷方式
        if [[ $s_lnName == "" ]]; then
            echo "未指定软连接名，使用desktop内名称"
            s_lnName=$(read_iniFile_field "$softDir/$softName.desktop" "Name")
        fi
        # 判断是否为软链接
        if [ -L ~/桌面/$s_lnName ]; then
            rm -f ~/桌面/$s_lnName
        fi
        
        rm -rf "$softDir"
        echo "已删除安装目录 $softDir"
    else
        echo "安装目录 $softDir 不存在"
    fi
    
    uninstall_soft
    echo "卸载完成"
}

function main() {
    check_os_version
    if [[ $? -ne 0 ]]; then
        exit 1
    fi
    
    if [[ $1 == "-h" || $1 == "--help" ]]; then
        help
        exit 0
    fi
    
    if [[ $1 == "-A" || $1 == "--allSettings" ]]; then
        echo "系统设置"
        systemSetting
        installDepends
        echo "系统设置完成"
        promptReboot
        exit 0
    fi
    
    if [[ $1 == "-a" || $1 == "--allUserSettings" ]]; then
        echo "用户设置"
        userSettings
        installSoft
        echo "用户设置完成"
        promptReboot
        exit 0
    fi
    
    if [[ $1 == "-S" || $1 == "--systemSetting" ]]; then
        systemSetting $s_userName
        promptReboot
        exit 0
    fi
    
    if [[ $1 == "-s" || $1 == "--userSettings" ]]; then
        userSettings
        promptReboot
        exit 0
    fi
    
    if [[ $1 == "-I" || $1 == "--installDepends" ]]; then
        installDepends
        exit 0
    fi
    
    if [[ $1 == "-U" || $1 == "--uninstallDepends" ]]; then
        uninstallDepends
        exit 0
    fi
    
    if [[ $1 == "-i" || $1 == "--installSoft" ]]; then
        installSoft
        exit 0
    fi
    
    if [[ $1 == "-u" || $1 == "--uninstallSoft" ]]; then
        uninstallSoft
        exit 0
    fi
    
    if [[ $1 == "-B" || $1 == "--installBuildDepends" ]]; then
        installBuildDepends
        exit 0
    fi
    
    if [[ $1 == "-b" || $1 == "--uninstallBuildDepends" ]]; then
        uninstallBuildDepends
        exit 0
    fi
    
    help
}

# ***************************************** 运行 ********************************************#
s_userName=$(read_iniFile_field "install.ini" "userName")                       # 用户名
s_softName=$(read_iniFile_field "install.ini" "softName")                       # 软件名
s_dependList=$(read_iniFile_field "install.ini" "dependList")                   # 依赖列表
s_buildDependList=$(read_iniFile_field "install.ini" "buildDependList")         # 构建依赖列表
s_lnName=$(read_iniFile_field "install.ini" "lnName")                           # 软链接名
s_buildType=$(read_iniFile_field "install.ini" "buildType")                     # 构建类型，默认为 Release

if [[ $s_userName == "" ]]; then
    # 获取id 1000的用户名
    s_userName=$(id -u -n 1000)
    echo "未指定用户名，默认使用 $s_userName"
fi

if [[ $s_softName == "" ]]; then
    echo "未指定软件名，退出"
    exit 1
fi

if [[ $s_dependList == "" ]]; then
    echo "未指定依赖列表"
fi

if [[ $s_buildDependList == "" ]]; then
    echo "未指定编译依赖列表"
fi

if [[ $s_buildType == "" ]]; then
    echo "未指定编译类型，默认为Release"
    s_buildType="Release"
fi

main "$@"
