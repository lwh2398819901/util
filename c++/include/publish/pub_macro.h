#ifndef PUB_MACRO_H
#define PUB_MACRO_H

#include <QDebug>
#include "logger.h"
//#define TEST_APP

#define PrintLog(x) qDebug()<<"file:"<<__FILE__<<" func:"<<__func__<<" line:"<<__LINE__<<" msg:"<<x
#define PrintErrLog(x)  qDebug()<<"errorLog:  file:"<<__FILE__<<" func:"<<__func__<<" line:"<<__LINE__<<" msg:"<<x

#define LOGGER_TRACE(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::trace);
#define LOGGER_DEBUG(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::debug);
#define LOGGER_INFO(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::info);
#define LOGGER_WARN(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::warn);
#define LOGGER_ERR(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::err);
#define LOGGER_CRITICAL(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::critical);

#define LOGGER_TRACE_P(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::trace);PrintLog(msg)ss
#define LOGGER_DEBUG_P(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::debug);PrintLog(msg)
#define LOGGER_INFO_P(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::info);PrintLog(msg)
#define LOGGER_WARN_P(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::warn);PrintLog(msg)
#define LOGGER_ERR_P(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::err);PrintErrLog(msg)
#define LOGGER_CRITICAL_P(msg) Logger::Instance()->log(__FILE__,__func__,msg,spdlog::level::critical);PrintLog(msg)

const int Second = 1000;

#endif // PUB_MACRO_H
