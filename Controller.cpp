#include "Controller.h"

Controller::Controller(QVector<QVector<Map::Cell>> maze, QObject* parent)
{
	m_maze = maze;
	Init();
	
}

void Controller::AGV_up()
{
	qDebug() << "AGV up";
	m_agv->move(0, -0.1);
}
void Controller::AGV_down()
{
	qDebug() << "AGV down";
	m_agv->move(0,0.1);
}
void Controller::AGV_left()
{
	qDebug() << "AGV left";
	m_agv->move(-0.1, 0);
}
void Controller::AGV_right()
{
	qDebug() << "AGV right";
	m_agv->move(0.1,0);
}
void Controller::AGV_stop()
{
	qDebug() << "AGV stop";
	m_agv->stop();
}

void Controller::Position_update(double *x, double *y)
{
	QPointF pos = m_agv->position();
	*x = pos.x();
	*y = pos.y();
}
// 广度优先寻找最短路径
void Controller::BFS_find_shortest_path()
{

}

// 深度优先最速寻找路径
void Controller::DFS_find_shortest_path()
{
	double x_pos, y_pos;
	Position_update(&x_pos, &y_pos);
	// 当前所在格子（不是目标格子！）
	int curCellX = static_cast<int>(x_pos / 10);
	int curCellY = static_cast<int>(y_pos / 10);

	// 寻找路径
}
// 初始化函数
void Controller::Init()
{
	m_agv = new AGV(m_maze, this);
}
Controller::~Controller()
{
}
