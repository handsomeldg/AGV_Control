#include "Display_Area.h"
#include <QPainter>

Display_Area::Display_Area(QWidget* parent)
    : QFrame(parent)
{
    int rows = 10;
    int cols = 10;

    QVector<QVector<Map::Cell>> map(rows, QVector<Map::Cell>(cols));
    m_map = new Map(10,10, map, this);
    maze_plan = m_map->generate();
    setFrameShape(QFrame::Box);
    setStyleSheet("background-color: white;");
}
// 生成随机地图
void Display_Area::generateMaze()
{
    maze_plan = m_map->generate();
    update();
}
void Display_Area::setMaze(const QVector<QVector<Map::Cell>>& maze)
{
    maze_plan = maze;

    update();
}
QVector<QVector<Map::Cell>> Display_Area::getMaze()
{
    return maze_plan;
}
void Display_Area::setAGVPosition(double *x, double *y)
{
    m_pos.setX(*x);
    m_pos.setY(*y);
    update();   // 触发重绘
}
void Display_Area::setPath(const QVector<QPointF>& path)   // ⭐ 新增
{
    m_path = path;
    update();   // 触发重绘
}
// 画已经走过的线
void Display_Area::setReachedIndex(int reachedIndex)
{
    m_reachedIndex = reachedIndex;
    update();   // 触发重绘
}

// 画已经走过的线
void Display_Area::visitPathIndex(QVector<QPointF> visitpath)
{
    m_visitpath = visitpath;
    update();   // 触发重绘
}

void Display_Area::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // ===== 1️⃣ 清空背景 =====
    painter.fillRect(rect(), Qt::white);   // 或 Qt::black / Qt::lightGray

    if (maze_plan.isEmpty())
        return;

    int H = maze_plan.size();
    int W = maze_plan[0].size();

    int cw = width() / W;
    int ch = height() / H;

    painter.setPen(QPen(Qt::black, 2));

    // ===== 2️⃣ 画迷宫 =====
    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            int sx = x * cw;
            int sy = y * ch;

            const Map::Cell& c = maze_plan[y][x];

            if (c.wallUp)
                painter.drawLine(sx, sy, sx + cw, sy);
            if (c.wallDown)
                painter.drawLine(sx, sy + ch, sx + cw, sy + ch);
            if (c.wallLeft)
                painter.drawLine(sx, sy, sx, sy + ch);
            if (c.wallRight)
                painter.drawLine(sx + cw, sy, sx + cw, sy + ch);
        }
    }

    // ===== 3️⃣ 画 AGV =====
    double scaleX = width() / 100.0;
    double scaleY = height() / 100.0;

    QPointF screenPos(m_pos.x() * scaleX,
        m_pos.y() * scaleY);

    painter.setBrush(Qt::blue);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(screenPos, 8, 8);  // 半径 8px

    painter.setPen(QPen(Qt::lightGray, 2));

    for (int i = 1; i < m_path.size(); i++)
    {
        if (i <= m_reachedIndex)
            ;
            //painter.setPen(QPen(Qt::lightGray, 2));
        else
        { 
            painter.setPen(QPen(Qt::green, 2));
        painter.drawLine((m_path[i-1].x() * 10 + 5)* scaleX, (m_path[i - 1].y() * 10 + 5)* scaleY, (m_path[i].x() * 10 + 5)* scaleX, (m_path[i].y() * 10 + 5)* scaleY);
        }
    }

    for (int i = 1; i < m_visitpath.size(); i++)
    {
        painter.setPen(QPen(Qt::lightGray, 2));
        painter.drawLine((m_visitpath[i - 1].x()) * scaleX, (m_visitpath[i - 1].y()) * scaleY, (m_visitpath[i].x()) * scaleX, (m_visitpath[i].y()) * scaleY);
    }
}
