#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

class Logger
{
    Logger(QString filePath,
           size_t maxSize,
           size_t maxCount,
           spdlog::level::level_enum log_level);

    Logger(const Logger &) = delete;
    void operator=(const Logger &) = delete;
public:
    static Logger *Instance(QString filePath = "",
                            size_t maxSize = 0,
                            size_t maxCount = 0,
                            spdlog::level::level_enum log_level = spdlog::level::n_levels);

    void log(const QString &file,
             const QString &func,
             const QString &message,
             spdlog::level::level_enum log_level = spdlog::level::info);

    void setLogLevel(spdlog::level::level_enum log_level);

    spdlog::level::level_enum getLogLevel();
private:
    static Logger *instance;
    std::shared_ptr<spdlog::logger> logger;
    size_t m_maxFileSize{};
    size_t m_maxFiles{};
};

#endif // LOGGER_H
