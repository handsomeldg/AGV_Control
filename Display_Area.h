#pragma once
#include <QFrame>
#include "Map.h"
class Display_Area : public QFrame {
    Q_OBJECT
public:
    explicit Display_Area(QWidget* parent = nullptr);

public slots:
    void setAGVPosition(double* x, double* y);
    void setMaze(const QVector<QVector<Map::Cell>>& maze);
    QVector<QVector<Map::Cell>> getMaze();
protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPointF m_pos;   // AGV µ±«∞Œª÷√
    Map *m_map;
    QVector<QVector<Map::Cell>> maze_plan;
};

