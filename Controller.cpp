#include "Controller.h"
#include <QThread> 
#include <QtConcurrent/QtConcurrent>
Controller::Controller(QVector<QVector<Map::Cell>> maze, QObject* parent)
{
	m_maze = maze;
	Init();
	
}

void Controller::AGV_up()
{
	qDebug() << "AGV up";
	m_agv->move(0, -0.1);
}
void Controller::AGV_down()
{
	qDebug() << "AGV down";
	m_agv->move(0,0.1);
}
void Controller::AGV_left()
{
	qDebug() << "AGV left";
	m_agv->move(-0.1, 0);
}
void Controller::AGV_right()
{
	qDebug() << "AGV right";
	m_agv->move(0.1,0);
}
void Controller::AGV_stop()
{
	qDebug() << "AGV stop";
	m_agv->stop();
}

void Controller::Position_update(double *x, double *y)
{
	QPointF pos = m_agv->position();
	*x = pos.x();
	*y = pos.y();
}
// 广度优先寻找最短路径
void Controller::BFS_find_shortest_path()
{
	double x_pos, y_pos;
	Position_update(&x_pos, &y_pos);
	// 当前所在格子（不是目标格子！）
	int curCellX = static_cast<int>(x_pos / 10);
	int curCellY = static_cast<int>(y_pos / 10);
	int endX = 9;
	int endY = 9;
	int nx, ny;
	const int W = 10;
	const int H = 10;
	QVector<QVector<bool>> visited(H, QVector<bool>(W, false));
	QVector<QVector<QPoint>> parent(H, QVector<QPoint>(W, QPoint(-1, -1)));

	QQueue<QPoint> queue;
	visited[curCellY][curCellX] = true;
	queue.enqueue(QPoint(curCellX, curCellY));
	bool found = false;
	while (!queue.empty())
	{
		QThread::msleep(100);
		QPoint cur = queue.dequeue();
		int x = cur.x();
		int y = cur.y();
		qDebug() << y << "x" << x;
		m_agv->setPosition((x * 10) + 4, (y * 10) + 4);
		if (x == endX && y == endY)
		{
			found = true;
			break;
		}

		const auto& cell = m_maze[y][x];

		// Down
		if (!cell.wallDown)
		{
			nx = x;
			ny = y + 1;

			if (nx >= 0 && nx < W && ny >= 0 && ny < H && !visited[ny][nx])
			{
				visited[ny][nx] = true;
				parent[ny][nx] = QPoint(x, y);
				queue.enqueue(QPoint(nx, ny));
			}
		}

		// Left
		if (!cell.wallLeft)
		{
			nx = x - 1;
			ny = y;

			if (nx >= 0 && nx < W && ny >= 0 && ny < H && !visited[ny][nx])
			{
				visited[ny][nx] = true;
				parent[ny][nx] = QPoint(x, y);
				queue.enqueue(QPoint(nx, ny));
			}
		}

		// Right
		if (!cell.wallRight)
		{
			nx = x + 1;
			ny = y;

			if (nx >= 0 && nx < W && ny >= 0 && ny < H && !visited[ny][nx])
			{
				visited[ny][nx] = true;
				parent[ny][nx] = QPoint(x, y);
				queue.enqueue(QPoint(nx, ny));
			}
		}

		// Up
		if (!cell.wallUp)
		{
			nx = x;
			ny = y - 1;

			if (nx >= 0 && nx < W && ny >= 0 && ny < H && !visited[ny][nx])
			{
				visited[ny][nx] = true;
				parent[ny][nx] = QPoint(x, y);
				queue.enqueue(QPoint(nx, ny));
			}
		}
	}
	if (!found)
	{
		qDebug() << "No path found!";
		return;
	}

	// 回溯
	QVector<QPoint> path;
	QPoint cur(endY, endX);
	while (cur != QPoint(-1, -1))
	{
		path.append(cur);
		cur = parent[cur.y()][cur.x()];
		
	}
	for(int i = 0;i< path.size();i++)
	{
		qDebug() << path[i] <<"  ";
	}
}

// 深度优先最速寻找路径
void Controller::DFS_find_shortest_path()
{
	double x_pos, y_pos;
	Position_update(&x_pos, &y_pos);
	// 当前所在格子（不是目标格子！）
	int curCellX = static_cast<int>(x_pos / 10);
	int curCellY = static_cast<int>(y_pos / 10);
	// 监控格子是否被访问过
	QVector<QVector<bool>> visited(10, QVector<bool>(10,false ) );
	// 存放走过的路径
	QVector<QPoint> path;
	// 寻找路径
	DFS_find_path(curCellX, curCellY, 9, 9, visited, path);
	
}
// 深度优先算法
bool Controller::DFS_find_path(int x, int y, int endCellX, int endCellY, QVector<QVector<bool>>& visited, QVector<QPoint>& path)
{
	QThread::msleep(100);
	if (x < 0 || x > 10 || y < 0 || y > 10)
		return false;
	if (visited[y][x])
		return false;
	qDebug() << y << "x" << x;
	visited[y][x] = true;
	m_agv->setPosition((x * 10) + 4, (y * 10) + 4);
	
	path.append(QPoint(x, y));
	if (x == endCellX && y == endCellY)
		return true;
	const auto& cell = m_maze[y][x];
	if (!cell.wallDown)
		if (DFS_find_path(x, y + 1, endCellX, endCellY, visited, path))
			return true;

	if (!cell.wallLeft)
		if (DFS_find_path(x - 1, y, endCellX, endCellY, visited, path))
			return true;

	if (!cell.wallRight)
		if (DFS_find_path(x + 1, y, endCellX, endCellY, visited, path))
			return true;

	if (!cell.wallUp)
		if (DFS_find_path(x, y - 1, endCellX, endCellY, visited, path))
			return true;
	path.removeLast();
	return false;
}
void Controller::startMove(QPointF from, QPointF to)
{
	m_startPos = from;
	m_targetPos = to;
	m_t = 0.0;

	if (!m_moveTimer)
	{
		m_moveTimer = new QTimer(this);
		connect(m_moveTimer, &QTimer::timeout,
			this, &Controller::onMoveTick);
	}

	m_moveTimer->start(16);   // ~60 FPS
}
void Controller::onMoveTick()
{
	const double speed = 0.05;   // 越小越慢（0.02~0.1 都行）

	m_t += speed;
	if (m_t >= 1.0)
	{
		// 到达终点
		m_agv->setPosition(m_targetPos.x(), m_targetPos.y());
		m_moveTimer->stop();
		return;
	}

	// 线性插值：pos = start + (target - start) * t
	QPointF pos = m_startPos + (m_targetPos - m_startPos) * m_t;
	m_agv->setPosition(pos.x(), pos.y());
}
// 小车回原
void Controller::AGV_back(QVector<QVector<Map::Cell>> maze)
{
	m_agv->reset(maze);
	startMove(m_agv->position(), QPointF(4, 4));
}
// 初始化函数
void Controller::Init()
{
	m_agv = new AGV(m_maze, this);
}
Controller::~Controller()
{
}
