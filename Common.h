#pragma once
#include <QObject>
class Common : public QObject
{
    Q_OBJECT
public:
    Common(QObject *parent = nullptr);
    enum class State
    {
        Idle,          // 空闲
        ManualMove,    // 手动按键控制
        Searching,     // BFS / DFS 搜索中
        AnimatingPath, // 沿路径自动运动
        Paused         // 可选：暂停
    };
    State state() const { return m_state; }
    void setState(State s);

private:
    State m_state;
};

