#pragma once
#include <QFrame>
#include "Map.h"
class Display_Area : public QFrame {
    Q_OBJECT
public:
    explicit Display_Area(QWidget* parent = nullptr);
    void setPath(const QVector<QPointF>& path);   // ⭐ 新增
public slots:
    void setAGVPosition(double* x, double* y);
    void setMaze(const QVector<QVector<Map::Cell>>& maze);
    QVector<QVector<Map::Cell>> getMaze();
    // 生成随机地图
    void generateMaze();
    // 画已经走过的线
    void setReachedIndex(int reachedIndex);
    // 画已经走过的线
    void visitPathIndex(QVector<QPointF> visitpath);
protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<QPointF> m_path;   // ⭐ 保存轨迹
    QVector<QPointF> m_visitpath;   // ⭐ 保存已经走过的点
    int m_reachedIndex;   // ⭐ 保存已经走过的点
    QPointF m_pos;   // AGV 当前位置
    Map* m_map;
    QVector<QVector<Map::Cell>> maze_plan;
};

