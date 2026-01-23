#pragma once
#include <QObject>
#include <Qdebug>
#include "AGV.h"
#include <QTimer>
#include "Map.h"
class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(QVector<QVector<Map::Cell>> maze,QObject* parent = nullptr);
    ~Controller();
    // 初始化函数
    void Init();
    // 信号触发
    void Signal();

    void Position_update(double* x, double* y);
public slots:
    void AGV_up();
    void AGV_down();
    void AGV_left();
    void AGV_right();
    void AGV_stop();

private:
    // 广度优先寻找最短路径
    void BFS_find_shortest_path();
    // 深度优先最速寻找路径
    void DFS_find_shortest_path();
    AGV *m_agv;
    QVector<QVector<Map::Cell>> m_maze;
};

