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
    echo "  ./config.sh "
    exit 0
}

function printValue() {
    print_success "currentDir: $s_currentDir"
    print_success "softName: $s_softName"
    print_success "sourceDir: $s_sourceDir"
    print_success "buildDir: $s_buildDir"
    print_success "targetDir: $s_targetDir"
    print_success "zipFilePath: $s_zipFilePath"
    print_success "buildType: $s_buildType"
    print_success "userName: $s_userName"
    print_success "dependList: $s_dependList"
    print_success "buildDependList: $s_buildDependList"
    print_success "Name: $s_lnName"
    echo ""
}

function cleanTarget() {
    rm -rf "$s_targetDir"
    rm -rf $s_zipFilePath/$s_softName*.zip
}

function cleanBuildDir() {
    rm -rf "$s_buildDir"
}

function create_user_functions_sh() {
    print_warning "user_functions.sh文件不存在，创建user_functions.sh模版文件"
    touch user_functions.sh
    echo '#!/bin/bash' >user_functions.sh
    echo 'source ./util.sh' >>user_functions.sh
    echo '' >>user_functions.sh

    echo 'function system_settings() {' >>user_functions.sh
    echo '    print_info "用户自定义 系统设置"' >>user_functions.sh
    echo '    print_info "$1"' >>user_functions.sh
    echo '}' >>user_functions.sh
    echo '' >>user_functions.sh

    echo 'function user_settings() {' >>user_functions.sh
    echo '    print_info "用户自定义 用户设置"' >>user_functions.sh
    echo '    print_info "$1"' >>user_functions.sh
    echo '}' >>user_functions.sh
    echo '' >>user_functions.sh

    echo 'function install_soft_pre() {' >>user_functions.sh
    echo '    print_info "用户自定义 安装软件 前调用"' >>user_functions.sh
    echo '    print_info "userName: $1"' >>user_functions.sh
    echo '    print_info "softName: $2"' >>user_functions.sh
    echo '}' >>user_functions.sh
    echo '' >>user_functions.sh


    echo 'function install_soft() {' >>user_functions.sh
    echo '    print_info "用户自定义 安装软件"' >>user_functions.sh
    echo '    print_info "userName: $1"' >>user_functions.sh
    echo '    print_info "softName: $2"' >>user_functions.sh
    echo '}' >>user_functions.sh
    echo '' >>user_functions.sh

    echo 'function uninstall_soft_pre() {' >>user_functions.sh
    echo '    print_info "用户自定义 卸载软件 前调用"' >>user_functions.sh
    echo '    print_info "userName: $1"' >>user_functions.sh
    echo '    print_info "softName: $2"' >>user_functions.sh
    echo '}' >>user_functions.sh
    echo '' >>user_functions.sh


    echo 'function uninstall_soft() {' >>user_functions.sh
    echo '    print_info "用户自定义 卸载软件"' >>user_functions.sh
    echo '    print_info "userName: $1"' >>user_functions.sh
    echo '    print_info "softName: $2"' >>user_functions.sh
    echo '}' >>user_functions.sh
    echo '' >>user_functions.sh

    echo 'function check_os_version() {' >>user_functions.sh
    echo '    print_info "用户自定义 检查系统版本 这是一个示例 请自行修改"' >>user_functions.sh
    echo '    check_ubuntu_version "22.04"' >>user_functions.sh
    echo '    if [[ $? -ne 0 ]]; then' >>user_functions.sh
    echo '        print_error  "当前系统版本不是Ubuntu 22.04 桌面版"' >>user_functions.sh
    echo '        return 1' >>user_functions.sh
    echo '    fi' >>user_functions.sh
    echo '    return 0' >>user_functions.sh
    echo '}' >>user_functions.sh
    echo '' >>user_functions.sh

    print_success "user_functions.sh文件创建成功"
}

function create_install_ini() {
    touch install.ini
    echo "[install]" >install.ini
    echo "userName=$s_userName" >>install.ini
    echo "softName=$s_softName" >>install.ini
    echo "dependList=$s_dependList" >>install.ini
    echo "buildDependList=$s_buildDependList" >>install.ini
    echo "lnName=$s_lnName" >>install.ini
    echo "buildType=$s_buildType" >>install.ini
}

function create_project_ini() {
    print_warning "配置文件不存在，创建配置文件"
    touch project.ini

    local script_src=$(cd "../../../" && pwd)
    local script_build=$(cd "$script_src/../" && pwd)
    local script_root=$(cd "$script_build/../" && pwd)


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
    echo "[project]" >project.ini
    echo "softName=${softName}" >>project.ini
    echo "sourceDir=${sourceDir}" >>project.ini
    echo "buildDir=${buildDir}" >>project.ini
    echo "targetDir=${targetDir}" >>project.ini
    echo "zipPath=${zipPath}" >>project.ini
    echo "buildType=${buildType}" >>project.ini
    echo "userName=${userName}" >>project.ini
    echo "dependList=${dependList}" >>project.ini
    echo "buildDependList=${buildDependList}" >>project.ini
    echo "lnName=${lnName}" >>project.ini

    print_success "配置文件创建成功"
}

function change_Project_Ini() {
    local script_src=$(cd "../../../" && pwd)
    local script_build=$(cd "$script_src/../" && pwd)
    local script_root=$(cd "$script_build/../" && pwd)

    # 设置默认值 全局变量使用s_开头
    local softName=$(basename $script_src)
    local sourceDir="${script_src}"
    local buildDir="${script_build}/build-$softName-unknown-Release"
    local targetDir="${script_root}/amd64"
    local zipPath="${script_root}"
    local buildType=$(read_iniFile_field "project.ini" "buildType") # 构建类型，默认为 Release
    local userName=$(read_iniFile_field "project.ini" "userName")
    local dependList=$(read_iniFile_field "project.ini" "dependList")
    local buildDependList=$(read_iniFile_field "project.ini" "buildDependList")
    local lnName=$(read_iniFile_field "project.ini" "lnName")

     # 将这些信息写入到project.ini文件
    echo "[project]" >project.ini
    echo "softName=${softName}" >>project.ini
    echo "sourceDir=${sourceDir}" >>project.ini
    echo "buildDir=${buildDir}" >>project.ini
    echo "targetDir=${targetDir}" >>project.ini
    echo "zipPath=${zipPath}" >>project.ini
    echo "buildType=${buildType}" >>project.ini
    echo "userName=${userName}" >>project.ini
    echo "dependList=${dependList}" >>project.ini
    echo "buildDependList=${buildDependList}" >>project.ini
    echo "lnName=${lnName}" >>project.ini

    print_success "配置文件修改成功"


}

function build_and_copy_software() {
    create_dir "$s_targetDir"
    if [ $? -ne 0 ]; then
        print_error "创建$s_targetDir 目录失败"
        exit 1
    fi

    create_dir "$s_buildDir"
    if [ $? -ne 0 ]; then
        print_error "创建$s_buildDir 目录失败"
        exit 1
    fi
    cd "$s_buildDir"

    cmake_cmd=$(cmake -DCMAKE_BUILD_TYPE="$s_buildType" "$s_sourceDir")
    if [ $? -ne 0 ]; then
        print_error "CMake 命令执行失败: $cmake_cmd"
        exit 1
    fi

    make_cmd=$(make)
    if [ $? -ne 0 ]; then
        print_error "make 命令执行失败: $make_cmd"
        exit 1
    fi

    copy_files $s_buildDir/bin/* $s_targetDir/$s_buildType
    if [ $? -ne 0 ]; then
        print_error "复制可执行程序失败"
        exit 1
    fi

    copy_files ${s_sourceDir}/resource/install/* $s_targetDir
    if [ $? -ne 0 ]; then
        print_error "复制 install目录文件失败"
        exit 1
    fi

    copy_files $s_currentDir/util.sh $s_targetDir
    if [ $? -ne 0 ]; then
        print_error "复制 util.sh 文件失败"
        exit 1
    fi

    copy_files $s_currentDir/install.sh $s_targetDir
    if [ $? -ne 0 ]; then
        print_error "复制 install.sh 文件失败"
        exit 1
    fi
}

function create_zip_file() {
    cd "$s_sourceDir"
    local current_date=$(date +"%m.%d")
    local current_tag=$(git describe --tags --abbrev=0)
    local current_branch=$(git rev-parse --abbrev-ref HEAD)  

    if [ -z "$current_tag" ]; then
        current_tag=$(date +"%H%M%S")
    fi

    cd "$s_targetDir"
    local zip_file_name="${s_softName}_${current_branch}_${current_tag}_${s_buildType}_${current_date}.zip"
    zip -r "$s_zipFilePath/$zip_file_name" "."
    if [ $? -ne 0 ]; then
        print_error "创建压缩包失败"
        exit 1
    fi

    print_success "压缩包创建完成：$s_zipFilePath/$zip_file_name"
}

function main() {
    build_and_copy_software
    if [ $? -ne 0 ]; then
        print_error "构建失败"
        exit 1
    fi

    create_zip_file
    if [ $? -ne 0 ]; then
        print_error "创建压缩包失败"
        exit 1
    fi

    cd $s_targetDir
    bash install.sh -i
}

# ***************************************** 运行 ********************************************#
isExit=0
s_currentDir=$(pwd)
s_iniDir="$s_currentDir/project_conf"
s_resourceDir=$s_currentDir/../../resource/ # 默认资源路径
create_dir $s_resourceDir
create_dir $s_resourceDir/install
create_dir $s_resourceDir/project_conf
create_dir $s_iniDir

cd $s_iniDir
# 判断用户脚本是否存在
if [ ! -f "./user_functions.sh" ]; then
    if [ ! -f "$s_resourceDir/install/user_functions.sh" ]; then
        create_user_functions_sh
        copy_files ./user_functions.sh $s_resourceDir/install/
        isExit=1
    else
        copy_files $s_resourceDir/install/user_functions.sh ./
    fi
fi

if [ ! -f "$s_resourceDir/install/user_functions.sh" ]; then
    copy_files ./user_functions.sh $s_resourceDir/install/
    isExit=1
fi

# 判断配置文件是否存在
print_warning "检查配置文件，注意 如果想要更改配置，请删除本地配置project.ini 文件后 修改用户保存的默认配置project.ini 文件，以用户本地保存的配置文件为主"
print_warning "如果用户保存的默认配置文件不存在，将创建默认配置文件"
print_warning "如果用户保存的默认配置文件存在，将使用用户保存的默认配置文件"
print_warning "通常，用户保存的默认配置文件路径 $s_resourceDir/project_conf/project.ini"
if [ ! -f "./project.ini" ]; then
    print_warning "默认配置文件不存在"
    if [ ! -f "$s_resourceDir/project_conf/project.ini" ]; then
        print_warning "用户保存的默认配置文件也不存在，将创建默认配置"
        create_project_ini
        isExit=1
    else
        print_warning "用户保存的默认配置文件存在，将使用用户保存的默认配置"
        copy_files $s_resourceDir/project_conf/project.ini ./
        change_Project_Ini
    fi
fi


# 设置默认值 全局变量使用s_开头
s_softName=$(read_iniFile_field "project.ini" "softName")   # 软件名称      appName
s_sourceDir=$(read_iniFile_field "project.ini" "sourceDir") # 源码目录      /home/test/sourceDir
s_buildDir=$(read_iniFile_field "project.ini" "buildDir")   # 构建目录      /home/test/buildDir
s_targetDir=$(read_iniFile_field "project.ini" "targetDir") # 目标目录      /home/test/amd64
s_zipFilePath=$(read_iniFile_field "project.ini" "zipPath") # zip文件目录   /home/test/
s_buildType=$(read_iniFile_field "project.ini" "buildType") # 构建类型，默认为 Release
s_userName=$(read_iniFile_field "project.ini" "userName")
s_dependList=$(read_iniFile_field "project.ini" "dependList")
s_buildDependList=$(read_iniFile_field "project.ini" "buildDependList")
s_lnName=$(read_iniFile_field "project.ini" "lnName")

create_install_ini
copy_files ./install.ini $s_resourceDir/install/

cd $s_currentDir

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
        print_success "清理完成"
        exit 0
        ;;
    -C | --cleanAll)
        cleanTarget
        cleanBuildDir
        print_success "清理完成"
        exit 0
        ;;
    -s | --settings)
        xdg-open ./project.ini
        exit 0
        ;;
    *)
        print_error "Unknown option: $1"
        help
        exit 1
        ;;
    esac
done

if [[ $isExit -eq 1 ]]; then
    print_warning "请检查配置文件后，再次运行"
    exit 1
fi

printValue

main "$@"
