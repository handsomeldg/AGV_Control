#include "Controller.h"

Controller::Controller(QObject* parent)
{

}

void Controller::AGV_up()
{
	qDebug() << "AGV up";
}
void Controller::AGV_down()
{
	qDebug() << "AGV down";
}
void Controller::AGV_left()
{
	qDebug() << "AGV left";
}
void Controller::AGV_right()
{
	qDebug() << "AGV right";
}
Controller::~Controller()
{
}
