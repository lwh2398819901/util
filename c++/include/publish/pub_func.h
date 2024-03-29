#ifndef PUB_FUNC_H
#define PUB_FUNC_H

#include <QByteArray>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QWidget>

/**
 * @brief 将字节数组转换为JSON对象
 * @param reply_data 输入的字节数组
 * @param obj[out] 输出的JSON对象
 * @return 如果成功则返回true，否则返回false
 */
bool byteArray2JsonOBj(const QByteArray &reply_data, QJsonObject &obj);

/**
 * @brief 检查JSON对象是否包含给定的键，并在存在时检索其值
 * @param obj 输入的JSON对象
 * @param key 要搜索的键
 * @param value[out] 与键关联的值
 * @return 如果找到键则返回true，否则返回false
 */
bool hasKey(const QJsonObject &obj, QString key, QString &value);
/**
 * @brief 检查JSON对象是否包含给定的键，并在存在时检索其值
 * @param obj 输入的JSON对象
 * @param key 要搜索的键
 * @param value[out] 与键关联的值
 * @return 如果找到键则返回true，否则返回false
 */
bool hasKey(const QJsonObject &obj, QString key, bool &value);
/**
 * @brief 检查JSON对象是否包含给定的键，并在存在时检索其值
 * @param obj 输入的JSON对象
 * @param key 要搜索的键
 * @param value[out] 与键关联的值
 * @return 如果找到键则返回true，否则返回false
 */
bool hasKey(const QJsonObject &obj, QString key, int &value);

/**
 * @brief 读取指定文件的内容
 * @param filePath 文件路径
 * @return 文件内容字符串  注意 这个函数没有考虑文件大小的问题，所以最好不要用来读取大文件
 */
QString readFileContent(const QString &filePath);

/**
 * @brief 创建文件
 * @param filePath 文件路径
 * @param content[out] 文件内容
 * @return 如果成功则返回true，否则返回false
*/
bool createFile(const QString &filePath, const QString &content="");

/**
 * @brief 追加文件内容，没有文件则自动创建
 * @param filePath 文件路径
 * @param content 文件内容
 * @return 如果成功则返回true，否则返回false
 */
bool appendFile(const QString &filePath, const QString &content="");


/**
 * @brief 获取文件的创建日期
 * @param path 文件路径
 * @return 文件的创建日期
 */
QDateTime getCreationDate(const QString &path);

/**
 * @brief 创建符号链接
 * @param source 链接的源
 * @param linkPath 要创建的链接路径
 * @return 如果成功则返回true，否则返回false
 *
 * 示例：
 *    将/home/test/123文件 创建软链接到 /tmp/下
 *    createSymbolicLink("/home/test/123","/tmp/123")
 *
 */
bool createSymbolicLink(const QString &source, const QString &linkPath);

/**
 * @brief 比较两张图片
 * @param image1 第一张图片
 * @param image2 第二张图片
 * @return 图片之间的相似度
 */
double compareImages(const QImage &image1, const QImage &image2);

/**
 * @brief 调整图片大小
 * @param filePath 图片文件路径
 * @return 如果成功则返回true，否则返回false
 */
bool scalImageSize(QString filePath);

/**
 * @brief 启动子程序并设置超时时间
 * @param command 要执行的命令
 * @param args 命令参数
 * @param output[out] 子程序输出
 * @param timeout 超时时间（默认为30000毫秒）
 * @return 子程序的退出代码
 */
int runCommandWithTimeout(const QString &command, const QStringList &args, QString &output, int timeout = 30000);

/**
 * @brief 启动子程序并分离
 * @param command 要执行的命令
 * @param args 命令参数
 */
void runCommandDetached(const QString &command, const QStringList& args);

/**
 * @brief 延迟禁用控件
 * @param widget 要禁用的控件
 * @param msec 延迟时间（默认为1000毫秒）
 */
void enableWidgetWithDelay(QWidget *widget, int msec = 1000);

/**
 * @brief 暂停程序执行的指定时间
 * @param msec 暂停时间（毫秒）
 */
void eventPause(int msec);

/**
 * @brief 生成字符列表的哈希值
 * @param list 字符串列表
 * @return md5数值
 */
QString generateUuid(QStringList list);

/**
 * @brief 生成字符串的md5
 * @param list 字符串列表
 * @return md5数值
 */
QString calculateMD5(const QString &text);


/**
 * @brief 比较版本号字符串
 * @param version1 版本号1
 * @param version2 版本号2
 * @return 如果version1大于version2则返回true，否则返回false
 */
bool compareVersionStrings(const QString& version1, const QString& version2);


/**
 * @brief 检查网络接口是否正常
 * @param url 网络接口
 * @param msec 超时时间 默认是3000 单位毫秒
 * @return 接口正常则返回true，否则返回false
 */
bool checkNetworkConnection(const QString &url, uint msec = 30000);


/**
 * @brief 消息窗口 自动隐藏
 * @param title 标题
 * @param text 消息内容
 * @param msec 显示时间 默认是3000 单位毫秒
 */
void showAutoCloseMessageBox(const QString& title, const QString& text, int msec = 5000);


#endif // PUB_FUNC_H
