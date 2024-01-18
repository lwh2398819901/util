#include "logger.h"
#include "configure.h"

Logger *Logger::instance = {};


Logger::Logger(QString filePath, size_t maxSize, size_t maxCount, spdlog::level::level_enum log_level)
{
    if (filePath.isEmpty()) {
        filePath = "./logs/log.log";
    }

    // 设置日志文件大小和数量
    logger = spdlog::rotating_logger_mt("logger", filePath.toStdString(),
                                        maxSize, maxCount);
    if (logger == nullptr) {
        exit(-1);
    }
    // 设置日志格式
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
    setLogLevel(log_level);
}

Logger *Logger::Instance(QString filePath, size_t maxSize, size_t maxCount, spdlog::level::level_enum log_level)
{
    if (!instance) {

        if (filePath.isEmpty()) {
            filePath = Configure::Instance()->getDefaultFilePath();
        }

        if (maxSize == 0) {
            maxSize = Configure::Instance()->getDefaultMaxFileSize().toInt();
        }

        if (maxSize == 0) {
            maxCount = Configure::Instance()->getDefaultMaxFiles().toInt();
        }

        if (log_level == spdlog::level::n_levels) {
            QString defaultLevel = Configure::Instance()->getDefaultLevel();
            if (defaultLevel == "trace") {
                log_level =  spdlog::level::trace;
            } else if (defaultLevel == "debug") {
                log_level =  spdlog::level::debug;
            } else if (defaultLevel == "info") {
                log_level =  spdlog::level::info;
            } else if (defaultLevel == "warn") {
                log_level =  spdlog::level::warn;
            } else if (defaultLevel == "err") {
                log_level =  spdlog::level::err;
            } else if (defaultLevel == "critical") {
                log_level =  spdlog::level::critical;
            } else if (defaultLevel == "off") {
                log_level =  spdlog::level::off;
            } else if (defaultLevel == "n_levels") {
                log_level =  spdlog::level::n_levels;
            } else {
                log_level =  spdlog::level::info;
            }
        }

        instance = new Logger(filePath, maxSize, maxCount, log_level);
    }
    return instance;
}

void Logger::log(const QString &file, const QString &func, const QString &message, spdlog::level::level_enum log_level)
{
    QString str = QString("%1: %2: %3").arg(file).arg(func).arg(message);
    logger->log(static_cast<spdlog::level::level_enum>(log_level), str.toStdString());
}

void Logger::setLogLevel(spdlog::level::level_enum log_level)
{
    logger->set_level(log_level);
    logger->flush_on(log_level);
}

spdlog::level::level_enum Logger::getLogLevel()
{
    return logger->level();
}





