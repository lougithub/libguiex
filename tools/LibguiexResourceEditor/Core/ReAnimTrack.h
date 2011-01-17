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
#include <list>


namespace RE
{


class ReAnimModel;


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

	ReAnimTrack( ReAnimModel* _model ): TSuper( _model ) {}
	virtual ~ReAnimTrack();

	ReAnimFrame*		CreateFrame( qreal _time );
	void				DeleteFrame( ReAnimFrame* _frame );
	bool				HasFrame( const ReAnimFrame* _frame ) const;
	int					GetFrameCount() const	{ return ( int )m_frames.size(); }
	bool				Interpolate( qreal _time, QVariant& _result, bool _allowExterpolate );
	qreal				GetTotalLength();

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
public:	
	virtual ReAnimNode*	CreateChild( const QVariant& _arg );
	virtual void		DestroyChild( ReAnimNode* _child );
	virtual int			GetChildrenCount() const { return ( int )m_frames.size(); }
	virtual ReAnimNode*	GetChild( int _index );
	virtual int			IndexOfChild( const ReAnimNode* _child );

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	virtual ReAnimFrame*DoCreateFrame( qreal _time ) = 0;
	virtual QVariant	InterpolateFrameValue( const QVariant& _valueA, const QVariant& _valueB, qreal _factor ) const = 0;
	void				GetNearestFrames( qreal _time, ReAnimFrame*& _left, ReAnimFrame*& _right );		

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	typedef std::list< ReAnimFrame* >	TFrameList;
	typedef TFrameList::iterator		TFrameListItor;
	typedef TFrameList::const_iterator	TFrameListCItor;

	TFrameList			m_frames;
};


// -----------------------------------------------------------------------------
// Translation track.
// -----------------------------------------------------------------------------
class ReAnimTranslationTrack : public ReAnimTrack
{
public:
	typedef ReAnimTrack TSuper;
	ReAnimTranslationTrack( ReAnimModel* _model ): TSuper( _model ) {}

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
protected:
	virtual ReAnimFrame* DoCreateFrame( qreal _time )
	{
		return new ReAnimTranslationFrame( GetModel(), _time );
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
	ReAnimRotationTrack( ReAnimModel* _model ): TSuper( _model ) {}

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
protected:
	virtual ReAnimFrame* DoCreateFrame( qreal _time )
	{
		return new ReAnimRotationFrame( GetModel(), _time );
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
	ReAnimScaleTrack( ReAnimModel* _model ): TSuper( _model ) {}

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
protected:
	virtual ReAnimFrame*DoCreateFrame( qreal _time )
	{
		return new ReAnimScaleFrame( GetModel(), _time );
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
	ReAnimAlphaTrack( ReAnimModel* _model ): TSuper( _model ) {}

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
protected:
	virtual ReAnimFrame*DoCreateFrame( qreal _time )
	{
		return new ReAnimAlphaFrame( GetModel(), _time );
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
