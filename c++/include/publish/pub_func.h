#ifndef PUB_FUNC_H
#define PUB_FUNC_H

#include "version.h"
#include <QByteArray>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QProcess>
#include <QTextStream>
#include <QTimer>
#include <QVector>
#include <QtGlobal>
#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>


#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#include <QStringDecoder>
#include <QStringEncoder>
#else
#include <QTextCodec>
# endif


#if ENABLE_QT_WIDGETS
#include <QWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QImage>
#include <QPixmap>

#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QApplication>
#include <QFileDialog>
#include <QClipboard>

#endif


// ====================== 非GUI函数 ======================

// ------------- 字符串处理和编码转换 -------------
/**
 * UTF-8 转 GBK 编码转换
 * @param utf8String UTF-8编码的字符串
 * @return GBK编码的字节数组
 * @example 
 *   QString utf8Str = "测试文本";
 *   QByteArray gbkData = utf8ToGbk(utf8Str);
 */
QByteArray utf8ToGbk(const QString &utf8String);

/**
 * GBK 转 UTF-8 编码转换
 * @param gbkByteArray GBK编码的字节数组
 * @return UTF-8编码的字符串
 * @example
 *   QByteArray gbkData = ...;
 *   QString utf8Str = gbkToUtf8(gbkData);
 */
QString gbkToUtf8(const QByteArray &gbkByteArray);

/**
 * 计算字符串的MD5哈希值
 * @param text 需要计算哈希值的字符串
 * @return MD5哈希值字符串
 * @example
 *   QString text = "需要加密的文本";
 *   QString md5 = calculateMD5(text);
 */
QString calculateMD5(const QString &text);

/**
 * 比较两个版本号字符串
 * @param version1 第一个版本号
 * @param version2 第二个版本号
 * @return 如果version1大于version2返回true
 * @example
 *   bool isNewer = compareVersionStrings("2.0.1", "1.9.9"); // 返回true
 */
bool compareVersionStrings(const QString& version1, const QString& version2);

/**
 * 根据字符串列表生成UUID
 * @param list 用于生成UUID的字符串列表
 * @return 生成的UUID字符串
 * @example
 *   QStringList list = {"数据1", "数据2"};
 *   QString uuid = generateUuid(list);
 */
QString generateUuid(QStringList list);

// ------------- JSON处理 -------------
/**
 * 将字节数组转换为JSON对象
 * @param reply_data 包含JSON数据的字节数组
 * @param obj 输出的JSON对象
 * @return 转换成功返回true
 * @example
 *   QByteArray data = "{\"name\":\"测试\"}";
 *   QJsonObject obj;
 *   if(byteArray2JsonOBj(data, obj)) {
 *       qDebug() << obj["name"].toString();
 *   }
 */
bool byteArray2JsonOBj(const QByteArray &reply_data, QJsonObject &obj);

/**
 * 检查JSON对象中是否存在指定键并获取其值
 * @param obj JSON对象
 * @param key 要查找的键
 * @param value 输出值
 * @return 找到键则返回true
 * @example
 *   QJsonObject obj;
 *   QString value;
 *   if(hasKey(obj, "name", value)) {
 *       qDebug() << "找到值:" << value;
 *   }
 */
bool hasKey(const QJsonObject &obj, QString key, QString &value);
bool hasKey(const QJsonObject &obj, QString key, bool &value);
bool hasKey(const QJsonObject &obj, QString key, int &value);
bool hasKey(const QJsonObject &obj, QString key, double &value);

// ------------- 文件操作 -------------
/**
 * 读取文件内容
 * @param filePath 文件路径
 * @return 文件内容字符串
 * @example
 *   QString content = readFileContent("/path/to/file.txt");
 */
QString readFileContent(const QString &filePath);

/**
 * 创建新文件
 * @param filePath 文件路径
 * @param content 文件初始内容
 * @return 创建成功返回true
 * @example
 *   bool success = createFile("/path/to/file.txt", "初始内容");
 */
bool createFile(const QString &filePath, const QString &content = "");

/**
 * 追加内容到文件
 * @param filePath 文件路径
 * @param content 要追加的内容
 * @return 追加成功返回true
 * @example
 *   bool success = appendFile("/path/to/file.txt", "追加的内容");
 */
bool appendFile(const QString &filePath, const QString &content = "");

/**
 * 复制文件
 * @param sourcePath 源文件路径
 * @param targetPath 目标文件路径
 * @return 复制成功返回true
 * @example
 *   bool success = copyFile("/source/file.txt", "/dest/file.txt");
 */
bool copyFile(const QString &sourcePath, const QString &targetPath);

/**
 * 复制目录
 * @param sourcePath 源目录路径
 * @param targetPath 目标目录路径
 * @return 复制成功返回true
 * @example
 *   bool success = copyDirectory("/source/dir", "/dest/dir");
 */
bool copyDirectory(const QString &sourcePath, const QString &targetPath);

/**
 * 获取文件创建时间
 * @param path 文件路径
 * @return 文件创建时间
 * @example
 *   QDateTime createTime = getCreationDate("/path/to/file.txt");
 */
QDateTime getCreationDate(const QString &path);

/**
 * 创建符号链接
 * @param source 源文件路径
 * @param linkPath 链接文件路径
 * @return 创建成功返回true
 * @example
 *   bool success = createSymbolicLink("/real/path", "/link/path");
 */
bool createSymbolicLink(const QString &source, const QString &linkPath);

// ------------- 进程和系统 -------------
/**
 * 执行命令并等待结果（带超时）
 * @param command 要执行的命令
 * @param args 命令参数列表
 * @param output 命令输出结果
 * @param timeout 超时时间（毫秒）
 * @return 命令执行的返回码
 * @example
 *   QString output;
 *   int exitCode = runCommandWithTimeout("ls", {"-l"}, output, 5000);
 */
int runCommandWithTimeout(const QString &command, const QStringList &args, QString &output, int timeout = 30000);

/**
 * 执行命令（不等待结果）
 * @param command 要执行的命令
 * @param args 命令参数列表
 * @example
 *   runCommandDetached("notepad", {"file.txt"});
 */
void runCommandDetached(const QString &command, const QStringList& args);

/**
 * 暂停事件循环指定时间
 * @param msec 暂停时间（毫秒）
 * @example
 *   eventPause(1000); // 暂停1秒
 */
void eventPause(int msec);

// ------------- 网络相关 -------------
/**
 * 检查网络连接状态
 * @param url 要检查的URL
 * @param msec 超时时间（毫秒）
 * @return 连接正常返回true
 * @example
 *   bool isConnected = checkNetworkConnection("https://www.example.com");
 */
bool checkNetworkConnection(const QString &url, uint msec = 3000);

/**
 * 检查是否为本地IP地址
 * @param ip IP地址字符串
 * @return 是本地IP返回true
 * @example
 *   bool isLocal = isLocalIP("127.0.0.1"); // 返回true
 */
bool isLocalIP(const QString &ip);

// ====================== GUI函数 ======================
#if ENABLE_QT_WIDGETS

// ------------- 窗口部件操作 -------------
/**
 * 延迟启用窗口部件
 * @param widget 要启用的窗口部件
 * @param msec 延迟时间（毫秒）
 * @example
 *   enableWidgetWithDelay(myButton, 2000);
 */
void enableWidgetWithDelay(QWidget *widget, int msec = 1000);

/**
 * 为表格创建复选框组
 * @param table 目标表格
 * @param uncontrolableColumns 不可控制的列集合
 * @param groupTitle 组标题
 * @return 创建的复选框组
 * @example
 *   QSet<int> uncontrolCols = {1, 2};
 *   QGroupBox *group = createCheckBoxGroupForTable(table, uncontrolCols, "选项");
 */
QGroupBox *createCheckBoxGroupForTable(QTableWidget *table,
                                     const QSet<int> &uncontrolableColumns,
                                     QString groupTitle);

/**
 * 搜索表格内容
 * @param tableWidget 目标表格
 * @param findStr 搜索文本
 * @example
 *   searchTableItem(tableWidget, "搜索内容");
 */
void searchTableItem(QTableWidget *tableWidget, const QString &findStr);

// ------------- 图像处理 -------------
/**
 * 比较两个图像的相似度
 * @param image1 第一个图像
 * @param image2 第二个图像
 * @return 相似度值（0-1之间）
 * @example
 *   double similarity = compareImages(image1, image2);
 */
double compareImages(const QImage &image1, const QImage &image2);

/**
 * 调整图像大小
 * @param filePath 图像文件路径
 * @return 调整成功返回true
 * @example
 *   bool success = scalImageSize("/path/to/image.jpg");
 */
bool scalImageSize(QString filePath);

/**
 * Base64字符串转图像
 * @param arr Base64编码的字符串
 * @return 转换后的图像
 * @example
 *   QPixmap pixmap = base64ToPixmap(base64String);
 */
QPixmap base64ToPixmap(const QString &arr);

/**
 * 图像转Base64字符串
 * @param map 要转换的图像
 * @return Base64编码的字符串
 * @example
 *   QString base64 = pixmapToBase64(pixmap);
 */
QString pixmapToBase64(const QPixmap &map);

// ------------- 消息框 -------------
/**
 * 显示自动关闭的消息框
 * @param title 消息框标题
 * @param text 消息内容
 * @param msec 显示时间（毫秒）
 * @example
 *   showAutoCloseMessageBox("提示", "操作完成", 3000);
 */
void showAutoCloseMessageBox(const QString& title, const QString& text, int msec = 5000);

/**
 * 显示错误消息框
 * @param message 错误信息
 * @example
 *   showErrorMsgBox("操作失败");
 */
void showErrorMsgBox(const QString &message,bool isCopy = false);

/**
 * 显示提示消息框
 * @param message 提示信息
 * @example
 *   showInfoMsgBox("操作成功");
 */
void showInfoMsgBox(const QString &message,bool isCopy = false);

/**
 * 显示询问消息框
 * @param message 询问信息
 * @return 用户选择"是"返回true
 * @example
 *   if(askQuestionMsgBox("是否继续？")) {
 *       // 用户选择了"是"
 *   }
 */
bool askQuestionMsgBox(const QString &message);

#endif // ENABLE_QT_WIDGETS

#endif // PUB_FUNC_H



