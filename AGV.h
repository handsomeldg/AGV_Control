#pragma once
#include <QObject>
#include <QTimer>
class AGV : public QObject
{
    Q_OBJECT
public:
    AGV(QObject* parent = nullptr);
    ~AGV();
    // 移动当前位置
    void move(double xspeed, double yspeed);
    // 停止移动
    void stop();
    // 读取当前位置
    QPointF position() const;


private:
    QTimer* m_timer;
    double x_pos, y_pos, x_speed, y_speed;
    double xmin = 0, xmax = 1000, ymin = 0, ymax = 1000;
};

