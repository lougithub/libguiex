// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101203.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_TRACK_H_
#define _RE_ANIM_TRACK_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include "Core\ReAnimItem.h"
#include "Core\ReAnimFrame.h"
#include <QList>


namespace RE
{


class ReAnimTrack : public ReAnimItem
{
	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	typedef ReAnimItem	TSuper;

	ReAnimTrack();
	virtual ~ReAnimTrack();

	ReAnimFrame*		CreateFrame( qreal _time );
	void				DeleteFrame( ReAnimFrame* _frame );
	ReAnimFrame*		GetFrameByIndex( int _index );
	bool				HasFrame( const ReAnimFrame& _frame ) const;
	int					GetFrameCount() const	{ return m_frames.size(); }

	// -------------------------------------------------------------------------
	// Override ReAnimItem.
	// -------------------------------------------------------------------------
public:
	virtual int			IndexOfChild( const ReAnimItem* _child );

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Utilities.
	// -------------------------------------------------------------------------
protected:
	bool				IsDirty() const;
	void				Sort();

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


}
#endif	// _RE_ANIM_TRACK_H_
