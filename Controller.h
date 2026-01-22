#pragma once
#include <QObject>
#include <Qdebug>
class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(QObject* parent = nullptr);
    ~Controller();

public slots:
    void AGV_up();
    void AGV_down();
    void AGV_left();
    void AGV_right();
};

