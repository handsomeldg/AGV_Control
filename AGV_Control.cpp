#include "AGV_Control.h"

AGV_Control::AGV_Control(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    Init();
    Layout();
    m_timer_position->start(10);
    Signal();
}

// 初始化函数
void AGV_Control::Init()
{
    m_timer_position = new QTimer(this);
    label_x = new QLabel("X: 0.0", this);
    label_y = new QLabel("Y: 0.0", this);
    btn_up = new QPushButton("up");
    btn_down = new QPushButton("down");
    btn_left = new QPushButton("left");
    btn_right = new QPushButton("right");
    // 使用网格布局（GridLayout）
    btn_layout = new QGridLayout();
    pos_layout = new QVBoxLayout();
    display_area = new QFrame(this);

    right_layout = new QVBoxLayout();

    main_layout = new QHBoxLayout(this);

    m_controller = new Controller(this);
}

// 布局函数
void AGV_Control::Layout()
{
    // 设置默认大小
    QSize btnSize(40, 20);   // 宽 40，高 20
    label_x->setAlignment(Qt::AlignRight);
    label_y->setAlignment(Qt::AlignRight);

    QFont font;
    font.setPointSize(12);
    label_x->setFont(font);
    label_y->setFont(font);

    btn_up->setFixedSize(btnSize);
    btn_down->setFixedSize(btnSize);
    btn_left->setFixedSize(btnSize);
    btn_right->setFixedSize(btnSize);
    // 按方向排列：
    // (0,1) 上
    // (1,0) 左  (1,1) 空格 (1,2) 右
    // (2,1) 下
    // ====== 右上角布局 ======

    pos_layout->addWidget(label_x);
    pos_layout->addWidget(label_y);
    pos_layout->setSpacing(4);
    pos_layout->setContentsMargins(0, 0, 0, 0);
    btn_layout->addWidget(btn_up, 0, 1);
    btn_layout->addWidget(btn_left, 1, 0);
    btn_layout->addWidget(btn_right, 1, 2);
    btn_layout->addWidget(btn_down, 2, 1);
    
    btn_layout->setHorizontalSpacing(5);
    btn_layout->setVerticalSpacing(5);
    btn_layout->setContentsMargins(0, 0, 0, 0);
    
    // --- 把按钮放到右下角 ---
    right_layout->addLayout(pos_layout);
    right_layout->addStretch();       // 上方空白撑开
    right_layout->addLayout(btn_layout);
    right_layout->setAlignment(btn_layout, Qt::AlignRight | Qt::AlignBottom);


    display_area->setFrameShape(QFrame::Box);  // 可见边框
    display_area->setStyleSheet("background-color: white;"); // 白色背景
    display_area->setMinimumSize(400, 300);    // 默认大小

    // --- 主布局，显示界面 + 右边按钮 ---
    
    main_layout->addWidget(display_area, 1); // displayArea 占满大部分
    main_layout->addLayout(right_layout);    // 按钮放右下
    main_layout->setContentsMargins(10, 10, 10, 10);

    setLayout(main_layout);
    setWindowTitle("AGV simulation display interface");
    resize(600, 400);
}


// 信号触发
void AGV_Control::Signal()
{
    connect(btn_up, &QPushButton::pressed,m_controller, &Controller::AGV_up);
    connect(btn_up, &QPushButton::released, m_controller, &Controller::AGV_stop);

    connect(btn_down, &QPushButton::pressed, m_controller, &Controller::AGV_down);
    connect(btn_down, &QPushButton::released, m_controller, &Controller::AGV_stop);

    connect(btn_left, &QPushButton::pressed, m_controller, &Controller::AGV_left);
    connect(btn_left, &QPushButton::released, m_controller, &Controller::AGV_stop);

    connect(btn_right, &QPushButton::pressed, m_controller, &Controller::AGV_right);
    connect(btn_right, &QPushButton::released, m_controller, &Controller::AGV_stop);

    connect(m_timer_position, &QTimer::timeout, this, [=]() {
        QPointF pos = m_controller->Position_update();
        label_x->setText("X: " + QString::number(pos.x(), 'f', 2));
        label_y->setText("Y: " + QString::number(pos.y(), 'f', 2));
        });
}

QPointF Controller::Position_update(void)
{
    return m_agv->position();
}

AGV_Control::~AGV_Control()
{}

