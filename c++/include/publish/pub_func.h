#ifndef PUB_FUNC_H
#define PUB_FUNC_H

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
#include <qglobal.h>

#include <QWidget>
#include <QMessageBox>
#include <QImage>
#include <QPixmap>
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
/**
 * Converts a byte array to a JSON object.
 * @param reply_data The input byte array.
 * @param obj The reference to the output JSON object.
 * @return Returns true if the conversion is successful, false otherwise.
 */
bool byteArray2JsonOBj(const QByteArray &reply_data, QJsonObject &obj);

/**
 * Checks if a JSON object contains a given key and retrieves its value if present.
 * @param obj The JSON object to search within.
 * @param key The key to look for in the JSON object.
 * @param value The reference to the variable where the value will be stored if the key is found.
 * @return Returns true if the key is found, false otherwise.
 */
bool hasKey(const QJsonObject &obj, QString key, QString &value);
bool hasKey(const QJsonObject &obj, QString key, bool &value);
bool hasKey(const QJsonObject &obj, QString key, int &value);
bool hasKey(const QJsonObject &obj, QString key, double &value);

/**
 * Reads the content of a file specified by the file path.
 * @param filePath The path of the file to read from.
 * @return Returns a string containing the file's content.
 * @note This function does not handle large files efficiently and is not recommended for such use cases.
 */
QString readFileContent(const QString &filePath);

/**
 * Creates a new file with the specified file path.
 * @param filePath The path where the file is to be created.
 * @param content Optional content to write into the file upon creation.
 * @return Returns true if the file is created successfully, false otherwise.
 */
bool createFile(const QString &filePath, const QString &content = "");

/**
 * Appends content to a file, or creates the file if it does not exist.
 * @param filePath The path of the file to append content to.
 * @param content The content to append to the file.
 * @return Returns true if the content is appended successfully, false otherwise.
 */
bool appendFile(const QString &filePath, const QString &content = "");

/**
 * Copies a file from the source path to the target path.
 * @param sourcePath The path of the source file.
 * @param targetPath The path where the file is to be copied to.
 * @return Returns true if the file is copied successfully, false otherwise.
 */
bool copyFile(const QString &sourcePath, const QString &targetPath);

/**
 * Copies a directory from the source path to the target path.
 * @param sourcePath The path of the source directory.
 * @param targetPath The path where the directory is to be copied to.
 * @return Returns true if the directory is copied successfully, false otherwise.
 */
bool copyDirectory(const QString &sourcePath, const QString &targetPath);

/**
 * Retrieves the creation date of a file.
 * @param path The path of the file.
 * @return Returns the creation date and time of the file.
 */
QDateTime getCreationDate(const QString &path);

/**
 * Creates a symbolic link that points to the source path.
 * @param source The path to the target of the symbolic link.
 * @param linkPath The path where the symbolic link is to be created.
 * @return Returns true if the symbolic link is created successfully, false otherwise.
 * @example createSymbolicLink("/home/test/123", "/tmp/123") creates a soft link for the file /home/test/123 to /tmp/123.
 */
bool createSymbolicLink(const QString &source, const QString &linkPath);

/**
 * Compares two images and returns the degree of their similarity.
 * @param image1 The first image to compare.
 * @param image2 The second image to compare.
 * @return Returns a double representing the similarity between the two images.
 */
double compareImages(const QImage &image1, const QImage &image2);

/**
 * Adjusts the size of an image based on the file path provided.
 * @param filePath The file path of the image to resize.
 * @return Returns true if the image size is adjusted successfully, false otherwise.
 */
bool scalImageSize(QString filePath);

/**
 * Executes a command in a subprocess with a specified timeout period.
 * @param command The command to be executed.
 * @param args The arguments for the command.
 * @param output The reference to a string where the subprocess output will be stored.
 * @param timeout The timeout period for the subprocess in milliseconds (default is 30000).
 * @return Returns the exit code of the subprocess.
 */
int runCommandWithTimeout(const QString &command, const QStringList &args, QString &output, int timeout = 30000);

/**
 * Executes a command in a subprocess and detaches it from the calling process.
 * @param command The command to be executed.
 * @param args The arguments for the command.
 */
void runCommandDetached(const QString &command, const QStringList& args);

/**
 * Enables a widget after a specified delay.
 * @param widget The widget to be enabled.
 * @param msec The delay in milliseconds before enabling the widget (default is 1000).
 */
void enableWidgetWithDelay(QWidget *widget, int msec = 1000);

/**
 * Pauses the execution of the program for a specified duration.
 * @param msec The duration of the pause in milliseconds.
 */
void eventPause(int msec);

/**
 * Generates a unique identifier (UUID) based on a list of strings.
 * @param list The list of strings to base the UUID generation on.
 * @return Returns a string representing the generated UUID.
 */
QString generateUuid(QStringList list);

/**
 * Calculates the MD5 hash of a given string.
 * @param text The string for which to calculate the MD5 hash.
 * @return Returns a string representing the MD5 hash.
 */
QString calculateMD5(const QString &text);

/**
 * Compares two version number strings to determine which one is greater.
 * @param version1 The first version number string to compare.
 * @param version2 The second version number string to compare.
 * @return Returns true if version1 is greater than version2, false otherwise.
 */
bool compareVersionStrings(const QString& version1, const QString& version2);

/**
 * Checks if a network connection to a specified URL is operational within a given timeout.
 * @param url The URL to check for network connectivity.
 * @param msec The timeout in milliseconds for the network operation (default is 3000).
 * @return Returns true if the network connection is operational, false otherwise.
 */
bool checkNetworkConnection(const QString &url, uint msec = 3000);

/**
 * Displays a message box with a message and title, which automatically closes after a set duration.
 * @param title The title of the message box.
 * @param text The content of the message to be displayed.
 * @param msec The duration in milliseconds the message box will be displayed before closing (default is 5000).
 */
void showAutoCloseMessageBox(const QString& title, const QString& text, int msec = 5000);


// 显示错误消息框的封装函数
void showErrorMsgBox(const QString &message);

// 显示提示消息框的封装函数
void showInfoMsgBox(const QString &message);

// 询问对话框
bool askQuestionMsgBox(const QString &message);


/**
 * Converts a base64 encoded string to a QPixmap image.
 * @param arr The base64 encoded string to convert.
 * @return Returns a QPixmap representing the image.
 */
QPixmap base64ToPixmap(const QString &arr);

/**
 * Converts a QPixmap image to a base64 encoded string.
 * @param map The QPixmap image to convert.
 * @return Returns a string containing the base64 encoded representation of the image.
 */
QString pixmapToBase64(const QPixmap &map);




// UTF-8 转 GBK
QByteArray utf8ToGbk(const QString &utf8String);

// GBK 转 UTF-8
QString gbkToUtf8(const QByteArray &gbkByteArray);

bool isLocalIP(const QString &ip);


#endif // PUB_FUNC_H



