#!/bin/bash
source ./util.sh

function system_settings() {
    echo "用户自定义 系统设置"
}

function user_settings() {
    echo "用户自定义 用户设置"
}

function install_soft() {
    echo "用户自定义 安装软件"
}

function uninstall_soft() {
    echo "用户自定义 卸载软件"
}

function check_os_version() {
    check_ubuntu_version "22.04"
    if [[ $? -ne 0 ]]; then
        echo -e "\e[31m当前系统版本不是Ubuntu 22.04 桌面版\e[0m"
        return 1
    fi
    return 0
}