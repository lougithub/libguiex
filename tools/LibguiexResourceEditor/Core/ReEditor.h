// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101109.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_EDITOR_H_
#define _RE_EDITOR_EDITOR_H_

#include <QThread>

class QApplication;
class QSemaphore;


namespace RE
{


class ReMainWindow;


class ReEditor : public QThread
{
	Q_OBJECT

public:
	ReEditor( QSemaphore* _lock, int _argc, char* _argv[], QObject* _parent = NULL );

protected:
	virtual void		run();

public slots:
	void				Sleep( unsigned long _msec );
	void				Tick();
	void				Shutdown();

protected:
	QApplication*		m_application;
	ReMainWindow*		m_mainWindow;

	QSemaphore*			m_lock;
	int					m_argc;
	char**				m_argv;
};


}		// namespace RE
#endif	// _RE_EDITOR_EDITOR_H_