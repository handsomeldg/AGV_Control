#pragma once

#include <QtWidgets/QWidget>
#include "ui_AGV_Control.h"
#include <QDebug>
#include <QPushButton>
#include <QGridLayout>
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
private:
    Ui::AGV_ControlClass ui;

    QPushButton * btn_up;
    QPushButton * btn_down;
    QPushButton * btn_left;
    QPushButton * btn_right;
    QGridLayout* btn_layout;
};

