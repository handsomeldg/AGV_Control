#include "Controller.h"

Controller::Controller(QObject* parent)
{
	Init();
	
}

void Controller::AGV_up()
{
	qDebug() << "AGV up";
	m_agv->move(-0.01,0);
}
void Controller::AGV_down()
{
	qDebug() << "AGV down";
	m_agv->move(0.01,0);
}
void Controller::AGV_left()
{
	qDebug() << "AGV left";
	m_agv->move(0,-0.01);
}
void Controller::AGV_right()
{
	qDebug() << "AGV right";
	m_agv->move(0, 0.01);
}
void Controller::AGV_stop()
{
	qDebug() << "AGV stop";
	m_agv->stop();
}

// ³õÊ¼»¯º¯Êý
void Controller::Init()
{
	m_agv = new AGV(this);
}
Controller::~Controller()
{
}
