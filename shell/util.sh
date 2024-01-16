#!/bin/bash

# 作者：lwh
# 说明：统一shell函数库
# 编写时间:2024.1.5
# 版本：1.0.0
# 使用示例：
#   创建调用shell文件 例如test.sh
#   编写下面内容
#   #!/bin/bash
#   # 加载或源入包含函数的脚本
#   source ./util.sh
#   调用函数
#   check_user_exist root

# 判断用户是否存在
function check_user_exist() {
    local userName=$1
    # 判断用户名是否为空
    if [[ -z $userName ]]; then
        echo "未指定用户名"
        return 1
    fi
    
    # 判断用户是否存在
    if id -u $userName >/dev/null 2>&1; then
        echo "用户 $userName 存在"
        return 0
    else
        echo "用户 $userName 不存在"
        return 1
    fi
}

# 判断是否为root用户
function check_root() {
    if [[ $EUID -ne 0 ]]; then
        echo "非root用户"
        return 1
    fi
    echo "root用户"
    return 0
}

# 判断是否为Ubuntu版本
function check_ubuntu_version() {
    local os_version=$1
    if [[ -z $os_version ]]; then
        echo "未指定Ubuntu版本"
        return 1
    fi
    
    local version=$(lsb_release -r | awk '{print $2}')
    if [[ $version != $os_version ]]; then
        echo "该脚本仅适用于Ubuntu $os_version 版本！"
        return 1
    fi
    return 0
}

# 执行apt update命令
function apt_update() {
    apt update
    if [[ $? -ne 0 ]]; then
        echo "执行apt update命令出错，请检查网络连接和软件源配置！"
        return 1
    fi
    return 0
}

# 检查并安装软件包
function check_and_install_package() {
    local package_name=$1
    if dpkg -s $package_name >/dev/null 2>&1; then
        echo "$package_name 已经安装！"
    else
        apt install -y $package_name
        if [[ $? -ne 0 ]]; then
            echo "安装 $package_name 出错！"
            return 1
        fi
    fi
}

# 检查并卸载软件包
function check_and_uninstall_package() {
    local package_name=$1
    if dpkg -s $package_name >/dev/null 2>&1; then
        apt remove -y $package_name
        if [[ $? -ne 0 ]]; then
            echo "卸载 $package_name 出错！"
        fi
    else
        echo "$package_name 未安装！"
    fi
}
#
function create_dir() {
    if [ ! -d "$1" ]; then
        mkdir -p "$1"
        if [[ $? -ne 0 ]]; then
            return 1
        fi
    fi
    return 0
}

# 定义拷贝文件的函数
function copy_files() {
    # 源目录
    local sourceDir="$1"
    # 目标目录
    local targetDir="$2"
    
    # 判断源文件与目标文件是否相同
    if [ "$sourceDir" == "$targetDir" ]; then
        echo "源文件与目标文件相同！"
        return 0
    fi
    
    isDir=0
    
    # 判断是文件还是目录
    if [ -f "$sourceDir" ]; then
        isDir=0
        elif [ -d "$sourceDir" ]; then
        isDir=1
    else
        echo "源文件不存在！"
        return 1
    fi
    
    # 创建目标目录（如果不存在）
    if [ ! -d "$targetDir" ]; then
        create_dir "$targetDir"
        if [[ $? -ne 0 ]]; then
            echo "创建目标目录失败！"
            return 1
        fi
    fi
    
    if [ $isDir -eq 1 ]; then
        cp -r "$sourceDir" "$targetDir"
    else
        # 判断目标文件是否已经存在，存在 判断是否相同  不同则拷贝
        if [ -f "$targetDir/$(basename "$sourceDir")" ]; then
            if cmp -s "$sourceDir" "$targetDir/$(basename "$sourceDir")"; then
                echo "文件 $sourceDir 和 $targetDir/$(basename "$sourceDir") 相同，无需拷贝"
                return
            fi
        fi
        # 拷贝文件
        cp "$sourceDir" "$targetDir"
        if [[ $? -ne 0 ]]; then
            echo "拷贝文件 $sourceDir 到 $targetDir 出错！"
            return 1
        fi
    fi
    echo "文件 $sourceDir 拷贝到 $targetDir 成功！"
    return 0
}

function promptReboot() {
    read -p "是否重启系统以应用更改？(y/n): " answer
    if [[ $answer == "y" || $answer == "Y" ]]; then
        reboot
    fi
}

# 读取指定字段
function read_iniFile_field() {
    # 指定.desktop文件路径
    local filePath=$1
    local field=$2
    # 使用grep提取Name字段的值
    local value=$(grep -oP '^'$field'=\K.*' "$filePath")
    echo "$value"
}


