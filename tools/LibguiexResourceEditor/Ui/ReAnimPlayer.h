// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110109.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_PLAYER_H_
#define _RE_ANIM_PLAYER_H_


#include <QObject>
#include <QTimer>


namespace RE
{


class ReRulerWidget;


class ReAnimPlayer : public QObject
{
	Q_OBJECT

	// ----------------------------------------------------------------------------
	// General.
	// ----------------------------------------------------------------------------
public:
	enum ePlayMode
	{
		EPlayMode_Loop,
		EPlayMode_Once,
		EPlayMode_BounceOnce,
		EPlayMode_BounceLoop,
		EPlayMode_Count
	};

public:
	ReAnimPlayer();

	void				Setup( ReRulerWidget* _ruler );
	void				Release();
	void				Play( qreal _startTiime, qreal _endTime, ePlayMode _mode = EPlayMode_Loop );
	void				Stop();

	bool				IsPlaying() const { return m_isPlaying; }

	// ----------------------------------------------------------------------------
	// Signals.
	// ----------------------------------------------------------------------------
signals:
	void				Ended();
	void				Looped();

	// ----------------------------------------------------------------------------
	// Slots.
	// ----------------------------------------------------------------------------
public slots:
	void				OnPulse();

	// ----------------------------------------------------------------------------
	// Variables.
	// ----------------------------------------------------------------------------
protected:
	ReRulerWidget*		m_ruler;
	int					m_rulerSnap;
	qreal				m_startTime;
	qreal				m_endTime;
	qreal				m_currentTime;
	qreal				m_fps;
	qreal				m_delta;
	ePlayMode			m_mode;
	bool				m_isPlaying;

	QTimer				m_timer;
};


}
#endif	// _RE_ANIM_PLAYER_H_
