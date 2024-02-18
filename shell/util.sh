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

# 打印绿色的正常打印
function print_success() {
    local message=$1
    echo -e "\e[32m[SUCCESS] $message\e[0m"
}

# 打印红色的错误打印
function print_error() {
    local message=$1
    echo -e "\e[31m[ERROR] $message\e[0m"
}

# 打印黄色的警告
function print_warning() {
    local message=$1
    echo -e "\e[33m[WARNING] $message\e[0m"
}

# 正常打印
function print_info() {
    local message=$1
    echo "[INFO] $message"
}

# 判断用户是否存在
function check_user_exist() {
    local userName=$1
    # 判断用户名是否为空
    if [[ -z $userName ]]; then
        print_error "未指定用户名"
        return 1
    fi

    # 判断用户是否存在
    if id -u $userName >/dev/null 2>&1; then
        print_info "用户 $userName 存在"
        return 0
    else
        print_error "用户 $userName 不存在"
        return 1
    fi
}

# 判断是否为root用户
function check_root() {
    if [[ $EUID -ne 0 ]]; then
        print_error "非root用户"
        return 1
    fi
    print_info "root用户"
    return 0
}

# 判断是否为Ubuntu版本
function check_ubuntu_version() {
    local os_version=$1
    if [[ -z $os_version ]]; then
        print_error "未指定Ubuntu版本"
        return 1
    fi

    local version=$(lsb_release -r | awk '{print $2}')
    if [[ $version != $os_version ]]; then
        print_error "该脚本仅适用于Ubuntu $os_version 版本！"
        return 1
    fi
    return 0
}

# 执行apt update命令
function apt_update() {
    apt update
    if [[ $? -ne 0 ]]; then
        print_error "执行apt update命令出错，请检查网络连接和软件源配置！\e[0m"
        return 1
    fi
    return 0
}

# 检查并安装软件包
function check_and_install_package() {
    local package_name=$1
    if dpkg -s $package_name >/dev/null 2>&1; then
        print_success "$package_name 已经安装！"
    else
        apt install -y $package_name
        if [[ $? -ne 0 ]]; then
            print_error "安装 $package_name 出错！"
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
            print_error "卸载 $package_name 出错！"
        fi
    else
        print_info "$package_name 未安装！"
    fi
}

# 创建文件夹
function create_dir() {
    if [ ! -d "$1" ]; then
        mkdir -p "$1"
        if [[ $? -ne 0 ]]; then
            print_error "创建文件夹 $1 失败！"
            return 1
        fi
    else
        print_info "文件夹 $1 已存在！"
    fi
    return 0
}

# 内部拷贝函数 不对外使用
function pri_copyFiles() {
    # 源目录
    local sourceDir="$1"
    # 目标目录
    local targetDir="$2"

    # 判断源文件与目标文件是否相同
    if [ "$sourceDir" == "$targetDir" ]; then
        return 0
    fi

    isDir=0

    # 判断是文件还是目录
    if [ -f "$sourceDir" ]; then
        isDir=0
    elif [ -d "$sourceDir" ]; then
        isDir=1
    else
        print_error "源文件不存在！"
        return 1
    fi

    # 创建目标目录（如果不存在）
    if [ ! -d "$targetDir" ]; then
        create_dir "$targetDir"
        if [[ $? -ne 0 ]]; then
            print_error "创建目标目录失败！"
            return 1
        fi
    fi

    if [ $isDir -eq 1 ]; then
        cp -r "$sourceDir" "$targetDir"
    else
        # 判断目标文件是否已经存在，存在 判断是否相同  不同则拷贝
        if [ -f "$targetDir/$(basename "$sourceDir")" ]; then
            if cmp -s "$sourceDir" "$targetDir/$(basename "$sourceDir")"; then
                return 0
            fi
        fi
        # 拷贝文件
        cp "$sourceDir" "$targetDir"
        if [[ $? -ne 0 ]]; then
            print_error "拷贝文件 $sourceDir 到 $targetDir 出错！"
            return 1
        fi
    fi
    print_success "文件 $sourceDir 拷贝到 $targetDir 成功！"
    return 0
}

# 定义拷贝文件的函数
# args: sourceDir targetDir
function copy_files() {
    if [[ $# -eq 2 ]]; then
        pri_copyFiles "$1" "$2"
        if [[ $? -ne 0 ]]; then
            return 1
        fi
    else
        last_arg=${!#}
        for tmp in "${@:1:$#-1}"; do
            pri_copyFiles "$tmp" "$last_arg"
            if [[ $? -ne 0 ]]; then
                return 1
            fi
        done
    fi

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
    echo "$value" # 返回字段值  使用echo打印
}
