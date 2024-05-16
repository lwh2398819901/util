
#include "workthread.h"


WorkThread::WorkThread(FunctionPointer func):
    m_func(func)
{

}

WorkThread::~WorkThread()
{
    LOGGER_DEBUG("~WorkThread()");
}

void WorkThread::doWork()
{
    LOGGER_DEBUG("WorkThread  Start");
    emit sig_workStart();
    m_status = THREAD_STATUS::START;
    m_work = true;
    while (m_work) {
        m_status = THREAD_STATUS::BUSY;
        m_func();
    }
    m_status = THREAD_STATUS::STOP;
    emit sig_workStop();
    LOGGER_DEBUG("WorkThread Finish");
}



bool WorkThread::isWork()
{
    return m_work;
}

void WorkThread::stopWork()
{
    m_work = false;
}


