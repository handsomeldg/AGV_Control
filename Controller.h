#pragma once
#include <QObject>
#include <Qdebug>
#include "AGV.h"
#include <QTimer>
class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(QObject* parent = nullptr);
    ~Controller();
    // 初始化函数
    void Init();
    // 信号触发
    void Signal();

    QPointF Position_update(void);
public slots:
    void AGV_up();
    void AGV_down();
    void AGV_left();
    void AGV_right();
    void AGV_stop();

private:
    AGV *m_agv;
    
};

