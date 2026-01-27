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
	qDebug() << (int )m_common->state();
	if (m_common->state() == Common::State::Searching || m_common->state() == Common::State::AnimatingPath)
	{
		qDebug() << "State not idle!";
		return ;
	}
	m_common->setState(Common::State::ManualMove);
	qDebug() << "AGV up";
	m_agv->move(0, -0.1);
}
void Controller::AGV_down()
{
	if (m_common->state() == Common::State::Searching || m_common->state() == Common::State::AnimatingPath)
	{
		qDebug() << "State not idle!";
		return ;
	}
	m_common->setState(Common::State::ManualMove);
	qDebug() << "AGV down";
	m_agv->move(0,0.1);
}
void Controller::AGV_left()
{
	if (m_common->state() == Common::State::Searching || m_common->state() == Common::State::AnimatingPath)
	{
		qDebug() << "State not idle!";
		return ;
	}
	m_common->setState(Common::State::ManualMove);
	qDebug() << "AGV left";
	m_agv->move(-0.1, 0);
}
void Controller::AGV_right()
{
	if (m_common->state() == Common::State::Searching || m_common->state() == Common::State::AnimatingPath)
	{
		qDebug() << "State not idle!";
		return ;
	}
	m_common->setState(Common::State::ManualMove);
	qDebug() << "AGV right";
	m_agv->move(0.1,0);
}
void Controller::AGV_stop()
{
	if (m_common->state() == Common::State::Searching || m_common->state() == Common::State::AnimatingPath)
	{
		qDebug() << "State not idle!";
		return;
	}
	m_common->setState(Common::State::Idle);
	qDebug() << "AGV stop";
	m_agv->stop();
}

void Controller::Position_update(double *x, double *y)
{
	QPointF pos = m_agv->position();
	*x = pos.x();
	*y = pos.y();
}

QVector<QPointF> smoothPath(const QVector<QPointF>& input, int iter = 2)
{
	QVector<QPointF> pts = input;

	for (int k = 0; k < iter; ++k)
	{
		QVector<QPointF> newPts;
		newPts.append(pts.first());

		for (int i = 0; i < pts.size() - 1; ++i)
		{
			QPointF p0 = pts[i];
			QPointF p1 = pts[i + 1];

			QPointF q = 0.75 * p0 + 0.25 * p1;
			QPointF r = 0.25 * p0 + 0.75 * p1;

			newPts.append(q);
			newPts.append(r);
		}

		newPts.append(pts.last());
		pts = newPts;
	}

	return pts;
}

// 广度优先寻找最短路径
QVector<QPointF> Controller::BFS_find_shortest_path()
{
	if (m_common->state() != Common::State::Idle)
	{
		qDebug() << "State not idle!";
		return m_path;
	}
	m_common->setState(Common::State::Searching);
	m_visitpath.clear();
	m_visitpath.append(m_agv->position());
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
	QVector<QVector<QPointF>> parent(H, QVector<QPointF>(W, QPoint(-1, -1)));

	QQueue<QPointF> queue;
	visited[curCellY][curCellX] = true;
	queue.enqueue(QPointF(curCellX, curCellY));
	bool found = false;
	QVector<QPointF> path1;
	while (!queue.empty())
	{
		QPointF cur = queue.dequeue();
		int x = cur.x();
		int y = cur.y();
		qDebug() << y << "x" << x;
		path1.append(QPointF(x, y));
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
				parent[ny][nx] = QPointF(x, y);
				queue.enqueue(QPointF(nx, ny));
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
				parent[ny][nx] = QPointF(x, y);
				queue.enqueue(QPointF(nx, ny));
			}
		}
	}
	if (!found)
	{
		qDebug() << "No path found!";
		return path1;
	}
	//startMoveAlongPath(path1);
	// 回溯
	QVector<QPointF> path;
	QPointF cur(endY, endX);
	while (cur != QPointF(-1, -1))
	{
		path.append(cur);
		cur = parent[cur.y()][cur.x()];
		
	}
	std::reverse(path.begin(), path.end());
	for(int i = 0;i< path.size();i++)
	{
		qDebug() << path[i] <<"  ";
	}
	
	return smoothPath(path, 2);
}


// 深度优先最速寻找路径
QVector<QPointF> Controller::DFS_find_shortest_path()
{
	if (m_common->state() != Common::State::Idle)
	{
		qDebug() << "State not idle!";
		return m_path;
	}
	m_common->setState(Common::State::Searching);
	m_visitpath.clear();
	m_visitpath.append(m_agv->position());
	double x_pos, y_pos;
	Position_update(&x_pos, &y_pos);
	// 当前所在格子（不是目标格子！）
	int curCellX = static_cast<int>(x_pos / 10);
	int curCellY = static_cast<int>(y_pos / 10);
	// 监控格子是否被访问过
	QVector<QVector<bool>> visited(10, QVector<bool>(10,false ) );
	// 存放走过的路径
	QVector<QPointF> path;
	// 寻找路径
	DFS_find_path(curCellX, curCellY, 9, 9, visited, path);

	return path;
}
// 深度优先算法
bool Controller::DFS_find_path(int x, int y, int endCellX, int endCellY, QVector<QVector<bool>>& visited, QVector<QPointF>& path)
{
	if (x < 0 || x > 10 || y < 0 || y > 10)
		return false;
	if (visited[y][x])
		return false;
	qDebug() << y << "x" << x;
	visited[y][x] = true;
	
	path.append(QPointF(x, y));
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
void Controller::startMove(QPoint from, QPoint to)
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
void Controller::startMoveAlongPath(const QVector<QPointF>& path)
{
	if (m_common->state() != Common::State::Searching)
	{
		qDebug() << "State not idle!";
		return;
	}
	m_common->setState(Common::State::AnimatingPath);
	if (path.size() < 2)
		return;
	m_path = path;
	m_pathIndex = 0;
	prepareNextSegment();
}

void Controller::prepareNextSegment()
{
	if (m_pathIndex >= m_path.size() - 1)
	{
		//if (m_moveTimer)
		//{
			m_moveTimer->stop();
			m_common->setState(Common::State::Idle);
		//}
		return;
	}
	QPointF a = m_path[m_pathIndex];
	QPointF b = m_path[m_pathIndex + 1];
	// 格子坐标 → 像素坐标（你现在是 10px 一格）
	m_startPos = QPointF(a.x() * 10 + 5, a.y() * 10 + 5);
	m_targetPos = QPointF(b.x() * 10 + 5, b.y() * 10 + 5);
	m_t = 0.0;

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
		m_pathIndex++;            // ⭐ 走到下一个格子
		prepareNextSegment();     // ⭐ 继续下一段
		
		return;
	}

	// 线性插值：pos = start + (target - start) * t
	QPointF pos = m_startPos + (m_targetPos - m_startPos) * m_t;
	m_agv->setPosition(pos.x(), pos.y());
	if (m_common->state() == Common::State::AnimatingPath)
		m_visitpath.append(pos);
	//emit reachedIndexChanged(m_pathIndex);
	emit visitPathChanged(m_visitpath);
}
// 小车回原
void Controller::AGV_back(QVector<QVector<Map::Cell>> maze)
{
	m_common->setState(Common::State::Idle);
	m_visitpath.clear();
	m_path.clear();
	m_pathIndex = 0;
	m_maze = maze;
	m_agv->reset(maze);
	startMove(m_agv->position(), QPoint(5, 5));
}
// 初始化函数
void Controller::Init()
{
	m_agv = new AGV(m_maze, this);
	m_common = new Common(this);
	if (!m_moveTimer)
	{
		m_moveTimer = new QTimer(this);
		connect(m_moveTimer, &QTimer::timeout,
			this, &Controller::onMoveTick);
	}
}
Controller::~Controller()
{
}
