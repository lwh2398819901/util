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

function cleanTarget () {
    rm -rf "$s_targetDir"
    rm -rf $s_zipFilePath/$s_softName*.zip
}

function cleanBuildDir () {
    rm -rf "$s_buildDir"
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
    echo '    echo "用户自定义 检查系统版本 这是一个示例 请自行修改"' >> user_functions.sh
    echo '    check_ubuntu_version "22.04"' >> user_functions.sh
    echo '    if [[ $? -ne 0 ]]; then' >> user_functions.sh
    echo '        echo -e "\e[31m当前系统版本不是Ubuntu 22.04 桌面版\e[0m"' >> user_functions.sh
    echo '        return 1' >> user_functions.sh
    echo '    fi' >> user_functions.sh
    echo '    return 0' >> user_functions.sh
    echo '}' >> user_functions.sh
    echo '' >> user_functions.sh
    
    echo -e "\e[32muser_functions.sh文件创建成功\e[0m"
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
    
    local script_src=$(cd "../../" && pwd)
    local script_build=$(cd "$script_src/../" && pwd)
    local script_root=$(cd "$script_build/../" && pwd)
    
    # 判断配置文件是否存在
    if [ -f "$script_src/resource/install/project.ini" ]; then
        copy_files "$script_src/resource/install/project.ini" ./
        return 0
    fi
    
    
    # 根据当前脚本位置构建其他路径
    local softName=$(basename $script_src)
    local sourceDir="${script_src}"
    local buildDir="${script_build}/build-$softName-unknown-Release"
    local targetDir="${script_root}/amd64"
    local zipPath="${script_root}"
    local buildType="Release"
    local userName=""
    local dependList="vlc libvlc-dev qt5-default qtmultimedia5-dev libqt5multimedia5-plugins libqt5multimediawidgets5 gstreamer1.0-libav gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly"
    local buildDependList="gcc g++ cmake qtcreator"
    local lnName=""
    
    # 将这些信息写入到project.ini文件
    echo "[project]" > project.ini
    echo "softName=${softName}" >> project.ini
    echo "sourceDir=${sourceDir}" >> project.ini
    echo "buildDir=${buildDir}" >> project.ini
    echo "targetDir=${targetDir}" >> project.ini
    echo "zipPath=${zipPath}" >> project.ini
    echo "buildType=${buildType}" >> project.ini
    echo "userName=${userName}" >> project.ini
    echo "dependList=${dependList}" >> project.ini
    echo "buildDependList=${buildDependList}" >> project.ini
    echo "lnName=${lnName}" >> project.ini
    
    echo -e "\e[32m配置文件创建成功\e[0m"
}


function build_and_copy_software() {
    create_dir "$s_targetDir"
    if [ $? -ne 0 ]; then
        echo -e "\e[31m创建$s_targetDir 目录失败\e[0m"
        exit 1
    fi
    
    create_dir "$s_buildDir"
    if [ $? -ne 0 ]; then
        echo -e "\e[31m创建$s_buildDir 目录失败\e[0m"
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
    
    copy_files $s_buildDir/bin/* $s_targetDir/$s_buildType
    if [ $? -ne 0 ]; then
        echo -e "\e[31m复制可执行程序失败\e[0m"
        exit 1
    fi
    
    copy_files ${s_sourceDir}/resource/install/* $s_targetDir
    if [ $? -ne 0 ]; then
        echo -e "\e[31m复制 install目录文件失败\e[0m"
        exit 1
    fi
    
    
    copy_files $s_currentDir/util.sh $s_targetDir
    if [ $? -ne 0 ]; then
        echo -e "\e[31m复制 util.sh 文件失败\e[0m"
        exit 1
    fi
    
    copy_files $s_currentDir/install.sh $s_targetDir
    if [ $? -ne 0 ]; then
        echo -e "\e[31m复制 install.sh 文件失败\e[0m"
        exit 1
    fi
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

# ***************************************** 运行 ********************************************#
isExit=0
s_currentDir=$(pwd)
# 判断配置文件是否存在
if [ ! -f "./user_functions.sh" ]; then
    create_user_functions_sh
fi

# 判断配置文件是否存在
if [ ! -f "./project.ini" ]; then
    create_project_ini
    echo "请检查配置文件后，再次运行"
    isExit=1
fi

# 设置默认值 全局变量使用s_开头
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

create_install_ini

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


if [[ $isExit -eq 1 ]]; then
    exit 1
fi


printValue

main "$@"