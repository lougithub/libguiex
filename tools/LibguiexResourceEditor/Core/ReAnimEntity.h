// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101220.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_ENTITY_H_
#define _RE_ANIM_ENTITY_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include "Core\ReAnimNode.h"
#include "Core\ReAnimDef.h"
#include <QVariant>


namespace RE
{


class ReAnimTrack;


class ReAnimEntity : public ReAnimNode
{
	typedef ReAnimNode	TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReAnimEntity();
	virtual ~ReAnimEntity();

	virtual ReAnimTrack*	CreateTrack( eTrackType _type );

	qreal					GetTotalLength() const;

protected:
	ReAnimTrack*			m_tracks[ ETrackType_Count ];
};



}
#endif	// _RE_ANIM_ENTITY_H_
