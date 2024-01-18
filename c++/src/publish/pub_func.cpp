#include "pub_func.h"
#include "pub_macro.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QJsonParseError>
#include <QJsonValue>
#include <QProcess>
#include <QUrlQuery>
#include <QTimer>
#include <QWidget>
#include <QEventLoop>




bool byteArray2JsonOBj(const QByteArray &reply_data, QJsonObject &obj)
{
    QJsonParseError json_error;
    QJsonDocument doc = QJsonDocument::fromJson(reply_data, &json_error);
    if (json_error.error != QJsonParseError::NoError) {
        PrintLog("json error:" + json_error.errorString());
        return false;
    }
    if (doc.isObject()) {
        obj = doc.object();
    }
    return doc.isObject();
}

bool hasKey(const QJsonObject &obj, QString key, QString &value)
{
    if (!obj.contains(key)) {
        return false;
    }
    QJsonValue v = obj.value(key);
    value = v.toString();
    return true;

}

bool hasKey(const QJsonObject &obj, QString key, bool &value)
{
    if (!obj.contains(key)) {
        return false;
    }
    QJsonValue v = obj.value(key);
    value = v.toBool();
    return true;
}

bool hasKey(const QJsonObject &obj, QString key, int &value)
{
    if (!obj.contains(key)) {
        return false;
    }
    QJsonValue v = obj.value(key);
    value = v.toInt();
    return true;
}


QDateTime getCreationDate(const QString &path)
{
    QFileInfo fileInfo(path);
    if (fileInfo.exists()) {
        return fileInfo.birthTime();
    }
    return QDateTime();
}

QString generateUuid(QStringList list)
{
    QString uniqueString;
    foreach (QString str, list) {
        uniqueString += str;
    }
    QByteArray hash = QCryptographicHash::hash(uniqueString.toUtf8(), QCryptographicHash::Md5);
    return hash.toHex();
}


void runCommand(const QString &command)
{
    QProcess::startDetached(command);
}

QString runCommandWithTimeout(const QString &command, int timeout)
{
    QProcess process;
    process.start(command);
    if (!process.waitForStarted(timeout)) {
        return "";
    }

    if (!process.waitForFinished(timeout)) {
        process.kill();
        return "";
    }

    return process.readAllStandardOutput();
}

int runCommandWithTimeout(const QString &command, QString &output, int timeout)
{

    // 获取当前环境变量
    QProcessEnvironment originalEnv = QProcessEnvironment::systemEnvironment();

    // 创建一个新的环境变量对象
    QProcessEnvironment tempEnv = originalEnv;
    tempEnv.insert("LANG", "en_US.UTF-8");
    tempEnv.insert("LC_ALL", "en_US.UTF-8");

    // 设置新的环境变量
    QProcess process;
    process.setProcessEnvironment(tempEnv);
    process.start(command);
    if (!process.waitForStarted(timeout)) {
        return -1;
    }

    if (!process.waitForFinished(timeout)) {
        process.kill();
        return -1;
    }

    output = QString::fromLocal8Bit(process.readAllStandardOutput());

    // 还原原始环境变量
    process.setProcessEnvironment(originalEnv);

    return process.exitCode();
}


double compareImages(const QImage &image1, const QImage &image2)
{
    // 获取图片的像素数据
    const uchar *data1 = image1.constBits();
    const uchar *data2 = image2.constBits();

    int width = image1.width();
    int height = image1.height();

    int totalPixels = width * height;
    int similarPixels = 0;

    // 比较每个像素的灰度值
    for (int i = 0; i < totalPixels; i++) {
        if (data1[i] == data2[i]) {
            similarPixels++;
        }
    }
    // 计算相似度
    double similarity = (double)similarPixels / totalPixels;
    return similarity;
}

bool scalImageSize(QString filePath)
{
    //修改文件长宽比
    QImage originalImage(filePath);    // 加载原始图片
    if (originalImage.isNull()) {
        return false;
    }

    if (originalImage.sizeInBytes() > 1 * 1024 * 1024) { // 1MB
        int targetWidth = 1280;
        int targetHeight = originalImage.height() * targetWidth / originalImage.width();
        QImage scaledImage = originalImage.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio);
        scaledImage.save(filePath);
    }

    return true;
}

void enableWidgetWithDelay(QWidget *widget, int msec)
{
    widget->setEnabled(false);
    QTimer::singleShot(msec, nullptr, [ = ]() {
        widget->setEnabled(true);
    });
}

void eventPause(int msec)
{
    // 创建一个事件循环
    QEventLoop eventLoop;

    // 创建一个定时器
    QTimer timer;
    timer.setSingleShot(true); // 让定时器只触发一次

    // 连接定时器的超时信号到事件循环的退出槽
    QObject::connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);

    // 启动定时器，2秒后触发超时信号
    timer.start(msec); // 2000毫秒 = 2秒

    // 进入事件循环，等待2秒
    eventLoop.exec();
}


// 函数：读取文件内容
QString readFileContent(const QString &filePath)
{
    // 创建一个QFile对象
    QFile file(filePath);
    // 检查文件是否打开成功
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        PrintErrLog("无法打开文件：" << filePath);
        return QString();
    }

    // 创建一个QTextStream对象来处理文件流
    QTextStream in(&file);
    // 读取文件所有内容
    QString content = in.readAll();

    // 关闭文件
    file.close();
    // 返回读取到的内容
    return content;
}

