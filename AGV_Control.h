#pragma once

#include <QtWidgets/QWidget>
#include "ui_AGV_Control.h"
#include <QDebug>
#include <QPushButton>
#include <QGridLayout>
#include <QFrame>
#include "Controller.h"
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
private:
    Ui::AGV_ControlClass ui;

    QPushButton * btn_up;
    QPushButton * btn_down;
    QPushButton * btn_left;
    QPushButton * btn_right;
    // 右侧按键布局
    QGridLayout* btn_layout;
    // 右侧布局
    QVBoxLayout* right_layout;
    // 主布局
    QHBoxLayout* main_layout;
    QFrame* display_area;
    Controller* m_controller;
};

