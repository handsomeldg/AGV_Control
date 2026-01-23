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

// ³õÊ¼»¯º¯Êý
void Controller::Init()
{
	m_agv = new AGV(m_maze, this);
}
Controller::~Controller()
{
}
