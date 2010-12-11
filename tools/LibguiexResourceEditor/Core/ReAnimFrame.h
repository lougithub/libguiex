// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101203.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_FRAME_H_
#define _RE_ANIM_FRAME_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include "Core\ReAnimDef.h"
#include "Core\ReAnimItem.h"
#include <QVariant>


namespace RE
{


class ReAnimFrame : public ReAnimItem
{
	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	typedef ReAnimItem	TSuper;

	enum { VALUE_COUNT = 2 };

	ReAnimFrame( qreal _time );

	//int					GetId() const				{ return m_id; }
	//int&				GetIdRef()					{ return m_id; }
	qreal				GetTime() const				{ return m_time; }
	qreal&				GetTimeRef()				{ return m_time; }
	ReAnimValueItem		GetValue( int _i ) const	{ return m_values[ _i ]; }
	ReAnimValueItem&	GetValueRef( int _i )		{ return m_values[ _i ]; }

	bool operator<( ReAnimFrame* _other ) const;

	// -------------------------------------------------------------------------
	// Override ReAnimItem.
	// -------------------------------------------------------------------------
public:
	virtual QVariant	ToKeyVariant() const;
	virtual QVariant	ToValueVariant() const;
	virtual QVariant	ToVariant() const;

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	//int					m_id;
	qreal				m_time;
	ReAnimValueItem		m_values[ VALUE_COUNT ];
};


}
#endif	// _RE_ANIM_FRAME_H_
