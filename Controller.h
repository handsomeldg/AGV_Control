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

    // 小车回原
    void AGV_back(QVector<QVector<Map::Cell>> maze);
    // 开始移动（带动画）
    void startMove(QPointF from, QPointF to);

public slots:
    void AGV_up();
    void AGV_down();
    void AGV_left();
    void AGV_right();
    void AGV_stop();
    // 广度优先寻找最短路径
    void BFS_find_shortest_path();
    // 深度优先最速寻找路径
    void DFS_find_shortest_path();
private:
    
    void onMoveTick();
    // 深度优先算法
    bool DFS_find_path(int x, int y, int endCellX, int endCellY, QVector<QVector<bool>>& visited, QVector<QPoint>& path);
    AGV *m_agv;
    QVector<QVector<Map::Cell>> m_maze;

    // === 动画状态 ===
    QTimer* m_moveTimer = nullptr;

    QPointF m_startPos;     // 当前段起点（像素）
    QPointF m_targetPos;   // 当前段终点（像素）
    double m_t = 0.0;       // 0~1 的插值进度
};

