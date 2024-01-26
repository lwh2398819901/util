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
#include <QVector>

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

void runCommandDetached(const QString &command,const QStringList& args)
{
    QProcess::startDetached(command,args);
}

int runCommandWithTimeout(const QString &command,const QStringList& args, QString &output, int timeout)
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
    process.start(command,args);
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
    QEventLoop eventLoop;
    QTimer::singleShot(msec,&eventLoop,&QEventLoop::quit);
    eventLoop.exec();
}

// 函数：读取文件内容
QString readFileContent(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        PrintErrLog("无法打开文件：" << filePath);
        return QString();
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    return content;
}

// 自定义函数比较版本号字符串
bool compareVersionStrings(const QString& version1, const QString& version2) {
    QVector<int> parts1;
    for (const auto& part : version1.split(".")) {
        parts1.push_back(part.toInt());
    }

    QVector<int> parts2;
    for (const auto& part : version2.split(".")) {
        parts2.push_back(part.toInt());
    }

    for (int i = 0; i < std::min(parts1.size(), parts2.size()); ++i) {
        int v1 = parts1[i];
        int v2 = parts2[i];

        if (v1 != v2) {
            return v1 > v2;
        }
    }
    return parts1.size() > parts2.size();
}
