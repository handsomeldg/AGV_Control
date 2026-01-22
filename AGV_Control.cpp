#include "AGV_Control.h"

AGV_Control::AGV_Control(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    Init();
    Layout();
}

// 初始化函数
void AGV_Control::Init()
{
    btn_up = new QPushButton("up");
    btn_down = new QPushButton("down");
    btn_left = new QPushButton("left");
    btn_right = new QPushButton("right");
}

// 布局函数
void AGV_Control::Layout()
{
    // 设置默认大小
    QSize btnSize(40, 20);   // 宽 40，高 20
    btn_up->setFixedSize(btnSize);
    btn_down->setFixedSize(btnSize);
    btn_left->setFixedSize(btnSize);
    btn_right->setFixedSize(btnSize);

    // 使用网格布局（GridLayout）
    btn_layout = new QGridLayout(this);

    // 按方向排列：
    // (0,1) 上
    // (1,0) 左  (1,1) 空格 (1,2) 右
    // (2,1) 下

    btn_layout->addWidget(btn_up, 0, 1);
    btn_layout->addWidget(btn_left, 1, 0);
    btn_layout->addWidget(btn_right, 1, 2);
    btn_layout->addWidget(btn_down, 2, 1);

    // 可选：设置网格间距
    btn_layout->setHorizontalSpacing(10);
    btn_layout->setVerticalSpacing(10);
    btn_layout->setAlignment(Qt::AlignCenter);

    setLayout(btn_layout);
}

AGV_Control::~AGV_Control()
{}

