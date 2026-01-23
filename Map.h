#pragma once
#include <QObject>
#include <QRandomGenerator>
class Map : public QObject
{
    Q_OBJECT
public:
    struct Cell {
        bool wallUp;
        bool wallDown;
        bool wallLeft;
        bool wallRight;
    };
    Map(int width, int height, QVector<QVector<Map::Cell>> map, QObject* parent);
    ~Map();


    // 生成随机地图
    QVector<QVector<Cell>> generate();
private:
    // DFS生成随机地图
    // x,y为起点坐标，visited为访问过的坐标集合
    void carve(int x, int y, QVector<QVector<bool>>& visited);
    int m_width , m_height;
    QVector<QVector<Map::Cell>> m_map;
};

