#ifndef PUB_FUNC_H
#define PUB_FUNC_H

#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QImage>
#include <QJsonObject>
#include <QNetworkRequest>


bool byteArray2JsonOBj(const QByteArray &reply_data, QJsonObject &obj);                 //byteArray转json对象
bool hasKey(const QJsonObject &obj, QString key, QString &value);                       //根据key获取value
bool hasKey(const QJsonObject &obj, QString key, bool &value);                          //根据key获取value
bool hasKey(const QJsonObject &obj, QString key, int &value);                           //根据key获取value

QDateTime getCreationDate(const QString &path);
QString generateUuid(QStringList list);

int runCommandWithTimeout(const QString &command, const QStringList &args, QString &output, int timeout = 30000);
void runCommandDetached(const QString &command,const QStringList& args);

double compareImages(const QImage &image1, const QImage &image2);
bool scalImageSize(QString filePath);
void enableWidgetWithDelay(QWidget *widget, int msec = 1000);
void eventPause(int msec);

QString readFileContent(const QString &filePath);

bool compareVersionStrings(const QString& version1, const QString& version2);



#endif // PUB_FUNC_H
