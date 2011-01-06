// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101203.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_FRAME_H_
#define _RE_ANIM_FRAME_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include "Core\ReAnimDef.h"
#include "Core\ReAnimNode.h"
#include <QVariant>


namespace RE
{


// -----------------------------------------------------------------------------
// Frame base.
// -----------------------------------------------------------------------------
class ReAnimFrame : public ReAnimNode
{
public:
	typedef ReAnimNode TSuper;
	ReAnimFrame( qreal _time ): m_time( _time )	{}

	bool operator<( ReAnimFrame* _other ) const
	{
		return m_time < _other->GetTime();
	}

	virtual QString	GetDescription() const
	{
		return QString( QObject::tr( "Time: %1" ) ).arg( m_time );
	}

	qreal	GetTime() const			{ return m_time; }
	void	SetTime( qreal _time )	{ m_time = _time; }

	virtual void SetFrameValue( qreal _value, int _index ) = 0;	
	virtual void SetFrameValue( const QVariant& _value ) = 0;

protected:
	qreal	m_time;
};


// -----------------------------------------------------------------------------
// Translation frame.
// -----------------------------------------------------------------------------
class ReAnimTranslationFrame : public ReAnimFrame
{
public:
	typedef ReAnimFrame TSuper;
	enum eValue { EValue_X, EValue_Y, EValue_Count };

public:
	ReAnimTranslationFrame( qreal _time )
		: TSuper( _time )
	{
		m_values[ EValue_X ].SetName( QObject::tr( "X" ) );
		m_values[ EValue_Y ].SetName( QObject::tr( "Y" ) );
	}

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
	virtual QVariant GetDataVariant() const
	{
		return QPointF( m_values[ EValue_X ].GetDataVariant().toDouble(),
			m_values[ EValue_Y ].GetDataVariant().toDouble() );
	}

	// -------------------------------------------------------------------------
	// Override ReAnimFrame.
	// -------------------------------------------------------------------------
	virtual void SetFrameValue( qreal _value, int _index )
	{
		m_values[ _index ].SetValue( _value );
	}

	virtual void SetFrameValue( const QVariant& _value )
	{
		QPointF pos = _value.toPointF();
		SetFrameValue( pos.x(), EValue_X );
		SetFrameValue( pos.y(), EValue_Y );
	}

	virtual QString GetDescription() const
	{
		return QString( QObject::tr( "%1\r\nPosition: %2, %3" ) )
			.arg( TSuper::GetDescription() )
			.arg( m_values[ EValue_X ].GetDataVariant().toDouble() )
			.arg( m_values[ EValue_Y ].GetDataVariant().toDouble() );
	}

protected:
	ReAnimValue	m_values[ EValue_Count ];
};


// -----------------------------------------------------------------------------
// Rotation frame.
// -----------------------------------------------------------------------------
class ReAnimRotationFrame : public ReAnimFrame
{
public:
	typedef ReAnimFrame TSuper;
	enum eValue { EValue_Angle, EValue_Count };

public:
	ReAnimRotationFrame( qreal _time )
		: TSuper( _time )
	{
		m_values[ EValue_Angle ].SetName( QObject::tr( "Angle" ) );
	}

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
	virtual QVariant GetDataVariant() const
	{
		return m_values[ EValue_Angle ].GetDataVariant().toDouble();
	}

	// -------------------------------------------------------------------------
	// Override ReAnimFrame.
	// -------------------------------------------------------------------------
	virtual void SetFrameValue( qreal _value, int _index )
	{
		m_values[ _index ].SetValue( _value );
	}

	virtual void SetFrameValue( const QVariant& _value )
	{
		qreal angle = _value.toDouble();
		SetFrameValue( angle, EValue_Angle );
	}

	virtual QString GetDescription() const
	{
		return QString( QObject::tr( "%1\r\nRotation: %2" ) )
			.arg( TSuper::GetDescription() )
			.arg( m_values[ EValue_Angle ].GetDataVariant().toDouble() );
	}

protected:
	ReAnimValue	m_values[ EValue_Count ];
};


// -----------------------------------------------------------------------------
// Scaling frame.
// -----------------------------------------------------------------------------
class ReAnimScaleFrame : public ReAnimFrame
{
public:
	typedef ReAnimFrame TSuper;
	enum eValue { EValue_X, EValue_Y, EValue_Count };

public:
	ReAnimScaleFrame( qreal _time )
		: TSuper( _time )
	{
		m_values[ EValue_X ].SetName( QObject::tr( "X" ) );
		m_values[ EValue_Y ].SetName( QObject::tr( "Y" ) );

		SetFrameValue( 1.0f, 1.0f );
	}

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
	virtual QVariant GetDataVariant() const
	{
		return QPointF( m_values[ EValue_X ].GetDataVariant().toDouble(),
			m_values[ EValue_Y ].GetDataVariant().toDouble() );
	}

	// -------------------------------------------------------------------------
	// Override ReAnimFrame.
	// -------------------------------------------------------------------------
	virtual void SetFrameValue( qreal _value, int _index )
	{
		m_values[ _index ].SetValue( _value );
	}

	virtual void SetFrameValue( const QVariant& _value )
	{
		QPointF pos = _value.toPointF();
		SetFrameValue( pos.x(), EValue_X );
		SetFrameValue( pos.y(), EValue_Y );
	}

	virtual QString GetDescription() const
	{
		return QString( QObject::tr( "%1\r\nScale: %2, %3" ) )
			.arg( TSuper::GetDescription() )
			.arg( m_values[ EValue_X ].GetDataVariant().toDouble() )
			.arg( m_values[ EValue_Y ].GetDataVariant().toDouble() );
	}

protected:
	ReAnimValue	m_values[ EValue_Count ];
};


// -----------------------------------------------------------------------------
// Alpha frame.
// -----------------------------------------------------------------------------
class ReAnimAlphaFrame : public ReAnimFrame
{
public:
	typedef ReAnimFrame TSuper;
	enum eValue { EValue_Alpha, EValue_Count };

public:
	ReAnimAlphaFrame( qreal _time )
		: TSuper( _time )
	{
		m_values[ EValue_Alpha ].SetName( QObject::tr( "Alpha" ) );
	}

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
	virtual QVariant GetDataVariant() const
	{
		return m_values[ EValue_Alpha ].GetDataVariant().toDouble();
	}

	// -------------------------------------------------------------------------
	// Override ReAnimFrame.
	// -------------------------------------------------------------------------
	virtual void SetFrameValue( qreal _value, int _index )
	{
		m_values[ _index ].SetValue( _value );
	}

	virtual void SetFrameValue( const QVariant& _value )
	{
		qreal alpha = _value.toDouble();
		SetFrameValue( alpha, EValue_Alpha );
	}

	virtual QString GetDescription() const
	{
		return QString( QObject::tr( "%1\r\nAlpha: %2" ) )
			.arg( TSuper::GetDescription() )
			.arg( m_values[ EValue_Alpha ].GetDataVariant().toDouble() );
	}

protected:
	ReAnimValue	m_values[ EValue_Count ];
};


}
#endif	// _RE_ANIM_FRAME_H_
