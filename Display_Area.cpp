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

void Display_Area::paintEvent(QPaintEvent* event)
{
    QPainter p(this);

    if (maze_plan.isEmpty()) return;

    int H = maze_plan.size();
    int W = maze_plan[0].size();

    int cw = width() / W;
    int ch = height() / H;

    p.setPen(QPen(Qt::black, 2));

    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            int sx = x * cw;
            int sy = y * ch;

            const Map::Cell& c = maze_plan[y][x];

            if (c.wallUp)
                p.drawLine(sx, sy, sx + cw, sy);
            if (c.wallDown)
                p.drawLine(sx, sy + ch, sx + cw, sy + ch);
            if (c.wallLeft)
                p.drawLine(sx, sy, sx, sy + ch);
            if (c.wallRight)
                p.drawLine(sx + cw, sy, sx + cw, sy + ch);
        }
    }

    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // ===== 坐标映射（AGV 坐标 → 屏幕坐标）=====
    double scaleX = width() / 100.0;   // 你的地图宽度
    double scaleY = height() / 100.0;   // 你的地图高度

    QPointF screenPos(m_pos.x() * scaleX,
        m_pos.y() * scaleY);

    // ===== 画 AGV =====
    painter.setBrush(Qt::blue);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(screenPos, 8, 8);  // 半径 8px 小圆点

}
