// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101203.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_TRACK_H_
#define _RE_ANIM_TRACK_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include "Core\ReAnimNode.h"
#include "Core\ReAnimFrame.h"
#include <QList>


namespace RE
{


// -----------------------------------------------------------------------------
// Track base.
// -----------------------------------------------------------------------------
class ReAnimTrack : public ReAnimNode
{
	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	typedef ReAnimNode	TSuper;

	ReAnimTrack();
	virtual ~ReAnimTrack();

	ReAnimFrame*		CreateFrame( qreal _time );
	void				DeleteFrame( ReAnimFrame* _frame );
	ReAnimFrame*		GetFrameByIndex( int _index );
	bool				HasFrame( const ReAnimFrame* _frame ) const;
	int					GetFrameCount() const	{ return m_frames.size(); }
	bool				Interpolate( qreal _time, QVariant& _result, bool _allowExterpolate );

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
public:
	virtual int			IndexOfChild( const ReAnimNode* _child );

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	virtual ReAnimFrame*DoCreateFrame( qreal _time ) = 0;
	virtual QVariant	InterpolateFrameValue( const QVariant& _valueA, const QVariant& _valueB, qreal _factor ) const = 0;
	void				GetNearestFrames( qreal _time, ReAnimFrame*& _left, ReAnimFrame*& _right );	

	bool				IsDirty() const	{ return m_isSortDirty; }
	void				Sort()			{ qSort( m_frames ); m_isSortDirty = false; }

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	typedef QList< ReAnimFrame* >		TFrameList;
	typedef TFrameList::iterator		TFrameListItor;
	typedef TFrameList::const_iterator	TFrameListCItor;

	TFrameList			m_frames;
	bool				m_isSortDirty;
};


// -----------------------------------------------------------------------------
// Translation track.
// -----------------------------------------------------------------------------
class ReAnimTranslationTrack : public ReAnimTrack
{
public:
	typedef ReAnimTrack TSuper;

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
protected:
	virtual ReAnimFrame* DoCreateFrame( qreal _time )
	{
		return new ReAnimTranslationFrame( _time );
	}

	virtual QVariant InterpolateFrameValue( const QVariant& _valueA, const QVariant& _valueB, qreal _factor ) const
	{
		QPointF valueA = _valueA.toPointF();
		QPointF valueB = _valueB.toPointF();

		return valueA * _factor + valueB * ( 1.0f - _factor );
	}
};


// -----------------------------------------------------------------------------
// Rotation track.
// -----------------------------------------------------------------------------
class ReAnimRotationTrack : public ReAnimTrack
{
public:
	typedef ReAnimTrack TSuper;

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
protected:
	virtual ReAnimFrame* DoCreateFrame( qreal _time )
	{
		return new ReAnimRotationFrame( _time );
	}

	virtual QVariant InterpolateFrameValue( const QVariant& _valueA, const QVariant& _valueB, qreal _factor ) const
	{
		qreal valueA = _valueA.toDouble();
		qreal valueB = _valueB.toDouble();

		return valueA * _factor + valueB * ( 1.0f - _factor );
	}
};



// -----------------------------------------------------------------------------
// Scaling track.
// -----------------------------------------------------------------------------
class ReAnimScaleTrack : public ReAnimTrack
{
public:
	typedef ReAnimTrack TSuper;

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
protected:
	virtual ReAnimFrame*DoCreateFrame( qreal _time )
	{
		return new ReAnimScaleFrame( _time );
	}

	virtual QVariant InterpolateFrameValue( const QVariant& _valueA, const QVariant& _valueB, qreal _factor ) const
	{
		QPointF valueA = _valueA.toPointF();
		QPointF valueB = _valueB.toPointF();

		return valueA * _factor + valueB * ( 1.0f - _factor );
	}
};


// -----------------------------------------------------------------------------
// Alpha track.
// -----------------------------------------------------------------------------
class ReAnimAlphaTrack : public ReAnimTrack
{
public:
	typedef ReAnimTrack TSuper;

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
protected:
	virtual ReAnimFrame*DoCreateFrame( qreal _time )
	{
		return new ReAnimAlphaFrame( _time );
	}

	virtual QVariant InterpolateFrameValue( const QVariant& _valueA, const QVariant& _valueB, qreal _factor ) const
	{
		qreal valueA = _valueA.toDouble();
		qreal valueB = _valueB.toDouble();

		return valueA * _factor + valueB * ( 1.0f - _factor );
	}
};


}
#endif	// _RE_ANIM_TRACK_H_
