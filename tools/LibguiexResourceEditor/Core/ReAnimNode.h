// -----------------------------------------------------------------------------
// Author: GameCrashDebug.
// Date: 20101203.
// -----------------------------------------------------------------------------
#ifndef _RE_ANIM_ROOT_H_
#define _RE_ANIM_ROOT_H_
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#include "Core\ReAnimDef.h"
#include <QVariant>


namespace RE
{


class ReAnimNode
{
	// -------------------------------------------------------------------------
	// General.
	// -------------------------------------------------------------------------
public:
	ReAnimNode(): m_parent( NULL ), m_name( QObject::tr( "AnimNode" ) ) {}
	virtual ~ReAnimNode() {}

	ReAnimNode*			GetParent() const					{ return m_parent; }
	void				SetParent( ReAnimNode* _p )			{ m_parent = _p; }

	QString				GetName() const						{ return m_name; }
	void				SetName( const QString& _name )		{ m_name = _name; }

	virtual QVariant	GetNameVariant() const				{ return m_name; }
	virtual QVariant	GetDataVariant() const				{ return QVariant(); }
	virtual int			IndexOfChild( ReAnimNode* )	const	{ return -1; }

	// -------------------------------------------------------------------------
	// Variables.
	// -------------------------------------------------------------------------
protected:
	ReAnimNode*			m_parent;
	QString				m_name;
};


class ReAnimValue : public ReAnimNode
{
public:
	ReAnimValue( qreal _value = 0.0f ): m_value( _value ) {}

	virtual QVariant	GetDataVariant() const				{ return m_value; }
	void				SetValue( qreal _value )			{ m_value = _value; }

protected:
	qreal				m_value;
};


}
#endif	// _RE_ANIM_ROOT_H_
