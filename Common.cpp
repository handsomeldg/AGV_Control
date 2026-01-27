#include "Common.h"
#include <QDebug>
Common::Common(QObject* parent)
{
	m_state = State::Idle;
}
void Common::setState(State s)
{
	if (m_state == s)
		return;
	m_state = s;
	qDebug() << "State:" << (int)m_state << "->" << (int)s;
}
