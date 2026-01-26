#pragma once
#include <QObject>
#include <QTimer>
#include "Map.h"
class AGV : public QObject
{
    Q_OBJECT
public:
    AGV(QVector<QVector<Map::Cell>> maze, QObject* parent);
    ~AGV();
    // 移动当前位置
    void move(double xspeed, double yspeed);
    // 停止移动
    void stop();
    // 读取当前位置
    QPointF position() const;
    // 设置目标位置
    void setPosition(double x, double y);
    // 初始化小车（回原）
    void reset(QVector<QVector<Map::Cell>> maze);
private:
    QTimer* m_timer;
    double x_pos, y_pos, x_speed, y_speed;
    double xmin = 0, xmax = 100, ymin = 0, ymax = 100;
    QVector<QVector<Map::Cell>> m_maze;
    // 格子数量
    const int CELL = 10;
    const int R = 4;  // 小球半径
};

