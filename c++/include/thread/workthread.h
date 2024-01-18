#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include "publish.h"

#include <QObject>
typedef void (*FunctionPointer)();

enum THREAD_STATUS {
    START,
    BUSY,
    STOP
};

class WorkThread : public QObject
{
    Q_OBJECT
public:
    explicit WorkThread(FunctionPointer func);
    ~WorkThread();
public slots:
    void doWork();
    bool isWork();
    void stopWork();
    THREAD_STATUS threadStatus() {return m_status;}
signals:
    void sig_workStart();
    void sig_workStop();
private:
    FunctionPointer m_func;
    bool m_work{false};
    THREAD_STATUS m_status{STOP};
};





#endif // WORKTHREAD_H
