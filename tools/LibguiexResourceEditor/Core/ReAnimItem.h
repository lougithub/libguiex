// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101203.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_ITME_H_
#define _RE_ANIM_ITME_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include "Core\ReAnimDef.h"
#include <QVariant>


namespace RE
{


class ReAnimItem
{
	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReAnimItem(): m_parent( NULL ), m_type( ETrackType_Translation ) {}
	virtual ~ReAnimItem() {}

	ReAnimItem*			GetParent() const	{ return m_parent; }
	ReAnimItem*&		GetParentRef()		{ return m_parent; }

	eTrackType			GetType() const		{ return m_type; }
	eTrackType&			GetTypeRef()		{ return m_type; }

	QString				GetName() const		{ return m_name; }
	QString&			GetNameRef()		{ return m_name; }

	virtual QVariant	ToKeyVariant() const				{ return m_name; }
	virtual QVariant	ToValueVariant() const				{ return QVariant(); }
	virtual int			IndexOfChild( ReAnimItem* )	const	{ return -1; }
	virtual QVariant	ToVariant() const					{ return QVariant(); }

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	ReAnimItem*			m_parent;
	QString				m_name;
	eTrackType			m_type;
};


class ReAnimValueItem : public ReAnimItem
{
public:
	ReAnimValueItem( qreal _data = 0.0f ): m_data( _data ) {}

	qreal				GetData() const		{ return m_data; }
	qreal&				GetDataRef()		{ return m_data; }

	// Override ReAnimItem.
	virtual QVariant	ToValueVariant() const				{ return m_data; }
	virtual QVariant	ToVariant() const					{ return m_data; }

protected:
	qreal				m_data;
};


}
#endif	// _RE_ANIM_ITME_H_
