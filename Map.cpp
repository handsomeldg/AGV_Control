#include "Map.h"
#include <random>


// 生成10*10的地图，默认四面都是false，即没有障碍物，随机添加障碍物
// 起点00，终点1010
Map::Map(int width, int height, QVector<QVector<Cell>> map,QObject* parent)
{
    m_width = width;
    m_height = height;
    m_map = map;
    // 将所有墙初始化为true，有障碍物
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            m_map[i][j].wallUp = true;
            m_map[i][j].wallDown = true;
            m_map[i][j].wallLeft = true;
            m_map[i][j].wallRight = true;
        }
    }
}

// 生成随机地图
QVector<QVector<Map::Cell>> Map::generate()
{
    QVector<QVector<bool>> visited(m_width, QVector<bool>(m_height, false));
    carve(0, 0, visited);
    return m_map;
}

// DFS生成随机地图
// x,y为起点坐标，visited为访问过的坐标集合
void Map::carve(int x, int y, QVector<QVector<bool>>& visited)
{
    static std::mt19937 rng(std::random_device{}());
    visited[y][x] = true; // ✅ 行 = y，列 = x

    QVector<int> directions = { 0, 1, 2, 3 };
    std::shuffle(directions.begin(), directions.end(), rng);

    for (int i : directions)
    {
        int newX = x;
        int newY = y;
        switch (i)
        {
        case 0: newY--; break; // 上
        case 1: newY++; break; // 下
        case 2: newX--; break; // 左
        case 3: newX++; break; // 右
        }

        if (newX < 0 || newX >= 10 || newY < 0 || newY >= 10 || visited[newY][newX])
            continue;

        // 打通墙
        if (i == 0) { m_map[y][x].wallUp = false;    m_map[newY][newX].wallDown = false; }
        if (i == 1) { m_map[y][x].wallDown = false;  m_map[newY][newX].wallUp = false; }
        if (i == 2) { m_map[y][x].wallLeft = false;  m_map[newY][newX].wallRight = false; }
        if (i == 3) { m_map[y][x].wallRight = false; m_map[newY][newX].wallLeft = false; }

        carve(newX, newY, visited);
    }
}
Map::~Map()
{
}
