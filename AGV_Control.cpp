#include "AGV_Control.h"
#include <QtConcurrent/QtConcurrent>
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
    btn_start = new QPushButton("start");
    btn_DFS = new QPushButton("DFS");
    btn_BFS = new QPushButton("BFS");
    btn_back = new QPushButton("back");
    btn_up = new QPushButton("up");
    btn_down = new QPushButton("down");
    btn_left = new QPushButton("left");
    btn_right = new QPushButton("right");
    // 使用网格布局（GridLayout）
    btn_layout = new QGridLayout();
    pos_layout = new QVBoxLayout();
    display_area = new Display_Area(this);

    right_layout = new QVBoxLayout();

    main_layout = new QHBoxLayout(this);

    m_controller = new Controller(display_area->getMaze(), this);
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
    btn_start->setFixedSize(btnSize);
    btn_DFS->setFixedSize(btnSize);
    btn_BFS->setFixedSize(btnSize);
    btn_back->setFixedSize(btnSize);
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
    btn_layout->addWidget(btn_start, 1, 1);
    btn_layout->addWidget(btn_DFS, 0, 0);
    btn_layout->addWidget(btn_BFS, 0, 2);
    btn_layout->addWidget(btn_left, 1, 0);
    btn_layout->addWidget(btn_right, 1, 2);
    btn_layout->addWidget(btn_down, 2, 1);
    btn_layout->addWidget(btn_back, 2, 0);
    
    btn_layout->setHorizontalSpacing(5);
    btn_layout->setVerticalSpacing(5);
    btn_layout->setContentsMargins(0, 0, 0, 0);
    
    // --- 把按钮放到右下角 ---
    right_layout->addLayout(pos_layout);
    right_layout->addStretch();       // 中间空白撑开
    right_layout->addLayout(btn_layout);
    right_layout->setAlignment(btn_layout, Qt::AlignRight | Qt::AlignBottom);


    display_area->setMinimumSize(400, 300);

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
    connect(m_controller, &Controller::reachedIndexChanged, display_area, &Display_Area::setReachedIndex);
    connect(m_controller, &Controller::visitPathChanged, display_area, &Display_Area::visitPathIndex);
    connect(btn_back, &QPushButton::clicked, this, [=]() {
        auto maze = display_area->getMaze();
        m_controller->AGV_back(maze); 
        QVector<QPointF> path;   // ⭐ 保存轨迹
        emit pathReady(path);   // ✅ 线程安全
        display_area->setPath(path);
        });
    connect(btn_start, &QPushButton::clicked, this, [=]() {
            display_area->generateMaze();
            auto maze = display_area->getMaze();
            m_controller->AGV_back(maze);
            QVector<QPointF> path;   // ⭐ 保存轨迹
            emit pathReady(path);   // ✅ 线程安全
            display_area->setPath(path);
        }
        );
    connect(btn_BFS, &QPushButton::clicked, this, [=]() {
        QtConcurrent::run([=]() {
            auto path = m_controller->BFS_find_shortest_path(); 
            emit pathReady(path);   // ✅ 线程安全
            display_area->setPath(path);
            }
        ); }
    );
    connect(this, &AGV_Control::pathReady, m_controller, &Controller::startMoveAlongPath,
        Qt::QueuedConnection);
    connect(btn_DFS, &QPushButton::clicked, this, [=]() {
        QtConcurrent::run([=]() {
            auto path = m_controller->DFS_find_shortest_path();
            emit pathReady(path);   // ✅ 线程安全
            display_area->setPath(path);
            }
        ); }
    );
    connect(btn_up, &QPushButton::pressed,m_controller, &Controller::AGV_up);
    connect(btn_up, &QPushButton::released, m_controller, &Controller::AGV_stop);

    connect(btn_down, &QPushButton::pressed, m_controller, &Controller::AGV_down);
    connect(btn_down, &QPushButton::released, m_controller, &Controller::AGV_stop);

    connect(btn_left, &QPushButton::pressed, m_controller, &Controller::AGV_left);
    connect(btn_left, &QPushButton::released, m_controller, &Controller::AGV_stop);

    connect(btn_right, &QPushButton::pressed, m_controller, &Controller::AGV_right);
    connect(btn_right, &QPushButton::released, m_controller, &Controller::AGV_stop);

    connect(m_timer_position, &QTimer::timeout, this, [=]() {
        m_controller->Position_update(&posX, &posY);
        label_x->setText("X: " + QString::number(posX, 'f', 2));
        label_y->setText("Y: " + QString::number(posY, 'f', 2));
        display_area->setAGVPosition(&posX, &posY);
        });

   // connect(m_controller, &Controller::Position_update, display_area, &Display_Area::setAGVPosition);
}



AGV_Control::~AGV_Control()
{}

