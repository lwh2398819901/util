#include "pub_func.h"
#include "pub_macro.h"
#include <qapplication.h>



#if defined(Q_OS_WIN)

#elif defined(Q_OS_LINUX)
#include <unistd.h>
#include <errno.h>
#elif defined(Q_OS_MAC)

#elif defined(Q_OS_ANDROID)

#elif defined(Q_OS_IOS)

#elif defined(Q_OS_UNIX)

#endif

// ====================== 非GUI函数 ======================

// ------------- 字符串处理和编码转换 -------------
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
QByteArray utf8ToGbk(const QString &utf8String)
{
    QByteArray gbkByteArray;
    QStringEncoder encoder("GBK");
    gbkByteArray = encoder.encode(utf8String);
    return gbkByteArray;
}

QString gbkToUtf8(const QByteArray &gbkByteArray)
{
    QString utf8String;
    QStringDecoder decoder("GBK");
    utf8String = decoder.decode(gbkByteArray);
    return utf8String;
}
#else
QByteArray utf8ToGbk(const QString &utf8String)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    return codec->fromUnicode(utf8String);
}

QString gbkToUtf8(const QByteArray &gbkByteArray)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    return codec->toUnicode(gbkByteArray);
}
#endif

QString calculateMD5(const QString &text)
{
    QByteArray hashBytes = QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Md5);
    return hashBytes.toHex();
}

bool compareVersionStrings(const QString& version1, const QString& version2)
{
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

QString generateUuid(QStringList list)
{
    QString uniqueString;
    foreach (QString str, list) {
        uniqueString += str;
    }
    return calculateMD5(uniqueString);
}

// ------------- JSON处理 -------------
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

bool hasKey(const QJsonObject &obj, QString key, double &value)
{
    if (!obj.contains(key)) {
        return false;
    }
    QJsonValue v = obj.value(key);
    value = v.toDouble();
    return true;
}

// ------------- 文件操作 -------------
QString readFileContent(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        PrintErrLog("Unable to open file: " << filePath);
        return QString();
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    return content;
}

bool createFile(const QString &filePath, const QString &content)
{
    QFile file(filePath);
    if (file.exists()) {
        return true;
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        PrintErrLog("Unable to create file: " << filePath);
        return false;
    }
    QTextStream out(&file);
    out << content;
    file.close();
    return true;
}

bool appendFile(const QString &filePath, const QString &content)
{
    QFile file(filePath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        PrintErrLog("Unable to open file: " << filePath);
        return false;
    }
    QTextStream out(&file);
    out << content;
    file.close();
    return true;
}

bool copyFile(const QString &sourcePath, const QString &targetPath)
{
    QFile sourceFile(sourcePath);
    QFile targetFile(targetPath);

    if (!sourceFile.exists()) {
        qDebug() << "The source file does not exist:" << sourcePath;
        return false;
    }

    if (!sourceFile.copy(targetPath)) {
        qDebug() << "Failed to copy the file: " << sourcePath << " to " << targetPath;
        return false;
    }

    QFileInfo sourceInfo(sourcePath);
    QFile::Permissions permissions = sourceInfo.permissions();
    QFile::setPermissions(targetPath, permissions);
    return true;
}

bool copyDirectory(const QString &sourcePath, const QString &targetPath)
{
    QDir sourceDir(sourcePath);
    QDir targetDir(targetPath);

    if (!targetDir.exists()) {
        if (!targetDir.mkpath(targetDir.path())) {
            qDebug() << "Unable to create the target directory:" << targetPath;
            return false;
        }
    }

    foreach (QFileInfo entry, sourceDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString targetEntryPath = targetDir.filePath(entry.fileName());
        if (entry.isDir()) {
            if (!copyDirectory(entry.filePath(), targetEntryPath)) {
                return false;
            }
        } else {
            if (!QFile::copy(entry.filePath(), targetEntryPath)) {
                qDebug() << "Failed to copy the file: " << entry.filePath() << " to " << targetEntryPath;
                return false;
            }
        }
    }
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

bool createSymbolicLink(const QString &source, const QString &linkPath)
{
    const char *src = source.toUtf8().constData();
    const char *lnk = linkPath.toUtf8().constData();
#if defined(Q_OS_LINUX)
    int result = symlink(src, lnk);
    if (result == -1) {
        perror("Error creating symbolic link:");
        LOGGER_ERR(QString("Error creating symbolic link: ") + strerror(errno));
        return false;
    }
    return true;
#endif
    return false;
}

// ------------- 进程和系统 -------------
int runCommandWithTimeout(const QString &command, const QStringList& args, QString &output, int timeout)
{
    QProcessEnvironment originalEnv = QProcessEnvironment::systemEnvironment();

    QProcessEnvironment tempEnv = originalEnv;
    tempEnv.insert("LANG", "en_US.UTF-8");
    tempEnv.insert("LC_ALL", "en_US.UTF-8");

    QProcess process;
    process.setProcessEnvironment(tempEnv);
    process.start(command, args);
    if (!process.waitForStarted(timeout)) {
        return -1;
    }

    if (!process.waitForFinished(timeout)) {
        process.kill();
        return -1;
    }

    output = QString::fromLocal8Bit(process.readAllStandardOutput());

    process.setProcessEnvironment(originalEnv);
    qDebug() << process.errorString();
    return process.exitCode();
}

void runCommandDetached(const QString &command, const QStringList& args)
{
    QProcess::startDetached(command, args);
}

void eventPause(int msec)
{
    QEventLoop eventLoop;
    QTimer::singleShot(msec, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
}

// ------------- 网络相关 -------------
bool checkNetworkConnection(const QString &url, uint msec)
{
    static QNetworkCookieJar jar;
    static QNetworkAccessManager manager;
    manager.setCookieJar(&jar);
    manager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    QUrl urlObj(url);
    if (!urlObj.isValid()) {
        return false;
    }

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    request.setUrl(url);

    QNetworkReply* reply = manager.get(request);
    QTimer timer;
    QEventLoop loop;

    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    timer.start(msec);
    loop.exec();

    if (timer.isActive()) {
        timer.stop();
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << ("Network connection error:" + reply->errorString());
            reply->deleteLater();
            return false;
        }
        reply->deleteLater();
        return true;
    }

    reply->abort();
    reply->deleteLater();
    return false;
}

bool isLocalIP(const QString &ip)
{
    QHostAddress address(ip);
    return address.isLoopback() || address.isLinkLocal() || address.isMulticast();
}

// ====================== GUI函数 ======================
#if ENABLE_QT_WIDGETS

// ------------- 窗口部件操作 -------------
void enableWidgetWithDelay(QWidget *widget, int msec)
{
    if (!widget) {
        return;
    }

    widget->setEnabled(false);
    QTimer::singleShot(msec, nullptr, [=]() {
        widget->setEnabled(true);
    });
}

QGroupBox *createCheckBoxGroupForTable(QTableWidget *table,
                                     const QSet<int> &uncontrolableColumns,
                                     QString groupTitle)
{
    QGroupBox *groupBox = new QGroupBox(groupTitle);
    QGridLayout *layout = new QGridLayout;
    int columnCount = table->columnCount();
    int row = 0;
    int col = 0;
    const int COLS_PER_ROW = 3;

    for (int i = 0; i < columnCount; ++i) {
        if (uncontrolableColumns.contains(i)) {
            continue;
        }

        QString headerText = table->horizontalHeaderItem(i)->text();
        QCheckBox *checkBox = new QCheckBox(headerText);
        checkBox->setChecked(true);
        layout->addWidget(checkBox, row, col);

        QObject::connect(checkBox, &QCheckBox::toggled,
        [table, i](bool checked) {
            table->setColumnHidden(i, !checked);
        });

        ++col;
        if (col >= COLS_PER_ROW) {
            col = 0;
            ++row;
        }
    }

    groupBox->setLayout(layout);
    return groupBox;
}

void searchTableItem(QTableWidget *tableWidget, const QString &findStr)
{
    if (findStr.isEmpty()) {
        for (int i = 0; i < tableWidget->rowCount(); i++) {
            tableWidget->setRowHidden(i, false);
        }
        return;
    }

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        bool found = false;
        for (int j = 0; j < tableWidget->columnCount(); j++) {
            QTableWidgetItem *item = tableWidget->item(i, j);
            if (item && item->text().contains(findStr, Qt::CaseInsensitive)) {
                found = true;
                break;
            }
        }
        tableWidget->setRowHidden(i, !found);
    }
}

// ------------- 图像处理 -------------
double compareImages(const QImage &image1, const QImage &image2)
{
    const uchar *data1 = image1.constBits();
    const uchar *data2 = image2.constBits();

    int width = image1.width();
    int height = image1.height();

    int totalPixels = width * height;
    int similarPixels = 0;

    for (int i = 0; i < totalPixels; i++) {
        if (data1[i] == data2[i]) {
            similarPixels++;
        }
    }

    double similarity = (double)similarPixels / totalPixels;
    return similarity;
}

bool scalImageSize(QString filePath)
{
    QImage originalImage(filePath);
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

QPixmap base64ToPixmap(const QString &arr)
{
    QPixmap pixmap;
    pixmap.loadFromData(QByteArray::fromBase64(arr.toLocal8Bit()));
    return pixmap;
}

QString pixmapToBase64(const QPixmap &map)
{
    QImage image = map.toImage();
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    image.save(&buffer, "PNG");
    return QString::fromLatin1(byteArray.toBase64());
}

// ------------- 消息框 -------------
void showAutoCloseMessageBox(const QString &title, const QString &text, int msec)
{
    QMessageBox* msgBox = new QMessageBox(QMessageBox::Information, title, text, QMessageBox::NoButton, 0, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    msgBox->show();
    eventPause(msec);
    delete msgBox;
}

void showErrorMsgBox(const QString &message,bool isCopy)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("错误");
    msgBox.setText(message);
    if(isCopy){
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Save | QMessageBox::Apply);
    }else {
        msgBox.setStandardButtons(QMessageBox::Ok);
    }

    msgBox.setButtonText(QMessageBox::Ok, "确定");
    msgBox.setButtonText(QMessageBox::Save, "保存文本");
    msgBox.setButtonText(QMessageBox::Apply, "复制文本");
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
        QString filePath = QFileDialog::getSaveFileName(nullptr, "保存错误信息", "", "Text Files (*.txt)");
        if (!filePath.isEmpty()) {
            createFile(filePath,message);
        }
    } else if (ret == QMessageBox::Apply) {
        // 放入剪切板
        QApplication::clipboard()->setText(message);
    }
}

void showInfoMsgBox(const QString &message,bool isCopy)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("提示");
    msgBox.setText(message); // 设置消息文本
    if(isCopy){
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Save | QMessageBox::Apply);
    }else {
        msgBox.setStandardButtons(QMessageBox::Ok);
    }
    msgBox.setButtonText(QMessageBox::Ok, "确定");
    msgBox.setButtonText(QMessageBox::Save, "保存文本");
    msgBox.setButtonText(QMessageBox::Apply, "复制文本");
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
        QString filePath = QFileDialog::getSaveFileName(nullptr, "保存提示信息", "", "Text Files (*.txt)");
        if (!filePath.isEmpty()) {
            createFile(filePath,message);
        }
    }else if (ret == QMessageBox::Apply) {
        QApplication::clipboard()->setText(message);
    }
}

bool askQuestionMsgBox(const QString &message)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle("询问");
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "是");
    msgBox.setButtonText(QMessageBox::No, "否");
    return msgBox.exec() == QMessageBox::Yes;
}

#endif // ENABLE_QT_WIDGETS
#include "pub_func.h"
#include "pub_macro.h"
#include <qapplication.h>



#if defined(Q_OS_WIN)

#elif defined(Q_OS_LINUX)
#include <unistd.h>
#include <errno.h>
#elif defined(Q_OS_MAC)

#elif defined(Q_OS_ANDROID)

#elif defined(Q_OS_IOS)

#elif defined(Q_OS_UNIX)

#endif

// ====================== 非GUI函数 ======================

// ------------- 字符串处理和编码转换 -------------
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
QByteArray utf8ToGbk(const QString &utf8String)
{
    QByteArray gbkByteArray;
    QStringEncoder encoder("GBK");
    gbkByteArray = encoder.encode(utf8String);
    return gbkByteArray;
}

QString gbkToUtf8(const QByteArray &gbkByteArray)
{
    QString utf8String;
    QStringDecoder decoder("GBK");
    utf8String = decoder.decode(gbkByteArray);
    return utf8String;
}
#else
QByteArray utf8ToGbk(const QString &utf8String)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    return codec->fromUnicode(utf8String);
}

QString gbkToUtf8(const QByteArray &gbkByteArray)
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    return codec->toUnicode(gbkByteArray);
}
#endif

QString calculateMD5(const QString &text)
{
    QByteArray hashBytes = QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Md5);
    return hashBytes.toHex();
}

bool compareVersionStrings(const QString& version1, const QString& version2)
{
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

QString generateUuid(QStringList list)
{
    QString uniqueString;
    foreach (QString str, list) {
        uniqueString += str;
    }
    return calculateMD5(uniqueString);
}

// ------------- JSON处理 -------------
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

bool hasKey(const QJsonObject &obj, QString key, double &value)
{
    if (!obj.contains(key)) {
        return false;
    }
    QJsonValue v = obj.value(key);
    value = v.toDouble();
    return true;
}

// ------------- 文件操作 -------------
QString readFileContent(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        PrintErrLog("Unable to open file: " << filePath);
        return QString();
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    return content;
}

bool createFile(const QString &filePath, const QString &content)
{
    QFile file(filePath);
    if (file.exists()) {
        return true;
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        PrintErrLog("Unable to create file: " << filePath);
        return false;
    }
    QTextStream out(&file);
    out << content;
    file.close();
    return true;
}

bool appendFile(const QString &filePath, const QString &content)
{
    QFile file(filePath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        PrintErrLog("Unable to open file: " << filePath);
        return false;
    }
    QTextStream out(&file);
    out << content;
    file.close();
    return true;
}

bool copyFile(const QString &sourcePath, const QString &targetPath)
{
    QFile sourceFile(sourcePath);
    QFile targetFile(targetPath);

    if (!sourceFile.exists()) {
        qDebug() << "The source file does not exist:" << sourcePath;
        return false;
    }

    if (!sourceFile.copy(targetPath)) {
        qDebug() << "Failed to copy the file: " << sourcePath << " to " << targetPath;
        return false;
    }

    QFileInfo sourceInfo(sourcePath);
    QFile::Permissions permissions = sourceInfo.permissions();
    QFile::setPermissions(targetPath, permissions);
    return true;
}

bool copyDirectory(const QString &sourcePath, const QString &targetPath)
{
    QDir sourceDir(sourcePath);
    QDir targetDir(targetPath);

    if (!targetDir.exists()) {
        if (!targetDir.mkpath(targetDir.path())) {
            qDebug() << "Unable to create the target directory:" << targetPath;
            return false;
        }
    }

    foreach (QFileInfo entry, sourceDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString targetEntryPath = targetDir.filePath(entry.fileName());
        if (entry.isDir()) {
            if (!copyDirectory(entry.filePath(), targetEntryPath)) {
                return false;
            }
        } else {
            if (!QFile::copy(entry.filePath(), targetEntryPath)) {
                qDebug() << "Failed to copy the file: " << entry.filePath() << " to " << targetEntryPath;
                return false;
            }
        }
    }
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

bool createSymbolicLink(const QString &source, const QString &linkPath)
{
    const char *src = source.toUtf8().constData();
    const char *lnk = linkPath.toUtf8().constData();
#if defined(Q_OS_LINUX)
    int result = symlink(src, lnk);
    if (result == -1) {
        perror("Error creating symbolic link:");
        LOGGER_ERR(QString("Error creating symbolic link: ") + strerror(errno));
        return false;
    }
    return true;
#endif
    return false;
}

// ------------- 进程和系统 -------------
int runCommandWithTimeout(const QString &command, const QStringList& args, QString &output, int timeout)
{
    QProcessEnvironment originalEnv = QProcessEnvironment::systemEnvironment();

    QProcessEnvironment tempEnv = originalEnv;
    tempEnv.insert("LANG", "en_US.UTF-8");
    tempEnv.insert("LC_ALL", "en_US.UTF-8");

    QProcess process;
    process.setProcessEnvironment(tempEnv);
    process.start(command, args);
    if (!process.waitForStarted(timeout)) {
        return -1;
    }

    if (!process.waitForFinished(timeout)) {
        process.kill();
        return -1;
    }

    output = QString::fromLocal8Bit(process.readAllStandardOutput());

    process.setProcessEnvironment(originalEnv);
    qDebug() << process.errorString();
    return process.exitCode();
}

void runCommandDetached(const QString &command, const QStringList& args)
{
    QProcess::startDetached(command, args);
}

void eventPause(int msec)
{
    QEventLoop eventLoop;
    QTimer::singleShot(msec, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
}

// ------------- 网络相关 -------------
bool checkNetworkConnection(const QString &url, uint msec)
{
    static QNetworkCookieJar jar;
    static QNetworkAccessManager manager;
    manager.setCookieJar(&jar);
    manager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    QUrl urlObj(url);
    if (!urlObj.isValid()) {
        return false;
    }

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    request.setUrl(url);

    QNetworkReply* reply = manager.get(request);
    QTimer timer;
    QEventLoop loop;

    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    timer.start(msec);
    loop.exec();

    if (timer.isActive()) {
        timer.stop();
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << ("Network connection error:" + reply->errorString());
            reply->deleteLater();
            return false;
        }
        reply->deleteLater();
        return true;
    }

    reply->abort();
    reply->deleteLater();
    return false;
}

bool isLocalIP(const QString &ip)
{
    QHostAddress address(ip);
    return address.isLoopback() || address.isLinkLocal() || address.isMulticast();
}

// ====================== GUI函数 ======================
#if ENABLE_QT_WIDGETS

// ------------- 窗口部件操作 -------------
void enableWidgetWithDelay(QWidget *widget, int msec)
{
    if (!widget) {
        return;
    }

    widget->setEnabled(false);
    QTimer::singleShot(msec, nullptr, [=]() {
        widget->setEnabled(true);
    });
}

QGroupBox *createCheckBoxGroupForTable(QTableWidget *table,
                                     const QSet<int> &uncontrolableColumns,
                                     QString groupTitle)
{
    QGroupBox *groupBox = new QGroupBox(groupTitle);
    QGridLayout *layout = new QGridLayout;
    int columnCount = table->columnCount();
    int row = 0;
    int col = 0;
    const int COLS_PER_ROW = 3;

    for (int i = 0; i < columnCount; ++i) {
        if (uncontrolableColumns.contains(i)) {
            continue;
        }

        QString headerText = table->horizontalHeaderItem(i)->text();
        QCheckBox *checkBox = new QCheckBox(headerText);
        checkBox->setChecked(true);
        layout->addWidget(checkBox, row, col);

        QObject::connect(checkBox, &QCheckBox::toggled,
        [table, i](bool checked) {
            table->setColumnHidden(i, !checked);
        });

        ++col;
        if (col >= COLS_PER_ROW) {
            col = 0;
            ++row;
        }
    }

    groupBox->setLayout(layout);
    return groupBox;
}

void searchTableItem(QTableWidget *tableWidget, const QString &findStr)
{
    if (findStr.isEmpty()) {
        for (int i = 0; i < tableWidget->rowCount(); i++) {
            tableWidget->setRowHidden(i, false);
        }
        return;
    }

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        bool found = false;
        for (int j = 0; j < tableWidget->columnCount(); j++) {
            QTableWidgetItem *item = tableWidget->item(i, j);
            if (item && item->text().contains(findStr, Qt::CaseInsensitive)) {
                found = true;
                break;
            }
        }
        tableWidget->setRowHidden(i, !found);
    }
}

// ------------- 图像处理 -------------
double compareImages(const QImage &image1, const QImage &image2)
{
    const uchar *data1 = image1.constBits();
    const uchar *data2 = image2.constBits();

    int width = image1.width();
    int height = image1.height();

    int totalPixels = width * height;
    int similarPixels = 0;

    for (int i = 0; i < totalPixels; i++) {
        if (data1[i] == data2[i]) {
            similarPixels++;
        }
    }

    double similarity = (double)similarPixels / totalPixels;
    return similarity;
}

bool scalImageSize(QString filePath)
{
    QImage originalImage(filePath);
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

QPixmap base64ToPixmap(const QString &arr)
{
    QPixmap pixmap;
    pixmap.loadFromData(QByteArray::fromBase64(arr.toLocal8Bit()));
    return pixmap;
}

QString pixmapToBase64(const QPixmap &map)
{
    QImage image = map.toImage();
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    image.save(&buffer, "PNG");
    return QString::fromLatin1(byteArray.toBase64());
}

// ------------- 消息框 -------------
void showAutoCloseMessageBox(const QString &title, const QString &text, int msec)
{
    QMessageBox* msgBox = new QMessageBox(QMessageBox::Information, title, text, QMessageBox::NoButton, 0, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    msgBox->show();
    eventPause(msec);
    delete msgBox;
}

void showErrorMsgBox(const QString &message,bool isCopy)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("错误");
    msgBox.setText(message);
    if(isCopy){
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Save | QMessageBox::Apply);
    }else {
        msgBox.setStandardButtons(QMessageBox::Ok);
    }

    msgBox.setButtonText(QMessageBox::Ok, "确定");
    msgBox.setButtonText(QMessageBox::Save, "保存文本");
    msgBox.setButtonText(QMessageBox::Apply, "复制文本");
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
        QString filePath = QFileDialog::getSaveFileName(nullptr, "保存错误信息", "", "Text Files (*.txt)");
        if (!filePath.isEmpty()) {
            createFile(filePath,message);
        }
    } else if (ret == QMessageBox::Apply) {
        // 放入剪切板
        QApplication::clipboard()->setText(message);
    }
}

void showInfoMsgBox(const QString &message,bool isCopy)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("提示");
    msgBox.setText(message); // 设置消息文本
    if(isCopy){
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Save | QMessageBox::Apply);
    }else {
        msgBox.setStandardButtons(QMessageBox::Ok);
    }
    msgBox.setButtonText(QMessageBox::Ok, "确定");
    msgBox.setButtonText(QMessageBox::Save, "保存文本");
    msgBox.setButtonText(QMessageBox::Apply, "复制文本");
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
        QString filePath = QFileDialog::getSaveFileName(nullptr, "保存提示信息", "", "Text Files (*.txt)");
        if (!filePath.isEmpty()) {
            createFile(filePath,message);
        }
    }else if (ret == QMessageBox::Apply) {
        QApplication::clipboard()->setText(message);
    }
}

bool askQuestionMsgBox(const QString &message)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle("询问");
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "是");
    msgBox.setButtonText(QMessageBox::No, "否");
    return msgBox.exec() == QMessageBox::Yes;
}

#endif // ENABLE_QT_WIDGETS
