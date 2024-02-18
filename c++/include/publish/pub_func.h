#ifndef PUB_FUNC_H
#define PUB_FUNC_H

#include <QByteArray>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QWidget>


//json相关
bool byteArray2JsonOBj(const QByteArray &reply_data, QJsonObject &obj);                 //byteArray转json对象
bool hasKey(const QJsonObject &obj, QString key, QString &value);                       //根据key获取value
bool hasKey(const QJsonObject &obj, QString key, bool &value);                          //根据key获取value
bool hasKey(const QJsonObject &obj, QString key, int &value);                           //根据key获取value

//文件相关
QString readFileContent(const QString &filePath);                                       //读取文件
QDateTime getCreationDate(const QString &path);                                         //获取文件创建日期
bool createSymbolicLink(const QString &source, const QString &linkPath);                //创建软链接

//图片操作
double compareImages(const QImage &image1, const QImage &image2);                       //图片对比
bool scalImageSize(QString filePath);                                                   //图片缩放

//调用程序
int runCommandWithTimeout(const QString &command,                                       //启动子程序
                          const QStringList &args,
                          QString &output,
                          int timeout = 30000);

void runCommandDetached(const QString &command,const QStringList& args);                //启动程序 分离


//qt控件
void enableWidgetWithDelay(QWidget *widget, int msec = 1000);                           //禁用控件
void eventPause(int msec);                                                              //暂停


//其他
QString generateUuid(QStringList list);                                                 //获取字符串hash
bool compareVersionStrings(const QString& version1, const QString& version2);           //对比版本号

#endif // PUB_FUNC_H
