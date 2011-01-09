// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110109.
// -----------------------------------------------------------------------------
#include "StdAfxEditor.h"
#include "Ui\ReAnimPlayer.h"
#include "Ui\ReRulerWidget.h"
#include "Ui\ReAnimUiInfo.h"


namespace RE
{


// -----------------------------------------------------------------------------
// General.
// -----------------------------------------------------------------------------
ReAnimPlayer::ReAnimPlayer()
: m_ruler( NULL )
, m_rulerSnap( 1 )
, m_startTime( 0.0f )
, m_endTime( 0.0f )
, m_currentTime( 0.0f )
, m_fps( ANIM_FPS_DEFAULT )
, m_delta( 1.0f / m_fps )
, m_mode( EPlayMode_Loop )
, m_isPlaying( false )
{
	m_timer.setSingleShot( false );
	connect( &m_timer, SIGNAL( timeout() ), this, SLOT( OnPulse() ) );
}


void ReAnimPlayer::Setup( ReRulerWidget* _ruler )
{
	m_ruler = _ruler;
	if( NULL != m_ruler )
		m_rulerSnap = m_ruler->GetSnap();
}


void ReAnimPlayer::Release()
{
	m_ruler = NULL;
}


void ReAnimPlayer::Play( qreal _startTiime, qreal _endTime, ePlayMode _mode )
{
	if( NULL != m_ruler && !m_isPlaying )
	{
		m_isPlaying = true;

		m_startTime = 0.0f;
		m_endTime = _endTime;
		m_currentTime = _startTiime;
		m_mode = _mode;

		m_delta = qAbs( m_delta );

		m_ruler->SetSnap( 1 );
		m_timer.start( 1000.0f / m_fps );
	}
}


void ReAnimPlayer::Stop()
{
	m_isPlaying = false;
	m_timer.stop();
	m_ruler->SetSnap( m_rulerSnap );
}


// -----------------------------------------------------------------------------
// Slots.
// -----------------------------------------------------------------------------
void ReAnimPlayer::OnPulse()
{
	m_currentTime += m_delta;
	if( m_currentTime > m_endTime )
	{
		if( EPlayMode_Loop == m_mode )
		{
			while( m_currentTime >= m_endTime )
				m_currentTime -= ( m_endTime - m_startTime );

			emit Looped();
		}
		else if( EPlayMode_Once == m_mode )
		{
			m_currentTime = m_endTime;
			Stop();

			emit Ended();
		}
		else if( EPlayMode_BounceOnce == m_mode )
		{
			m_currentTime = m_endTime - ( m_currentTime - m_endTime );
			m_delta = -qAbs( m_delta );
		}
		else if( EPlayMode_BounceLoop == m_mode )
		{			
			m_currentTime = m_endTime - ( m_currentTime - m_endTime );
			m_delta = -m_delta;

			emit Looped();
		}
	}
	else if( m_currentTime < m_startTime )
	{
		if( EPlayMode_BounceOnce == m_mode )
		{
			m_currentTime = 0;
			Stop();

			emit Ended();
		}
		else if( EPlayMode_BounceLoop == m_mode )
		{
			m_currentTime = m_startTime - m_currentTime;
			m_delta = -m_delta;

			emit Looped();
		}
	}

	//if( m_isPlaying )
	{
		m_ruler->SetValue( m_currentTime, true );
	}
}


}
