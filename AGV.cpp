#include "AGV.h"
#include "QDebug"
AGV::AGV(QVector<QVector<Map::Cell>> maze,QObject* parent)
{
	m_timer = new QTimer(this);
	x_pos = R;
	y_pos = R;
	x_speed = 0;
	y_speed = 0;
	m_maze = maze;
	connect(m_timer, &QTimer::timeout, this, [=]() {
		double newX = x_pos + x_speed;
		double newY = y_pos + y_speed;
		if (newX < xmin || newX > xmax ||
			newY < ymin || newY > ymax)
		{
			m_timer->stop();
			return;
		}
        // 当前所在格子（不是目标格子！）
        int curCellX = static_cast<int>(x_pos / CELL);
        int curCellY = static_cast<int>(y_pos / CELL);

        // —— 2. 向右 ——
        if (x_speed > 0)
        {
            double probeX = newX + R;     // 前探点
            int nextCellX = static_cast<int>(probeX / CELL);
            double probeY = newY + R;
            int nextCellY = static_cast<int>(probeY / CELL);
            if (nextCellX != curCellX)    // 即将跨格子
            {
                if (m_maze[curCellY][curCellX].wallRight)
                {
                    m_timer->stop();
                    return;
                }
            }
        }

        // —— 3. 向左 ——
        else if (x_speed < 0)
        {
            double probeX = newX - R;
            int nextCellX = static_cast<int>(probeX / CELL);

            if (nextCellX != curCellX)
            {
                if (m_maze[curCellY][curCellX].wallLeft)
                {
                    m_timer->stop();
                    return;
                }
            }
        }

        // —— 4. 向下 ——
        if (y_speed > 0)
        {
            double probeY = newY + R;
            int nextCellY = static_cast<int>(probeY / CELL);

            if (nextCellY != curCellY)
            {
                if (m_maze[curCellY][curCellX].wallDown)
                {
                    m_timer->stop();
                    return;
                }
            }
        }

        // —— 5. 向上 ——
        else if (y_speed < 0)
        {
            double probeY = newY - R;
            int nextCellY = static_cast<int>(probeY / CELL);

            if (nextCellY != curCellY)
            {
                if (m_maze[curCellY][curCellX].wallUp)
                {
                    m_timer->stop();
                    return;
                }
            }
        }
		x_pos = newX;
		y_pos = newY;
		});
}
// 移动当前位置
void AGV::move(double xspeed, double yspeed)
{
	x_speed = xspeed;
	y_speed = yspeed;
	m_timer->start(1);
}

// 初始化小车
void AGV::reset(QVector<QVector<Map::Cell>> maze)
{
	m_maze = maze;
}

// 设置目标位置
void AGV::setPosition(double x, double y)
{
    x_pos = x;
    y_pos = y;
}
// 停止移动
void AGV::stop()
{
	m_timer->stop();
}
// 读取当前位置
QPointF AGV::position() const
{
	return QPointF(x_pos, y_pos);
}
AGV::~AGV()
{
}
