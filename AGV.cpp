#include "AGV.h"

AGV::AGV(QObject* parent)
{
	m_timer = new QTimer(this);
	x_pos = 0;
	y_pos = 0;
	x_speed = 0;
	y_speed = 0;

	connect(m_timer, &QTimer::timeout, this, [=]() {
		double newX = x_pos + x_speed;
		double newY = y_pos + y_speed;

		if (newX < xmin || newX > xmax ||
			newY < ymin || newY > ymax)
		{
			m_timer->stop();
			return;
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
