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
class ReAnimModel;


class ReAnimEntity : public ReAnimNode
{
	typedef ReAnimNode	TSuper;

	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReAnimEntity( ReAnimModel* _model );
	virtual ~ReAnimEntity();

	virtual ReAnimTrack*CreateTrack( eTrackType _type );

	qreal				GetTotalLength() const;

	// -------------------------------------------------------------------------
	// Override ReAnimNode.
	// -------------------------------------------------------------------------
public:
	virtual QVariant	GetNameVariant() const	{ return QObject::tr( "Entity" ); }
	virtual QVariant	GetDataVariant() const	{ return m_name; }

	virtual ReAnimNode*	CreateChild( const QVariant& _arg );
	virtual void		DestroyChild( ReAnimNode* _child );
	virtual int			GetChildrenCount() const { return ETrackType_Count; }
	virtual ReAnimNode*	GetChild( int _index );
	virtual int			IndexOfChild( const ReAnimNode* _child );

protected:
	ReAnimTrack*		m_tracks[ ETrackType_Count ];
};



}
#endif	// _RE_ANIM_ENTITY_H_
