#!/bin/bash
# 加载通用函数
source ./util.sh
# ***************************************** 函数定义 ********************************************#

function help() {
    echo "Usage: ./config.sh [options]"
    echo "Options:"
    echo "  -h, --help     显示本帮助文档并退出"
    echo "  -c, --clean    清理目标目录"
    echo "  -C, --cleanAll 清理构建目录和目标目录"
    echo "  -s, --settings 打开设置文件，配置脚本设置"
    echo ""
    echo "Example:"
    echo "  ./config.sh -c"
    echo "  ./config.sh -C"
    echo "  ./config.sh -s"
    echo "  ./config.sh   自动打包并安装"
    exit 0
}

function build_and_copy_software() {
    create_dir "$s_targetDir"
    if [ $? -ne 0 ]; then
        echo -e "\e[31m创建$s_targetDir 目录失败: $s_targetDir\e[0m"
        exit 1
    fi
    
    create_dir "$s_buildDir"
    if [ $? -ne 0 ]; then
        echo -e "\e[31m创建$s_buildDir 目录失败: $s_buildDir\e[0m"
        exit 1
    fi
    cd "$s_buildDir"
    
    cmake_cmd=$(cmake -DCMAKE_BUILD_TYPE="$s_buildType" "$s_sourceDir")
    if [ $? -ne 0 ]; then
        echo -e "\e[31mCMake 命令执行失败: $cmake_cmd\e[0m"
        exit 1
    fi
    
    make_cmd=$(make)
    if [ $? -ne 0 ]; then
        echo -e "\e[31mmake 命令执行失败: $make_cmd\e[0m"
        exit 1
    fi
    
    copy_files "./bin/*" "${s_targetDir}/${s_buildType}"
    if [ $? -ne 0 ]; then
        echo -e "\e[31m复制可执行程序$s_softName 失败\e[0m"
        exit 1
    fi
    
    copy_files "${s_sourceDir}/resource/install/" "$s_targetDir"
    if [ $? -ne 0 ]; then
        echo -e "\e[31m复制 install目录 文件失败\e[0m"
        exit 1
    fi
    
    mv $s_targetDir/install/*  $s_targetDir
    if [ $? -ne 0 ]; then
        echo -e "\e[31m移动文件失败\e[0m"
        exit 1
    fi
    
    copy_files "$s_currentDir/util.sh" $s_targetDir
    if [ $? -ne 0 ]; then
        echo -e "\e[31m复制 util.sh 文件失败\e[0m"
        exit 1
    fi
    
    copy_files "$s_currentDir/install.sh" $s_targetDir
    if [ $? -ne 0 ]; then
        echo -e "\e[31m复制 install.sh 文件失败\e[0m"
        exit 1
    fi
    
    copy_files "$s_currentDir/user_functions.sh" $s_targetDir
    if [ $? -ne 0 ]; then
        echo -e "\e[31m复制 user_functions.sh 文件失败\e[0m"
        exit 1
    fi
    
    copy_files "$s_currentDir/install.ini" $s_targetDir
    if [ $? -ne 0 ]; then
        echo -e "\e[31m复制 install.ini 文件失败\e[0m"
        exit 1
    fi
    rm -rf $s_targetDir/install
}

function create_zip_file() {
    cd "$s_sourceDir"
    local current_date=$(date +"%m.%d")
    local current_tag=$(git describe --tags --abbrev=0)
    
    if [ -z "$current_tag" ]; then
        current_tag=$(date +"%H%M%S")
    fi
    
    cd "$s_targetDir"
    local zip_file_name="${s_softName}_${current_tag}_${s_buildType}_${current_date}.zip"
    zip -r "$s_zipFilePath/$zip_file_name" "."
    if [ $? -ne 0 ]; then
        echo -e "\e[31m创建压缩包失败\e[0m"
        exit 1
    fi
    
    echo "压缩包创建完成：$s_zipFilePath/$zip_file_name"
}

function cleanTarget () {
    rm -rf "$s_targetDir"
    rm -rf $s_zipFilePath/$s_softName*.zip
    exit 0
}

function cleanBuildDir () {
    rm -rf "$s_buildDir"
    exit 0
}

function main() {
    build_and_copy_software
    if [ $? -ne 0 ]; then
        echo -e "\e[31m构建失败\e[0m"
        exit 1
    fi
    
    create_zip_file
    if [ $? -ne 0 ]; then
        echo -e "\e[31m创建压缩包失败\e[0m"
        exit 1
    fi
    
    cd $s_targetDir
    bash install.sh -i
}

function printValue() {
    echo -e "\e[32mcurrentDir: $s_currentDir\e[0m"
    echo ""
    echo -e "\e[32msoftName: $s_softName\e[0m"
    echo ""
    echo -e "\e[32msourceDir: $s_sourceDir\e[0m"
    echo ""
    echo -e "\e[32mbuildDir: $s_buildDir\e[0m"
    echo ""
    echo -e "\e[32mtargetDir: $s_targetDir\e[0m"
    echo ""
    echo -e "\e[32mzipFilePath: $s_zipFilePath\e[0m"
    echo ""
    echo -e "\e[32mbuildType: $s_buildType\e[0m"
    echo ""
    echo -e "\e[32muserName: $s_userName\e[0m"
    echo ""
    echo -e "\e[32mdependList: $s_dependList\e[0m"
    echo ""
    echo -e "\e[32mbuildDependList: $s_buildDependList\e[0m"
    echo ""
    echo -e "\e[32mlnName: $s_lnName\e[0m"
    echo ""
}

function create_user_functions_sh() {
    echo -e "\e[31muser_functions.sh文件不存在，创建user_functions.sh模版文件\e[0m"
    touch user_functions.sh
    echo '#!/bin/bash' > user_functions.sh
    echo 'source ./util.sh' >> user_functions.sh
    echo '' >> user_functions.sh
    
    echo 'function system_settings() {' >> user_functions.sh
    echo '    echo "用户自定义 系统设置"' >> user_functions.sh
    echo '}' >> user_functions.sh
    echo '' >> user_functions.sh
    
    
    echo 'function user_settings() {' >> user_functions.sh
    echo '    echo "用户自定义 用户设置"' >> user_functions.sh
    echo '}' >> user_functions.sh
    echo '' >> user_functions.sh
    
    
    echo 'function install_soft() {' >> user_functions.sh
    echo '    echo "用户自定义 安装软件"' >> user_functions.sh
    echo '}' >> user_functions.sh
    echo '' >> user_functions.sh
    
    echo 'function uninstall_soft() {' >> user_functions.sh
    echo '    echo "用户自定义 卸载软件"' >> user_functions.sh
    echo '}' >> user_functions.sh
    echo '' >> user_functions.sh
    
    
    echo 'function check_os_version() {' >> user_functions.sh
    echo '    check_ubuntu_version "22.04"' >> user_functions.sh
    echo '    if [[ $? -ne 0 ]]; then' >> user_functions.sh
    echo '        echo -e "\e[31m当前系统版本不是Ubuntu 22.04 桌面版\e[0m"' >> user_functions.sh
    echo '        return 1' >> user_functions.sh
    echo '    fi' >> user_functions.sh
    echo '    return 0' >> user_functions.sh
    echo '}' >> user_functions.sh
    echo '' >> user_functions.sh
    
    echo -e "\e[32muser_functions.sh文件创建成功\e[0m"
    xdg-open ./user_functions.sh
}

function create_install_ini() {
    touch install.ini
    echo "[install]" > install.ini
    echo "userName=$s_userName" >> install.ini
    echo "softName=$s_softName" >> install.ini
    echo "dependList=$s_dependList" >> install.ini
    echo "buildDependList=$s_buildDependList" >> install.ini
    echo "lnName=$s_lnName" >> install.ini
    echo "buildType=$s_buildType" >> install.ini
}

function create_project_ini() {
    echo -e "\e[31m配置文件不存在，创建配置文件\e[0m"
    touch project.ini
    echo "softName=appName" > project.ini
    echo "sourceDir=/home/Template/sourceDir" >> project.ini
    echo "buildDir=/home/Template/buildDir" >> project.ini
    echo "targetDir=/home/Template/amd64" >> project.ini
    echo "zipPath=/home/Template/" >> project.ini
    echo "buildType=Release" >> project.ini
    echo "userName=" >> project.ini
    echo "dependList=" >> project.ini
    echo "buildDependList=" >> project.ini
    echo "lnName=" >> project.ini
    echo -e "\e[32m配置文件创建成功\e[0m"
    xdg-open ./project.ini
}


function check_project_ini() {
    # 判断是否使用的是模版的内容，如果是模版内容则提示退出
    if [ "$s_softName" = "appName" ] && [ "$s_sourceDir" = "/home/Template/sourceDir" ] && [ "$s_buildDir" = "/home/Template/buildDir" ] && [ "$s_targetDir" = "/home/Template/amd64" ] && [ "$s_zipFilePath" = "/home/Template/" ] ; then
        echo -e "\e[31m请修改配置文件 project.ini 中的内容\e[0m"
        echo "使用 ./config.sh -s"
        exit 1
    fi
}

# ***************************************** 运行 ********************************************#
isExit=0
# 判断配置文件是否存在
if [ ! -f "./project.ini" ]; then
    create_project_ini
    isExit=1
fi

# 判断配置文件是否存在
if [ ! -f "./user_functions.sh" ]; then
    create_user_functions_sh
    isExit=1
fi

if [ $isExit -eq 1 ]; then
    exit 0
fi

# 设置默认值 全局变量使用s_开头
s_currentDir=$(pwd)
s_softName=$(read_iniFile_field "project.ini" "softName")       # 软件名称  appName
s_sourceDir=$(read_iniFile_field "project.ini" "sourceDir")     # 源码目录      /home/test/sourceDir
s_buildDir=$(read_iniFile_field "project.ini" "buildDir")       # 构建目录      /home/test/buildDir
s_targetDir=$(read_iniFile_field "project.ini" "targetDir")     # 目标目录      /home/test/amd64
s_zipFilePath=$(read_iniFile_field "project.ini" "zipPath")     # zip文件目录   /home/test/
s_buildType=$(read_iniFile_field "project.ini" "buildType")     # 构建类型，默认为 Release
s_userName=$(read_iniFile_field "project.ini" "userName")
s_dependList=$(read_iniFile_field "project.ini" "dependList")
s_buildDependList=$(read_iniFile_field "project.ini" "buildDependList")
s_lnName=$(read_iniFile_field "project.ini" "lnName")

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
        -h | --help)
            help
            exit 0
        ;;
        -c | --clean)
            cleanTarget
            echo "清理完成"
            exit 0
        ;;
        -C | --cleanAll)
            cleanTarget
            cleanBuildDir
            echo "清理完成"
            exit 0
        ;;
        -s | --settings)
            xdg-open ./project.ini
            exit 0
        ;;
        *)
            echo -e "\e[31mUnknown option: $1\e[0m"
            help
            exit 1
        ;;
    esac
done

check_project_ini
create_install_ini

main