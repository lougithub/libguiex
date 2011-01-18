// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20110116.
// -----------------------------------------------------------------------------
#ifndef _RE_EDITOR_ZOOMINFO_H_
#define _RE_EDITOR_ZOOMINFO_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include <QObject>


namespace RE
{


class ReZoomInfo : public QObject
{
	Q_OBJECT
	typedef QObject TSuper;
	typedef int T;
	
	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------
public:
	ReZoomInfo(): m_scalar( ( T )1 ), m_min( ( T )0 ), m_max( ( T )1 ), m_delta( ( T )0 ), m_isEnabled( false ) {}

	ReZoomInfo( T _scalar, T _min, T _max, T _delta )
		: m_scalar( _scalar )
		, m_min( _min )
		, m_max( _max )
		, m_delta( _delta )
		, m_isEnabled( true )
	{
	}

	T GetScalar() const
	{
		return m_scalar;
	}

	void Init( T _scalar, T _min, T _max, T _delta )
	{
		m_scalar = _scalar;
		m_min = _min;
		m_max = _max;
		m_delta = _delta;
		m_isEnabled = true;
	}

	void Reset( T _scalar )
	{
		m_scalar = _scalar;
		m_isEnabled = true;

		emit ScalarChanged( m_scalar );
	}

	void Zoom( int _step )
	{
		if( m_isEnabled )
		{
			m_scalar += _step * m_delta;
			if( m_scalar > m_max )
				m_scalar = m_max;
			else if( m_scalar < m_min )
				m_scalar = m_min;

			emit ScalarChanged( m_scalar );
		}
	}

	bool CanZoomIn() const
	{
		return m_isEnabled ? m_scalar < m_max : false;
	}

	bool CanZoomOut() const
	{
		return m_isEnabled ? m_scalar > m_min : false;
	}

	void Enable( bool _isEnable )
	{
		m_isEnabled = _isEnable;
	}

	bool IsEnabled() const
	{
		return m_isEnabled;
	}

	// -------------------------------------------------------------------------
	// Signals.
	// -------------------------------------------------------------------------
signals:
	void				ScalarChanged( int _scalar );

protected:
	T					m_scalar;
	T					m_delta;
	T					m_min;
	T					m_max;
	bool				m_isEnabled;
};


}
#endif	// _RE_EDITOR_ZOOMINFO_H_
