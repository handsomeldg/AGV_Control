#pragma once

#include <QtWidgets/QWidget>
#include "ui_AGV_Control.h"
#include <QDebug>
#include <QPushButton>
#include <QGridLayout>
#include <QFrame>
#include "Controller.h"
#include <QLabel>
#include "Display_Area.h"

class AGV_Control : public QWidget
{
    Q_OBJECT

public:
    AGV_Control(QWidget *parent = nullptr);
    ~AGV_Control();
    // 初始化函数
    void Init();
    // 布局函数
    void Layout();
    // 信号触发
    void Signal();

signals:
    void AGV_up();
    void AGV_down();
    void AGV_left();
    void AGV_right();
    void pathReady(QVector<QPointF> path);
private:
    double posX = 0, posY = 0;
    QLabel* label_x;
    QLabel* label_y;
    Ui::AGV_ControlClass ui;

    QPushButton * btn_up;
    QPushButton * btn_down;
    QPushButton * btn_left;
    QPushButton * btn_right;
    QPushButton* btn_start;
    QPushButton* btn_DFS;
    QPushButton* btn_BFS;
    QPushButton* btn_back;
    // 右侧按键布局
    QVBoxLayout* pos_layout;
    QGridLayout* btn_layout;
    // 右侧布局
    QVBoxLayout* right_layout;
    // 主布局
    QHBoxLayout* main_layout;
    Display_Area* display_area;
    Controller* m_controller;

    QTimer* m_timer_position;
};

